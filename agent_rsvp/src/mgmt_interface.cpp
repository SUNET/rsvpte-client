/***************************************************************************
 *   This is an implementation of OIF UNI-C RSVP and IETF UNI-C RSVP.      *
 *   The code was created for purpose of EC IST Mupbed project.            *
 *   Copyright (C) 2007 by Poznan Supercomputing and Networking Center     *
 *   http://www.man.poznan.pl                                              *
 *                                                                         *     
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#ifndef MGMT_INTERFACE_BODY_FILE
#define MGMT_INTERFACE_BODY_FILE

#include "mgmt_interface.h"

extern rsvp_globals globals;

//========================================================================================

mgmt_interface::mgmt_interface(rsvp_dispatcher* disp)
{
    struct sockaddr_in loc_addr;
    state                       = STATE_LISTEN;
    dispatcher                  = disp;
    m_console_addr              = 0;
    paths_count                 = 0;
    socket_descriptor           = -1;
    
#ifdef MGMT_LOCAL_ADDRESS
    loc_addr.sin_addr.s_addr    = inet_addr(MGMT_LOCAL_ADDRESS);
#else
    loc_addr.sin_addr.s_addr    = 0;
#endif
    loc_addr.sin_family         = AF_INET;
    loc_addr.sin_port           = htons(globals.management_port);

    socket_descriptor_listen    = socket(PF_INET, SOCK_STREAM, 6);
    bind(socket_descriptor_listen, (struct sockaddr*)&loc_addr, sizeof(loc_addr));
    listen(socket_descriptor_listen, 1);
    VERBOSE(3, "mgmt_interface::agent is listening mngt messages on %s:%i\n", MGMT_LOCAL_ADDRESS, globals.management_port);
    init();    
}

//----------------------------------------------------------------------------------------

/* this constructor will let you connect to mgmt application or other agent's instance */
mgmt_interface::mgmt_interface(rsvp_dispatcher* disp, unsigned int address)
{
    dispatcher                  = disp;
    m_console_addr              = 0;
    paths_count                 = 0;
    mngt_server_address 	= address;
    connect_to_mngt_server();
    init();
}

//----------------------------------------------------------------------------------------
void mgmt_interface::connect_to_mngt_server()
{
    struct sockaddr_in rem_addr;
    
    state                       = STATE_CONNECT;
    socket_descriptor_listen    = -1;
    socket_descriptor           = socket(PF_INET, SOCK_STREAM, 6);
    rem_addr.sin_family         = AF_INET;
    rem_addr.sin_port           = htons(globals.management_port);
    rem_addr.sin_addr.s_addr    = mngt_server_address;
    int ret_value = connect(socket_descriptor, (struct sockaddr*)&rem_addr, sizeof(rem_addr));
    if(ret_value == 0){
    	VERBOSE(3, "mgmt_interface::connect to %s succesful\n", int_2_addr(mngt_server_address));
    }
    else {
    	VERBOSE(0, "mgmt_interface::connect to %s unsuccesful\n", int_2_addr(mngt_server_address));
    }
}

//----------------------------------------------------------------------------------------

void mgmt_interface::init()
{
    int t;
    for(t=0;t<MAX_PENDING_RSVP_PACKETS;t++)
    {
        pending_rsvp_packets[t].cid = -1;
        pending_rsvp_packets[t].mtype = -1;
        pending_rsvp_packets[t].mmid = -1;
        pending_rsvp_packets[t].waiting_confirmation = false;
    }
    for(t=0;t<MAX_PENDING_MGMT_PACKETS;t++)
    {
        pending_mgmt_packets[t].packet = NULL;
        pending_mgmt_packets[t].length = -1;
    }
    local_next_m_id = remote_next_m_id = 0;
    pthread_create(&listener_tid,        NULL, (void*(*)(void*))(listener),        this);
    pthread_create(&socket_listener_tid, NULL, (void*(*)(void*))(socket_listener), this);
    name_thread("mgmt_interface::listener",        listener_tid);
    name_thread("mgmt_interface::socket_listener", socket_listener_tid);
    dispatcher->register_interface(this);
}

//----------------------------------------------------------------------------------------
        
mgmt_interface::~mgmt_interface()
{
    dispatcher->unregister_interface(this);
    close(socket_descriptor_listen);
    close(socket_descriptor);
}

