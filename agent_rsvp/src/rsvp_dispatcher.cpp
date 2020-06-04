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
 
#ifndef RSVP_DISPATCHER_BODY_FILE
#define RSVP_DISPATCHER_BODY_FILE

#include "rsvp_dispatcher.h"

extern rsvp_globals globals;
extern rsvp_configuration global_newconn_configuration;

rsvp_dispatcher::rsvp_dispatcher(const char* local_addr)
{
    init(inet_addr(local_addr));
}

rsvp_dispatcher::rsvp_dispatcher(unsigned int local_addr)
{
    init(local_addr);
}

rsvp_dispatcher::rsvp_dispatcher(const char* local_addr, ip_mask ipm)
{
    ipm_allowed_addrs = ipm;
    init(inet_addr(local_addr));
}

rsvp_dispatcher::rsvp_dispatcher(unsigned int local_addr, ip_mask ipm)
{
    ipm_allowed_addrs = ipm;
    init(local_addr);
}

void rsvp_dispatcher::init(unsigned int local_addr)
{

    int t;
    socket = new rsvp_socket(local_addr, ipm_allowed_addrs);
    for(t=0;t<MAX_REGISTERED_CONNS;t++)
    {
        registered_conns[t].conn = NULL;
        registered_conns[t].id = 0;
        registered_conns[t].wake_interval = 0;
        registered_conns[t].wake_counter_sec = 0;
        registered_conns[t].wake_counter_usec = 0;
    }
    for(t=0;t<MAX_REGISTERED_INTERFACES;t++)
        registered_interfaces[t].interface = NULL;
    router_addrs_count = 0;
    registered_conns_count = 0;
    registered_conns_access.set_value(1);
    registered_interfaces_count = 0;
    pthread_create(&tid, NULL, receiver_thread, this);
    name_thread("rsvp_dispatcher::receiver_thread", tid);
}

rsvp_dispatcher::~rsvp_dispatcher()
{
    socket->close_connection();
    free(socket);
}