//----------------------------------------------------------------------------------------

void mgmt_interface::listener(mgmt_interface* i)
{
    while(1)
    {
        i->wait_for_touch();
        
        /* check pending_rsvp_packets */
        for(int t=0; t<MAX_PENDING_RSVP_PACKETS; t++)
            i->check_pending_rsvp_packet(t);
            
        /* check socket for mgmt messages */
        for(int k=0; k<MAX_PENDING_MGMT_PACKETS; k++)
            i->check_pending_mgnt_packet(k);

        // check dispatcher state - we have nothing to do here,
        // but why don't we send a status, or sth? :)
    }
}

//----------------------------------------------------------------------------------------

void mgmt_interface::socket_listener(mgmt_interface* i)
{
    while(1)
    {
        if(i->socket_descriptor_listen > 0 && i->socket_descriptor == -1)
            i->accepting_connection();
        else if(i->socket_descriptor_listen < 0 && i->socket_descriptor != -1)
            i->read_mgnt_packets();

        VERBOSE(7, "socket_descriptor is %d and socket_descriptor_listen is %d\n",
                 i->socket_descriptor, i->socket_descriptor_listen);
    }
}

//----------------------------------------------------------------------------------------

void mgmt_interface::check_pending_rsvp_packet(int packet_index)
{
    pending_rsvp_packet* pend_pack  = &pending_rsvp_packets[packet_index];
    rsvp_configuration*  conf       = &pend_pack->conf;
        
    if(pend_pack->cid < 0)
        return;
        
    VERBOSE(7, "checking pending rsvp packet numero %d\n", packet_index);
    if(pend_pack->mtype == MTYPE_PATH)
    {
        if(pend_pack->waiting_confirmation == true)
            return;
            
        VERBOSE(3, "mgmt_interface - RSVP PATH delivered\n");
        if(conf->ispresent(RC_ADMIN_STATUS_RTAD_FLAGS) == VAL_TRUE)
        {
            unsigned char rtad = *(unsigned char*)conf->read(RC_ADMIN_STATUS_RTAD_FLAGS);
            VERBOSE(7, "admin status present, rtad = 0x%x\n", rtad);
            if((rtad & 0x0F) == 9)
                send_delete_req(pend_pack->conf);
            else
            {
                pend_pack->mmid = local_next_m_id;
                send_create_req(*conf, local_next_m_id);
                local_next_m_id++;
            }
        }
        else
        {
            pend_pack->mmid = local_next_m_id;
            send_create_req(*conf, local_next_m_id);
            local_next_m_id++;
        }
        pend_pack->waiting_confirmation = true;  
        return;
    }
    if(pend_pack->mtype == MTYPE_RESV)
    {
        VERBOSE(3, "mgmt_interface - RSVP RESV delivered\n");
        if(add_path(pend_pack) == true)
            send_create_ack(*conf);
        pend_pack->cid = -1;
        return;
    }

    /* for MTYPE_PATHERR, MTYPE_RESVERR, MTYPE_PATHTEAR, MTYPE_PATHTEAR, MTYPE_RESVTEAR */
    int tunnel_id     = *((unsigned short*)conf->read(RC_TUNNEL_ID));
    //int dest_addr     = *((unsigned int*)  conf->read(RC_SESSION_DEST_ADDR));
    //int src_addr      = *((unsigned int*)  conf->read(RC_EXTENDED_IPV4_ADDR));

    switch(pend_pack->mtype)
    {
    case MTYPE_PATHERR:
    case MTYPE_RESVERR:
        VERBOSE(3, "mgmt_interface - RSVP PATHERR or RSVP RESVERR delivered\n");
        send_create_ack(pend_pack->conf, VAL_TRUE); // "VAL_TRUE" means NACK
        send_delete_req(pend_pack->conf);
        break;
    case MTYPE_PATHTEAR:
    case MTYPE_RESVTEAR:
        VERBOSE(3, "mgmt_interface - RSVP PATHTEAR or RSVP RESVTEAR delivered\n");
        send_delete_req(pend_pack->conf);
        break;
    }

    int path_index = find_path(tunnel_id, 0, 0);// src_addr, dest_addr);
    if (path_index != -1)
        delete_path(path_index);
    pend_pack->cid = -1;
}

//----------------------------------------------------------------------------------------

void mgmt_interface::check_pending_mgnt_packet(int packet_index)
{
    pending_mgnt_packet* pend_mgmt_pack = &pending_mgmt_packets[packet_index];

    if(pend_mgmt_pack->length <= 0)
        return;
    
    VERBOSE(7, "checking pending mgmt packet numero %d\n", packet_index);

    pending_rsvp_packet* rsvp_packet    = NULL;
    rsvp_connection*     new_conn       = NULL;
    int path_index    = -1;
    int tunnel_id     = -1;
    int dest_addr     = -1;
    int src_addr      = -1;
    

    mgmt_packet* pckt = new mgmt_packet((char*)(pend_mgmt_pack->packet), pend_mgmt_pack->length);
    rsvp_configuration  conf            = pckt->get_conf();
    int                 packet_type     = pckt->get_type();
    int                 packet_cmd      = pckt->get_comm();
    int                 packet_id       = pckt->get_id();

    switch(packet_cmd)
    {
    case COMMAND_CREATE:
        switch(packet_type)
        {
        case TYPE_REQ:
           VERBOSE(3, "CREATE REQ received\n");       
           remote_next_m_id = packet_id;
           // create new connection with proper config
           new_conn = new rsvp_connection(dispatcher, conf);
           new_conn->create_path(resolve_router_addr(conf));
           break;
        case TYPE_ACK:
           VERBOSE(3, "CREATE ACK received\n");
           if(rsvp_packet = find_pending_rsvp_packet(&conf, pckt->get_id()))
           {
               // tell dispatcher (?) to deliver this path packet, or somehow create the path
               dispatcher->deliver(rsvp_packet->cid, rsvp_packet->conf, rsvp_packet->mtype);
               add_path(rsvp_packet);
               // delete pending path packet
               rsvp_packet->cid = -1;
           }
           else
           {
               VERBOSE(0, "CREATE ACK received - pending packet not found\n");
           }
           break;
        case TYPE_NACK:
            VERBOSE(3, "CREATE NACK received\n");
            if(rsvp_packet = find_pending_rsvp_packet(&conf, pckt->get_id()))
            {
                // tell dispatcher (?) to deliver resverr (patherr?)
                // packet or somehow indicate failure
                dispatcher->deliver(rsvp_packet->cid, rsvp_packet->conf,
                                    rsvp_packet->mtype, VAL_TRUE);
                // delete pending rsvp packet (path)
                rsvp_packet->cid = -1;
            }
            else
            {
                VERBOSE(0, "CREATE NACK received - pending packet not found\n");
            }
            break;
        }
        break;
    case COMMAND_DELETE:
        switch(packet_type)
        {
        case TYPE_REQ:
            remote_next_m_id = packet_id;
            VERBOSE(3, "DELETE REQ received\n");
            tunnel_id     = *((unsigned short*)conf.read(RC_TUNNEL_ID));
            //dest_addr     = *((unsigned int*)  conf.read(RC_SESSION_DEST_ADDR));
            //src_addr      = *((unsigned int*)  conf.read(RC_EXTENDED_IPV4_ADDR));
            /* tell dispatcher to delete path */
            new_conn = dispatcher->get_connection(tunnel_id);
            if(new_conn != NULL)
            {
                new_conn->destroy_path();
                send_delete_ack(pckt->get_conf());
                path_index = find_path(tunnel_id, 0, 0);// src_addr, dest_addr);
                if (path_index != -1)
                    delete_path(path_index);   // not deleting to see all previous pathes
            }
            else
                send_delete_ack(pckt->get_conf(), true);
            break;
        case TYPE_ACK:
            // practically, do nothing - print some info, or sth
            VERBOSE(3, "DELETE ACK received\n");
            break;
        case TYPE_NACK:
            // impossible! Nevertheless, we can ignore this... :)
            VERBOSE(3, "DELETE NACK received\n");
            break;
        }
        break;
    case COMMAND_STATUS:
        switch(packet_type)
        {
        case TYPE_REQ:
            remote_next_m_id = packet_id;
            VERBOSE(3, "STATUS REQ received\n");
            // we should send status information:
            // for example, we could loop:
            if(conf.ispresent(RC_TUNNEL_ID) == true)
            {
                tunnel_id = *((unsigned short*)conf.read(RC_TUNNEL_ID));
                path_index = find_path(tunnel_id, 0, 0);
                if (path_index != -1)
                {
                    rsvp_configuration* conf = dispatcher->get_connection_configuration(tunnel_id);
                    if(conf != NULL)	
                    {	    
                        conf->setadd(RC_PATH_STATUS, (unsigned int) dispatcher->get_connection_state(tunnel_id));
                        send_status_ack(*conf, -1, -1); 
                        delete conf;
                    }
                    else
                        send_status_nack();
                }
                else
                    send_status_nack();	
            }
            else
            {
                int count = 0;
                for(int i=0; i<paths_count; i++)
                {           
                    // send status about n-th path in paths[]
                    // we have the configuration, so we can construct a full-info packet :D
                    rsvp_configuration* conf = dispatcher->get_connection_configuration(paths[i].tunnel_id);
                    if(conf != NULL)	
                    {	
                        conf->setadd(RC_PATH_STATUS, (unsigned int) dispatcher->get_connection_state(paths[i].tunnel_id));
                        send_status_ack(*conf, i, paths_count); 
                        delete conf;
                        count += 1;
                    }
                    else
                        if (i == 0)
                            send_status_nack();
                }
                if(paths_count == 0)
                    send_status_nack();
            }
            break;
        }
        break;
    }
    delete(pckt);
    pend_mgmt_pack->length = -1;
    free(pend_mgmt_pack->packet);
}