void* rsvp_dispatcher::receiver_thread(void* arg)
{
    struct timeval tv;
    int retv;
    rsvp_dispatcher* o;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    o = (rsvp_dispatcher*)arg;
    VERBOSE(4, "receiver thread starts\n");
    while(1)
    {
        retv = o->socket->wait_for_packet(&tv);
        VERBOSE(7, "wait_for_packet returned to receiver_thread\n");
        if(retv == ERROR_SELECT)
            return NULL;
        if(retv == ERROR_TIMEOUT)
        {
            VERBOSE(9, "dispatcher thread should send some hellos, but won't :)\n");
            if(o->send_hellos() != 0)
            {
                VERBOSE(0, "error while sending hello packets\n");
            }
            tv.tv_sec = 30; // we assume, that all refresh periods are 30 sec, however this might not be true with all connections - we ignore it for now and we send refresh messages every 30 sec.
            tv.tv_usec = 0;
        }
        if(retv > 0 || retv == ERROR_RECV_BUF_FULL)
        {
            VERBOSE(9, "so go with a packet :)\n");
            int t;
            void* buffer;

            buffer = NULL;
            t = 0;
            while(t != ERROR_RECV_BUF_EMPTY)
            {
                free(buffer);
                buffer = malloc(MAX_RECV_BUFFER_SIZE);
                t = o->socket->get_packet(buffer, MAX_RECV_BUFFER_SIZE);
                if(t == ERROR_BUF_2_SMALL)
                {
                    VERBOSE(0, "Buffer is too small!\n");
                }
                if(t > 0)
                {
                    rsvp_packet packet;
                    rsvp_configuration* packet_conf;
                    struct IPHeader iphdr;
                    unsigned short tunnel_id;
                    int x, a, mtype;

                    mtype = packet.parse_headers(&buffer, &t, &iphdr);
                    if(mtype < 0)
                        continue;
                    VERBOSE(7, "mtype got %d\n", mtype);
                    packet_conf = packet.parse_data(buffer, t);

                    // let's respond to a hello message
                    if(mtype == MTYPE_HELLO)
                    {
                        VERBOSE(5, "receiving RSVP HELLO\n");

                        unsigned char req_ack = *((unsigned char*)packet_conf->read(RC_HELLO_REQ_ACK));
                        if(req_ack == 1)
                        {
                            rsvp_configuration hello_conf;
                            hello_conf.add(RC_HELLO_REQ_ACK, (unsigned char)2);
                            hello_conf.add(RC_HELLO_SRC_INSTANCE, globals.hello_instance);
                            unsigned int* other_inst = (unsigned int*)packet_conf->read(RC_HELLO_SRC_INSTANCE);
                            hello_conf.add(RC_HELLO_DST_INSTANCE, *other_inst);
                            hello_conf.add(RC_RESTART_TIME, (unsigned int)0);
                            hello_conf.add(RC_RECOVERY_TIME, (unsigned int)0);\
                            
                            rsvp_packet hello_packet;
                            hello_packet.set_configuration(&hello_conf);
                            hello_packet.add_object(CLASS_HELLO_ACK);
                            hello_packet.add_object(CLASS_RESTART_CAP);
                            
                            char* hello_data[1];
                            hello_data[0] = (char*)malloc(8192);
                            unsigned int hello_data_size = hello_packet.get_data(hello_data[0], 8192);
                            hello_data_size = hello_packet.make_rsvp_packet((void**)hello_data, hello_data_size, MTYPE_HELLO);

                            VERBOSE(5, "send RSVP HELLO\n");  
                            o->send(hello_data[0], hello_data_size, inet_addr(globals.rsvp_router_addr));   
                        }
                        continue;
                    }
                    else if(mtype == MTYPE_SREFRESH)
                    {
                        VERBOSE(5, "receiving RSVP SREFRESH\n");                        
                        
                        unsigned char req_ack = *((unsigned char*)packet_conf->read(RC_HELLO_REQ_ACK));
                        if(req_ack == 1)
                        {
                            packet_conf->setadd(RC_MSG_ID_FLAGS, (unsigned char)0);
                            
                            if(packet_conf->ispresent(RC_SREFRESH_MESSAGE_ID) == true)
                            {
                                // TODO: check all srefresh message id list value
                            }
                            
                            rsvp_packet ack_packet;
                            ack_packet.set_configuration(packet_conf);
                            ack_packet.add_object(CLASS_MESSAGE_ID_ACK);
                                                      
                            char* ack_data[1];
                            ack_data[0] = (char*)malloc(8192);
                            unsigned int ack_data_size = ack_packet.get_data(ack_data[0], 8192);
                            ack_data_size = ack_packet.make_rsvp_packet((void**)ack_data, ack_data_size, MTYPE_ACK);              

                            VERBOSE(5, "send RSVP ACK\n");  
                            o->send(ack_data[0], ack_data_size, inet_addr(globals.rsvp_router_addr));   
                            
                        }
                    }
                    else if(mtype < 12) //@todo hot-fix! Repair in some nice way!
                    {
                        VERBOSE(7, "trying to read TUNNEL_ID\n");
                        tunnel_id = *(unsigned short*)packet_conf->read(RC_TUNNEL_ID);
                        a = -1;
                        VERBOSE(7, "waiting on registered_conns_access semaphore\n");
                        o->registered_conns_access.wait();
                        VERBOSE(7, "semaphore passed\n");

                        for(x=0;x<o->registered_conns_count;x++)
                            if(tunnel_id == o->registered_conns[x].id)
                                a = x;
                        if(a >= 0)
                        {
                            o->registered_conns[a].conn->get_packet(mtype, buffer, t);
                            o->registered_conns_access.signal_plus();
                        }
                        else if(mtype == MTYPE_PATH)
                        {
                            rsvp_connection* con;
                            o->registered_conns_access.signal_plus();
                            VERBOSE(5, "spawning a new rsvp_connection object\n");
                            con = new rsvp_connection(o, tunnel_id, global_newconn_configuration);
                            VERBOSE(7, "monster spawned - beware! ;)\n");
                            con->get_packet(mtype, buffer, t);
                        }
                        else
                        {
                            o->registered_conns_access.signal_plus();
                            VERBOSE(3, "incoming RSVP message was not served\n");
                        }
                    }
                }
            }
        }
        if(retv == 0)
            return NULL;
    }
    return NULL;
}

int rsvp_dispatcher::add_router(unsigned int addr)
{
    if(router_addrs_count == MAX_ROUTER_ADDRS)
        return ERROR_MAX_ELEMENTS;
    router_addrs[router_addrs_count] = addr;
    router_addrs_count++;
    return 0;
}

int rsvp_dispatcher::send(void* buf, int buf_size, unsigned int addr)
{
    return socket->send(addr, buf, buf_size);
}

int rsvp_dispatcher::register_connection(rsvp_connection* conn, unsigned short conn_id, int wk_int)
{
    int t;
    registered_conns_access.wait();
    for(t=0;t<registered_conns_count;t++)
        if(registered_conns[t].id == conn_id)
        {
            registered_conns_access.signal_plus();
            return ERROR_VAL_PRES;
        }
    registered_conns[registered_conns_count].conn = conn;
    registered_conns[registered_conns_count].id = conn_id;
    registered_conns[registered_conns_count].wake_interval = wk_int;
    registered_conns_count++;
    registered_conns_access.signal_plus();
    return 0;
}