//----------------------------------------------------------------------------------------

#define SOCK_PARAMS     fd_set      set;                        \
                        int         s_errno;                    \
                        int         result;                     \
                        int         bytes_read;                 \
                        char        read_buf [READ_BUF_SIZE];   \
                        struct sockaddr_in  addr;

void mgmt_interface::read_mgnt_packets()
{
    SOCK_PARAMS;
        
    FD_ZERO(&set);
    FD_SET(socket_descriptor, &set);
    if((result = select(socket_descriptor+1, &set, NULL, NULL, NULL)) == -1)
    {
        s_errno = errno;
        VERBOSE(0,"select returned error (-1) and set errno to %d\n", s_errno);
    }
    else
    {
        if(result <= 0)
            return;
        
        s_errno = 0;
        while(s_errno < MAX_PENDING_MGMT_PACKETS && pending_mgmt_packets[s_errno].length != -1)
            s_errno++;
        if(s_errno != MAX_PENDING_MGMT_PACKETS)
        {
            // save packet information into pending_mgmt_packets
            // && check if connection wasn't closed
            bytes_read = read(socket_descriptor, read_buf, READ_BUF_SIZE);
            int err2 = errno;
            if(bytes_read > 0)
            {
                pending_mgmt_packets[s_errno].packet = malloc(bytes_read);
                memcpy(pending_mgmt_packets[s_errno].packet, read_buf,bytes_read);
                pending_mgmt_packets[s_errno].length = bytes_read;
                touch();
            }
            if(bytes_read == 0)
            {
                close(socket_descriptor);
                socket_descriptor = -1;
                socket_descriptor_listen = -(socket_descriptor_listen);
                state = STATE_LISTEN;
            }
            if(bytes_read < 0)
            {
                VERBOSE(0, "read returned value %d, errno is %d\n", bytes_read, err2);
                if(globals.uni_proxy_server == false)
                {
                    // damian 18.01.2007: because of looping when mngt connection breaks
                    socket_descriptor = -1;						
                    socket_descriptor_listen = -(socket_descriptor_listen);
                    state = STATE_LISTEN;
                    sleep(1);
                connect_to_mngt_server();
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------

void mgmt_interface::accepting_connection()
{
    SOCK_PARAMS;
    
    FD_ZERO(&set);
    FD_SET(socket_descriptor_listen, &set);
    if((result = select(socket_descriptor_listen+1, &set, NULL, NULL, NULL)) == -1)
    {
        s_errno = errno;
        VERBOSE(0,"select returned error (-1) and set errno to %d\n", s_errno);
    }
    else
    {
        if(result <= 0)
            return;

	VERBOSE(3,"Incoming mngt connetion accepted\n");
        socket_descriptor = accept(socket_descriptor_listen,
                            (struct sockaddr*)&addr, (socklen_t*)&s_errno);
        socket_descriptor_listen = -(socket_descriptor_listen);
        state = STATE_CONNECTED;
    }
}

//----------------------------------------------------------------------------------------

pending_rsvp_packet* mgmt_interface::find_pending_rsvp_packet(rsvp_configuration* pack_conf, int m_id)
{
    int tunnel_id = *((unsigned short*)(pack_conf->read(RC_TUNNEL_ID)));
    pending_rsvp_packet* rsvp_packet = NULL;
    
    for(int i=0; i<MAX_PENDING_RSVP_PACKETS; i++)
    {
        rsvp_packet = &pending_rsvp_packets[i];
        if(rsvp_packet->mmid == -1)
            return NULL;
        
        VERBOSE(7, "find_pending_rsvp_packet: m_id=%d, rsvp_packet->mmid=%d\n", m_id, rsvp_packet->mmid);
        VERBOSE(7, "find_pending_rsvp_packet: tunnel_id=%d, rsvp_packet->cid=%d\n", tunnel_id, rsvp_packet->cid);
        //if(rsvp_packet->cid == tunnel_id && rsvp_packet->mmid-1 ==  m_id)
        if(rsvp_packet->cid == tunnel_id && rsvp_packet->mmid ==  m_id)
            return rsvp_packet;
    }
    return NULL;
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_create_req(rsvp_configuration conf, int mid)
{
    VERBOSE(3, "send CREATE REQUEST\n");
    return send_mgnt_packet(&conf, mid, TYPE_REQ, COMMAND_CREATE);
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_create_ack(rsvp_configuration conf, int nack)
{
    rsvp_configuration  pckt_conf;
    pckt_conf.copy_from(RC_SESSION_DEST_ADDR,   conf);
    pckt_conf.copy_from(RC_TUNNEL_ID,           conf);
    pckt_conf.copy_from(RC_EXTENDED_IPV4_ADDR,  conf);
    if(nack == VAL_TRUE)
    {
        pckt_conf.copy_from(RC_ERROR_NODE_ADDR, conf);
        pckt_conf.copy_from(RC_ERROR_FLAGS,     conf);
        pckt_conf.copy_from(RC_ERROR_CODE,      conf);
        pckt_conf.copy_from(RC_ERROR_VALUE,     conf);
        VERBOSE(3, "send CREATE NACK\n");
    }
    else
    {
        VERBOSE(3, "send CREATE ACK\n");
    }

    return send_mgnt_packet(&pckt_conf, remote_next_m_id, (nack==0)?TYPE_ACK:TYPE_NACK, COMMAND_CREATE);
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_delete_req(rsvp_configuration conf)
{
    rsvp_configuration  pckt_conf;

    pckt_conf.copy_from(RC_SESSION_DEST_ADDR,   conf);
    pckt_conf.copy_from(RC_TUNNEL_ID,           conf);
    pckt_conf.copy_from(RC_EXTENDED_IPV4_ADDR,  conf);

    VERBOSE(3, "send DELETE REQUEST\n");
    return send_mgnt_packet(&pckt_conf, local_next_m_id, TYPE_REQ, COMMAND_DELETE);
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_delete_ack(rsvp_configuration conf, int nack)
{
    rsvp_configuration  pckt_conf;
    pckt_conf.copy_from(RC_SESSION_DEST_ADDR,   conf);
    pckt_conf.copy_from(RC_TUNNEL_ID,           conf);
    pckt_conf.copy_from(RC_EXTENDED_IPV4_ADDR,  conf);
    
    if(nack == VAL_TRUE)
    {
        // add some objects, that will indicate an error
        // - question is, should we do this using standard RSVP objects
        // or rather defining our own error objects?
        VERBOSE(3, "send DELETE NACK\n");
    }
    else
        VERBOSE(3, "send DELETE ACK\n");

    return send_mgnt_packet(&pckt_conf, remote_next_m_id, (nack==0)?TYPE_ACK:TYPE_NACK, COMMAND_DELETE);
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_status_ack(rsvp_configuration conf, int num, int total)
{
    VERBOSE(3, "send STATUS ACK\n");
    return send_mgnt_packet(&conf, remote_next_m_id, TYPE_ACK, COMMAND_STATUS, num, total);
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_status_nack()
{
    VERBOSE(3, "send STATUS NACK\n");
    if(socket_descriptor < 0)
        return ERROR_SOCK_NOT_FUNC;
        
    int          data_size      = 0;
    mgmt_packet* pckt           = new mgmt_packet(remote_next_m_id,TYPE_NACK, COMMAND_STATUS);
    void*        data           = pckt->get_data(&data_size);
    int          bytes_written  = write(socket_descriptor, data, data_size);
    
    if(bytes_written != data_size)
        return ERROR_PACKET_SEND;
    return 0; 
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_transit_req() 
{
// let's leave it until rsvp will be able to pass rsvp packets through our linux router
}

//----------------------------------------------------------------------------------------

int mgmt_interface::send_mgnt_packet(rsvp_configuration* conf, int mid, int type
                    , int cmd, int multi_num, int multi_count)
{
    if(socket_descriptor < 0)
        return ERROR_SOCK_NOT_FUNC;
        
    int          data_size      = 0;
    mgmt_packet* pckt           = new mgmt_packet(conf, mid,
                                    type, cmd,
                                    multi_num, multi_count);
    void*        data           = pckt->get_data(&data_size);
    int          bytes_written  = write(socket_descriptor, data, data_size);
    
    if(bytes_written != data_size)
        return ERROR_PACKET_SEND;
    return 0; 
}

//----------------------------------------------------------------------------------------

int mgmt_interface::ask_gui(int conn_id, rsvp_configuration config, int msg_type)
{
    int t = 0;
    if(!(state == STATE_CONNECTED || state == STATE_CONNECT))
        return ERROR_INT_NOT_US;
    /* save conf to pending_rsvp_packets */
    while(t < MAX_PENDING_RSVP_PACKETS && pending_rsvp_packets[t].cid != -1)
        t++;
    if(t == MAX_PENDING_RSVP_PACKETS)
        return ERROR_BUF_2_SMALL;
    pending_rsvp_packets[t].cid     = conn_id;
    pending_rsvp_packets[t].conf    = config;
    pending_rsvp_packets[t].mtype   = msg_type;
    pending_rsvp_packets[t].waiting_confirmation = false;
    // wake up main mgmt thread
    touch();
    return 0;
}

//----------------------------------------------------------------------------------------

bool mgmt_interface::add_path(pending_rsvp_packet* pend_pack)
{
    rsvp_configuration*  conf       = &pend_pack->conf;
    
    if(paths_count >= MAX_MGMT_PATHS)
    	return false;

    path_elem* path = &paths[paths_count];
    path->tunnel_id    = *((unsigned short*)conf->read(RC_TUNNEL_ID));
    path->dest_addr    = *((unsigned int*)  conf->read(RC_SESSION_DEST_ADDR));
    path->src_addr     = *((unsigned int*)  conf->read(RC_EXTENDED_IPV4_ADDR));
    path->conf         = pend_pack->conf;
    path->conn_id      = pend_pack->cid;
    
    if(paths_count == 0)
    {
    	/* if the last path is equal to a new there is no need to save a new one */
    	path_elem* path_prev    = &paths[paths_count-1];
	if(path->tunnel_id == path_prev->tunnel_id &&
           path->tunnel_id == path_prev->tunnel_id &&
	   path->src_addr  == path_prev->src_addr  &&
	   path->conn_id   == path_prev->conn_id)
	   {
	   	return false;
	   }
    }

    paths_count++;
    return true;
}

//----------------------------------------------------------------------------------------


void mgmt_interface::delete_path(int number)
{
    if(paths_count <= 0)
        return;
    for(int t=number;t<paths_count-1;t++)
    {
        paths[t].tunnel_id      = paths[t+1].tunnel_id;
        paths[t].src_addr       = paths[t+1].src_addr;
        paths[t].dest_addr      = paths[t+1].dest_addr;
        paths[t].conf           = paths[t+1].conf;
        paths[t].conn_id        = paths[t+1].conn_id;
    }
    paths_count--;
}

//----------------------------------------------------------------------------------------

int mgmt_interface::find_path(int tunnel_id, int src_addr, int dest_addr)
{ 
    path_elem* node;
    for(int i=0; i<paths_count; i++)
    {
        node = &paths[i];
        if(tunnel_id == node->tunnel_id)// && src_addr == node->src_addr && dest_addr == node->dest_addr)
            return i;
    }
    return -1;
}

//----------------------------------------------------------------------------------------

#endif