int rsvp_dispatcher::unregister_connection(int pos)
{
    if(pos >= registered_conns_count)
        return -1;
    
    for(int t=pos;t<registered_conns_count-1;t++)
    {
        registered_conns[t].conn                = registered_conns[t+1].conn;
        registered_conns[t].id                  = registered_conns[t+1].id;
        registered_conns[t].wake_interval       = registered_conns[t+1].wake_interval;
        registered_conns[t].wake_counter_sec    = registered_conns[t+1].wake_counter_sec;
        registered_conns[t].wake_counter_usec   = registered_conns[t+1].wake_counter_usec;
    }
    if(registered_conns_count > 0)
        registered_conns_count--;
    return 0;
}

void rsvp_dispatcher::report_paths()
{
    int t;
    unsigned short rcid;
    unsigned int rclabel;
    int rcpstate;
    registered_conns_access.wait();
    VERBOSE(1, " +-----------------------------------------------------+\n");
    VERBOSE(1, " | Number of registered connections: %2d                |\n", registered_conns_count);
    VERBOSE(1, " +-----------------------------------------------------+\n");
    if(registered_conns_count == 0)
    {
        registered_conns_access.signal_plus();
        return;
    }
    VERBOSE(1, " | ID    |   LABEL    |                     PATH STATE |\n");
   
    for(t=0;t<registered_conns_count;t++)
    {
        VERBOSE(1, " | %5d | 0x%08x | %30s |\n", registered_conns[t].id, registered_conns[t].conn->get_label(), registered_conns[t].conn->get_path_state(NULL));
    }
    VERBOSE(1, " +-----------------------------------------------------+\n");
    registered_conns_access.signal_plus();
}

int rsvp_dispatcher::register_interface(generic_interface* interf)
{
    if(registered_interfaces_count == MAX_REGISTERED_INTERFACES)
        return ERROR_MAX_ELEMENTS;
    registered_interfaces[registered_interfaces_count].interface = interf;
    registered_interfaces_count++;
    return 0;
}

int rsvp_dispatcher::unregister_interface(generic_interface* interf)
{
    int t, x;
    x = -1;

    for(t=0;t<registered_interfaces_count-1;t++)
        if(registered_interfaces[t].interface == interf)
        {
            x = t;
            break;
        }
    if(x == -1)
        return ERROR_VAL_NOT_PRES;
    for(t=x;t<registered_interfaces_count-1;t++)
    {
        registered_interfaces[t].interface = registered_interfaces[t+1].interface;
    }
    if(registered_interfaces_count > 0)
        registered_interfaces_count--;
    return 0;
}

int rsvp_dispatcher::ask_interface(unsigned short cid, rsvp_configuration conf, int mtype)
{
    if(registered_interfaces_count == 0)
        return ERROR_NO_INT;
    return registered_interfaces[0].interface->ask_gui(cid, conf, mtype);
}

int rsvp_dispatcher::deliver(unsigned short cid, rsvp_configuration conf, int mtype, int nack) //@todo nack nieuzywany - poprawic dla wiadomosci MGMT NACK
{
    int t;
    VERBOSE(5, "waiting fot access (rsvp_dispatcher::deliver)\n");
    registered_conns_access.wait();
    VERBOSE(5, "access granted (rsvp_dispatcher::deliver)\n");
    for(t=0;t<registered_conns_count;t++)
    {
        if(registered_conns[t].id == cid)
        {
            registered_conns[t].conn->get_packet(mtype, NULL, 0, &conf, nack);
            registered_conns_access.signal_plus();
            return 0;
        }
    }
    registered_conns_access.signal_plus();
    return ERROR_NO_CONN;
}

rsvp_connection* rsvp_dispatcher::get_connection(unsigned short cid)
{
    int t;
    registered_conns_access.wait();
    for(t=0;t<registered_conns_count;t++)
        if(registered_conns[t].id == cid)
        {
            registered_conns_access.signal_plus();
            return registered_conns[t].conn;
        }
    registered_conns_access.signal_plus();
    return NULL;
}


int rsvp_dispatcher::get_connection_state(unsigned short cid)
{
    int t;
    registered_conns_access.wait();
    for(t=0;t<registered_conns_count;t++)
        if(registered_conns[t].id == cid)
        {
            registered_conns_access.signal_plus();
            return registered_conns[t].conn->get_path_state();
        }
    registered_conns_access.signal_plus();
    return -1;
}

rsvp_configuration* rsvp_dispatcher::get_connection_configuration(unsigned short cid)
{
    rsvp_configuration* rsvp_conf = new rsvp_configuration();
    int t;
    registered_conns_access.wait();
    for(t=0;t<registered_conns_count;t++)
        if(registered_conns[t].id == cid)
        {
            registered_conns_access.signal_plus();
            *rsvp_conf = registered_conns[t].conn->path_configuration;
            return rsvp_conf;
            //return registered_conns[t].conn->path_configuration;
        }
    registered_conns_access.signal_plus();
    return NULL;
}

int rsvp_dispatcher::send_hellos()
{
    // @todo fill this method with code (when we will want to send hello packets by ourselves)
    return 0;
}

#endif
