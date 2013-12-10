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
 
#ifndef RSVP_CONNECTION_BODY_FILE
#define RSVP_CONNECTION_BODY_FILE

#include "rsvp_connection.h"

extern rsvp_globals globals;

//------------------------------------------------------------------------------

void rsvp_connection::adding_resv_objects(rsvp_packet* resv)
{   
    unsigned char switching_type =  *(unsigned char*)path_configuration.read(RC_LSP_SWITCHING_TYPE);

    if(globals.protocol_vers == IETF_RSVP)
    {
        resv->add_object(CLASS_SESSION_IETF);
        
        /* different switching types require different HOP object versions */
        if(switching_type >= 100)
            resv->add_object(CLASS_IPV4_IF_ID_RSVP_HOP); /* no-packet switching */
        else
            resv->add_object(CLASS_IPV4_RSVP_HOP); /* packet switching */
        
        resv->add_object(CLASS_TIME_VALUES);
        resv->add_object(CLASS_STYLE);
        resv->add_object(CLASS_TOKEN_FLOWSPEC);
        resv->add_object(CLASS_FILTER_SPEC);
        resv->add_object(CLASS_GENERALIZED_LABEL);
    }
    else
    {
        resv->add_object(CLASS_MESSAGE_ID);
        resv->add_object(CLASS_SESSION_OIF);
        resv->add_object(CLASS_IPV4_IF_ID_RSVP_HOP);
        resv->add_object(CLASS_TIME_VALUES);
        resv->add_object(CLASS_STYLE);
        resv->add_object(CLASS_SONETSDH_FLOWSPEC);
        resv->add_object(CLASS_FILTER_SPEC);
        resv->add_object(CLASS_GENERALIZED_LABEL);
        resv->add_object(CLASS_RESV_CONFIRM);
        resv->add_object(CLASS_TOKEN_FLOWSPEC);

        if(globals.protocol_vers == OIFUNI20)
        {
            if(resv->add_object(CLASS_CALL_ID_2) < 0)
                if(resv->add_object(CLASS_CALL_ID_1) < 0)
                    resv->add_object(CLASS_CALL_ID_0);
        }
    }
}

//============================================================================

rsvp_connection::rsvp_connection(rsvp_dispatcher* disp, unsigned short conn_id, rsvp_configuration conf)
{
    init(disp, conf);
    connection_id   = conn_id;
    
    configuration.setadd(RC_TUNNEL_ID, (unsigned short)conn_id);
    VERBOSE(6, "tunnel ID set\n");

    create_refresh_thread();
}

//------------------------------------------------------------------------------

rsvp_connection::rsvp_connection(rsvp_dispatcher* disp, rsvp_configuration conf)
{
    init(disp, conf);

    VERBOSE(7, "trying to read tunnel id...\n");
    connection_id = *((unsigned short*)configuration.read(RC_TUNNEL_ID));
    VERBOSE(7, "tunnel id read\n");

    create_refresh_thread();
}

//------------------------------------------------------------------------------

void rsvp_connection::init(rsvp_dispatcher* disp, rsvp_configuration conf)
{
    VERBOSE(7, "rsvp_connection constructor\n");

    dispatcher      = disp;
    configuration   = conf;
    path_state      = PATH_STATE_NULL;
    path_direction  = DIRECTION_UNDEFINED;
    label           = 0;
    key             = 0;
    router_address  = resolve_router_addr(configuration);


    set_conf_defaults();
    configuration.add_missing(conf_defaults);

    if(configuration.ispresent(RC_REFRESH_PERIOD) == VAL_TRUE)
        refresh_interval = *((unsigned int*)configuration.read(RC_REFRESH_PERIOD)) / 1000;
    else
        refresh_interval = 30000;
    VERBOSE(6, "refresh interval set\n");

    access.set_value(1);
}

//------------------------------------------------------------------------------

void rsvp_connection::create_refresh_thread()
{
    dispatcher->register_connection(this, connection_id);
    VERBOSE(4, "new connection registered\n");

    refresh_tread_state = 1;
    pthread_create(&tid, NULL, (void*(*)(void*))(refresh_path), this);
    name_thread("rsvp_connection::refresh_path", tid);
    VERBOSE(6, "connection thread spawned\n");
}

//------------------------------------------------------------------------------

void rsvp_connection::set_conf_defaults()
{
    unsigned int ulabel_buf;
    
    if (globals.protocol_vers == OIFUNI10)
        ulabel_buf = htonl(65536);
    else
        ulabel_buf = 0xffffffff;
    
    if(conf_defaults.ispresent(RC_TOKEN_BUCKET_SIZE) == false)
    {
        conf_defaults.setadd(RC_TOKEN_BUCKET_RATE,     (float)31250000); // 250Mbps
        conf_defaults.setadd(RC_TOKEN_BUCKET_SIZE,     (float)31250000);
        conf_defaults.setadd(RC_PEAK_DATA_RATE,        (float)31250000);
    }
    else
    {
        unsigned int bucket_size =  *(unsigned int*)conf_defaults.read(RC_TOKEN_BUCKET_SIZE);
        conf_defaults.setadd(RC_PEAK_DATA_RATE,        (float)bucket_size);
        conf_defaults.setadd(RC_TOKEN_BUCKET_SIZE,     (float)bucket_size);
    }
    
    if(globals.protocol_vers != IETF_RSVP)
    {
        conf_defaults.setadd(RC_LSP_ENC_TYPE,          (unsigned char)2);// ethernet v2/DIX // to remove !!
        conf_defaults.setadd(RC_LSP_SWITCHING_TYPE,    (unsigned char)0x33);// l2sc // to remove !!
        conf_defaults.setadd(RC_LSP_GPID,              (unsigned short)0x22);//sdh (lambda,fibre)
        conf_defaults.setadd(RC_UPSTREAM_LABEL,        (void*)&ulabel_buf, sizeof(ulabel_buf));
    }
    else
    {
        conf_defaults.setadd(RC_LSP_ENC_TYPE,          (unsigned char)1);// packet // to remove !!
        conf_defaults.setadd(RC_LSP_SWITCHING_TYPE,    (unsigned char)1);// psc-1 // to remove !!
        conf_defaults.setadd(RC_LSP_GPID,              (unsigned short)2048);// ip
    }

    conf_defaults.setadd(RC_EXTENDED_IPV4_ADDR,    inet_addr(globals.rsvp_local_if_addr));
    conf_defaults.setadd(RC_DATA_OUT_IF_ADDR,      inet_addr(globals.rsvp_local_if_addr));  
    conf_defaults.setadd(RC_DATA_IN_IF_ADDR,       inet_addr(globals.rsvp_local_if_addr));
    conf_defaults.setadd(RC_CONTROL_OUT_IF_ADDR,   inet_addr(globals.rsvp_local_if_addr));
    conf_defaults.setadd(RC_SENDER_SRC_ADDR,       inet_addr(globals.rsvp_local_if_addr));
      
    conf_defaults.setadd(RC_DATA_OUT_IF_HANDLE,    (unsigned int) 1000);
    conf_defaults.setadd(RC_DATA_IN_IF_HANDLE,     (unsigned int) 1000);
    conf_defaults.setadd(RC_CONTROL_OUT_IF_HANDLE, (unsigned int) 1000);
    
    conf_defaults.setadd(RC_SENDER_SRC_ID,         (unsigned short)1);
    
    conf_defaults.setadd(RC_MSG_ID_FLAGS,          (unsigned char)1);
    conf_defaults.setadd(RC_MSG_ID_EPOCH,          (unsigned int)globals.epoch);
    conf_defaults.setadd(RC_MSG_ID_ID,             (unsigned int)globals.get_message_id());
    conf_defaults.setadd(RC_SESSION_NAME,          (char*)"default_path");
    conf_defaults.setadd(RC_SETUP_PRIORITY,        (unsigned char)0);
    conf_defaults.setadd(RC_HOLDING_PRIORITY,      (unsigned char)0);
    conf_defaults.setadd(RC_LOCAL_PROTECTION_FLAG, (unsigned char)1);
    conf_defaults.setadd(RC_LABEL_RECORDING_FLAG,  (unsigned char)0);
    conf_defaults.setadd(RC_SE_STYLE_FLAG,         (unsigned char)1);
    
    
    /* DEPRECATED IN FINAL UNI2.0Eth
    conf_defaults.setadd(RC_CALL_ID_ADDR_TYPE,     (unsigned char)1);
    conf_defaults.setadd(RC_CALL_ID_SRC_TN_ADDR,   inet_addr("10.0.0.1"));
    conf_defaults.setadd(RC_CALL_ID_LOCAL_ID1,     (unsigned int)0x0a0b0c0d);
    conf_defaults.setadd(RC_CALL_ID_LOCAL_ID2,     (unsigned int)0x0e0f0001);
    */
    
    conf_defaults.setadd(RC_MINIMUM_POLICED_UNIT,  (unsigned int)64);
    conf_defaults.setadd(RC_MAXIMUM_PACKET_SIZE,   (unsigned int)1518);
     
    conf_defaults.setadd(RC_TNA_SRC_ADDR,          inet_addr("10.0.0.1"));
    conf_defaults.setadd(RC_TNA_DST_ADDR,          inet_addr("10.0.0.2"));
    //conf_defaults.setadd(RC_SERVICE_LEVEL,           (unsigned char)0);
    conf_defaults.setadd(RC_ADMIN_STATUS_RTAD_FLAGS, (unsigned int)0);
    
    conf_defaults.setadd(RC_TR_D_ADDR,             inet_addr("127.0.0.1"));
    
    conf_defaults.setadd(RC_REFRESH_PERIOD,        (unsigned int)30000);
    
    conf_defaults.setadd(RC_ETHERNET_CIR,          (float)31250000);
    conf_defaults.setadd(RC_ETHERNET_CBS,          (float)0);
    conf_defaults.setadd(RC_ETHERNET_EIR,          (float)0);
    conf_defaults.setadd(RC_ETHERNET_EBS,          (float)0);
    conf_defaults.setadd(RC_ETHERNET_SW_GRANUALITY,(unsigned short)1);
    conf_defaults.setadd(RC_ETHERNET_INDEX,        (unsigned char)0);
    conf_defaults.setadd(RC_ETHERNET_PROFILE,      (unsigned char)0);
       
}

//------------------------------------------------------------------------------

rsvp_connection::~rsvp_connection()
{
}

//------------------------------------------------------------------------------

void rsvp_connection::PathErr_RD_flags_response(rsvp_configuration* packet_conf)
{
    VERBOSE(5, "deleting path because of Admin_status RD flags - responding with PathErr\n");
    
    packet_conf->setadd(RC_MSG_ID_FLAGS,    (unsigned char) 1);
    packet_conf->setadd(RC_MSG_ID_EPOCH,    (unsigned int)  globals.epoch);
    packet_conf->setadd(RC_ERROR_NODE_ADDR, inet_addr(globals.rsvp_local_if_addr));
    packet_conf->setadd(RC_ERROR_FLAGS,     (unsigned char) 4);
    packet_conf->setadd(RC_ERROR_CODE,      (unsigned char) 0);  // confirmation
    packet_conf->setadd(RC_ERROR_VALUE,     (unsigned short)0);
    packet_conf->setadd(RC_MSG_ID_ID,       (unsigned int)  globals.get_message_id());
    
    
    packet_conf->setadd(RC_FILTER_SRC_ADDR, *((unsigned int*)packet_conf->read(RC_SENDER_SRC_ADDR)));
    packet_conf->setadd(RC_FILTER_SRC_PORT, *((unsigned short*)packet_conf->read(RC_SENDER_SRC_ID)));
    packet_conf->setadd(RC_WILDCARD_FILTER, (unsigned char)0);
    packet_conf->setadd(RC_FIXED_FILTER,    (unsigned char)1);
    packet_conf->setadd(RC_SHARED_EXPLICIT, (unsigned char)0);
    
    unsigned int label_buf = htonl(label);
    packet_conf->setadd(RC_LABEL, &label_buf, sizeof(label_buf));

    if (globals.protocol_vers == IETF_RSVP)
    {
        rsvp_packet  resv;
        resv.set_configuration(packet_conf);
        
        packet_conf->setadd(RC_ADMIN_STATUS_RTAD_FLAGS,  (unsigned char)0x01); // D flags
        
        resv.set_configuration(packet_conf);
        adding_resv_objects(&resv);
        resv.add_object(CLASS_ADMIN_STATUS);
        
        unsigned int dest_addr = inet_addr(globals.rsvp_router_addr);
    
        VERBOSE(3, "send RSVP RESV with D flag to %s\n", int_2_addr(dest_addr));
        send_rsvp_message(&resv, MTYPE_RESV, dest_addr);
        return;
    }
    else
    {
        rsvp_packet  patherr;
        patherr.set_configuration(packet_conf);
    
        patherr.add_object(CLASS_MESSAGE_ID);
        patherr.add_object(CLASS_SESSION_OIF);
        patherr.add_object(CLASS_ERROR_SPEC);
        patherr.add_object(CLASS_SENDER_TEMPLATE);

        if (globals.protocol_vers == OIFUNI20)
        {
            //patherr.add_object(CLASS_TOKEN_SENDER_TSPEC); //UNI2.0Eth obsolete
            patherr.add_object(CLASS_ETHERNET_SENDER_TSPEC);
            if(patherr.add_object(CLASS_CALL_ID_2) < 0)
                if(patherr.add_object(CLASS_CALL_ID_1) < 0)
                    patherr.add_object(CLASS_CALL_ID_0);
        }
   
       
        dispatcher->ask_interface(connection_id, *packet_conf, MTYPE_PATHTEAR);
    
        unsigned int dest_addr = inet_addr(globals.rsvp_router_addr); //*(unsigned int*)packet_conf->read(RC_CONTROL_OUT_IF_ADDR);
        
        VERBOSE(3, "send RSVP PATHERR to %s\n", int_2_addr(dest_addr));
        send_rsvp_message(&patherr, MTYPE_PATHERR, dest_addr);
    
        // MPLS setup
        mpls_ioctl   mpls;
        mpls.delete_in_label(label);
        label = 0;
        key = 0;
    
        // again, it would be nice to know, why the path went down. Again, someday... :)
        path_state = PATH_STATE_DOWN;
        return;
    }
}

//------------------------------------------------------------------------------

void rsvp_connection::PathTear_RD_flags_response(rsvp_configuration* packet_conf)
{           
    VERBOSE(5, "deleting path because of Admin_status RD flags - responding with PathTear\n");

    packet_conf->setadd(RC_MSG_ID_FLAGS,    (unsigned char) 1);
    packet_conf->setadd(RC_MSG_ID_EPOCH,    (unsigned int)  globals.epoch);
    packet_conf->setadd(RC_MSG_ID_ID,       (unsigned int)  globals.get_message_id());
    packet_conf->setadd(RC_SENDER_SRC_ADDR, *((unsigned int*)packet_conf->read(RC_FILTER_SRC_ADDR)));
    packet_conf->setadd(RC_SENDER_SRC_ID, *((unsigned short*)packet_conf->read(RC_FILTER_SRC_PORT)));
    packet_conf->setadd(RC_CONTROL_OUT_IF_ADDR,   inet_addr(globals.rsvp_local_if_addr));
    

    rsvp_packet  pathtear;
    pathtear.set_configuration(packet_conf);

    if (globals.protocol_vers == IETF_RSVP)
    {
        pathtear.add_object(CLASS_SESSION_IETF);
        pathtear.add_object(CLASS_SENDER_TEMPLATE);
        pathtear.add_object(CLASS_UPSTREAM_LABEL);
        
        unsigned char switching_type =  *(unsigned char*)packet_conf->read(RC_LSP_SWITCHING_TYPE);
        /* different switching types require different HOP object versions */
        if(switching_type >= 100)
            pathtear.add_object(CLASS_IPV4_IF_ID_RSVP_HOP); /* no-packet switching */
        else
            pathtear.add_object(CLASS_IPV4_RSVP_HOP); /* packet switching */
        pathtear.add_object(CLASS_STYLE);
        pathtear.add_object(CLASS_FILTER_SPEC);
        
    }
    else
    {
        pathtear.add_object(CLASS_MESSAGE_ID);
        pathtear.add_object(CLASS_SESSION_OIF);
        pathtear.add_object(CLASS_SENDER_TEMPLATE);

        if (globals.protocol_vers == OIFUNI20)
        {
            //pathtear.add_object(CLASS_TOKEN_SENDER_TSPEC); // UNI2.0Eth obsolete
            pathtear.add_object(CLASS_ETHERNET_SENDER_TSPEC);
            if(pathtear.add_object(CLASS_CALL_ID_2) < 0)
                if(pathtear.add_object(CLASS_CALL_ID_1) < 0)
                    pathtear.add_object(CLASS_CALL_ID_0);
        }
        else if(globals.protocol_vers == OIFUNI10)
            pathtear.add_object(CLASS_SONETSDH_SENDER_TSPEC);

        pathtear.add_object(CLASS_UPSTREAM_LABEL);
        pathtear.add_object(CLASS_IPV4_IF_ID_RSVP_HOP);
    }
    
    dispatcher->ask_interface(connection_id, *packet_conf, MTYPE_RESVTEAR);
  
    VERBOSE(3, "send RSVP PATHTEAR to %s\n", int_2_addr(router_address));
    send_rsvp_message(&pathtear, MTYPE_PATHTEAR, router_address);

    // MPLS setup
    mpls_ioctl   mpls;
    mpls.delete_in_label(label);
    label = 0;
    key = 0;

    // again, it would be nice to know, why the path went down. Again, someday... :)
    path_state = PATH_STATE_DOWN;
    return;
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_path(rsvp_configuration* packet_conf)
{
    VERBOSE(5, "we are responding the call of the path\n");

    path_configuration = *packet_conf;

    // let's look at the packet_conf and (someday... :)) check, if we can accept the path
    // it would be polite to respond with RESV then...

    if(label == 0)
        label = ++globals.label_id;

    path_state  = PATH_STATE_PATH_RECV;
    unsigned int dest_addr = *(unsigned int*)packet_conf->read(RC_CONTROL_OUT_IF_ADDR);

    if(packet_conf->ispresent(RC_ADMIN_STATUS_RTAD_FLAGS) == VAL_TRUE)
    {
        //@todo check why the object isn't there!
        unsigned char rtad = *(unsigned char*)packet_conf->read(RC_ADMIN_STATUS_RTAD_FLAGS);
        VERBOSE(7, "admin status present, rtad = 0x%x\n", rtad);

        if((rtad & 0x0F) == 9)
        {
            /* "reflect" & "delete" flags in Admin Status object
            - we should delete the path! */
            PathErr_RD_flags_response(packet_conf);
            return;
        }
        else if((rtad & 0x0F) == 10)
        {
            /* "reflect" & "administratively down" flags in Admin Status object
            - we should indicate path deletion! */
            packet_conf->setadd(RC_ADMIN_STATUS_RTAD_FLAGS, (unsigned char)9);
        }
        else
            packet_conf->setadd(RC_ADMIN_STATUS_RTAD_FLAGS, (unsigned char)0);
    }
    packet_conf->setadd(RC_CONTROL_OUT_IF_ADDR, inet_addr(globals.rsvp_local_if_addr));
    packet_conf->setadd(RC_TUNNEL_ID,           (unsigned short)connection_id);

    if(path_configuration.ispresent(RC_UPSTREAM_LABEL) == true)
    {
        label = *((unsigned int*)packet_conf->read(RC_UPSTREAM_LABEL));
        unsigned int label_buf = htonl(*((unsigned int*)packet_conf->read(RC_UPSTREAM_LABEL)));
        packet_conf->setadd(RC_LABEL, &label_buf, sizeof(label_buf)); 
    }
    else
    {
         unsigned int label_buf = htonl(label); 
         packet_conf->setadd(RC_LABEL, &label_buf, sizeof(label_buf));
    }
    
    packet_conf->setadd(RC_FILTER_SRC_ADDR, *((unsigned int*)packet_conf->read(RC_SENDER_SRC_ADDR)));
    packet_conf->setadd(RC_FILTER_SRC_PORT, *((unsigned short*)packet_conf->read(RC_SENDER_SRC_ID)));
    packet_conf->setadd(RC_WILDCARD_FILTER, (unsigned char)0);
    packet_conf->setadd(RC_FIXED_FILTER,    (unsigned char)1);
    packet_conf->setadd(RC_SHARED_EXPLICIT, (unsigned char)0);
    packet_conf->setadd(RC_RESV_CONFIRM_RECV_ADDR, inet_addr(globals.rsvp_local_if_addr));
    packet_conf->setadd(RC_MSG_ID_ID, globals.get_message_id());


    configuration = *packet_conf;

    rsvp_packet resv;
    resv.set_configuration(packet_conf);

    adding_resv_objects(&resv);
    
    dest_addr = inet_addr(globals.rsvp_router_addr);

    VERBOSE(3, "send RSVP RESV to %s\n", int_2_addr(dest_addr));
    send_rsvp_message(&resv, MTYPE_RESV, dest_addr);

    // MPLS setup
    mpls_ioctl      mpls;
    mpls.set_in_label(label);
    path_state = PATH_STATE_UP;
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_path_nack(rsvp_configuration* packet_conf)
{
    VERBOSE(5, "we are responding the call of the path with error\n");

    path_state = PATH_STATE_PATH_RECV;

    unsigned int dest_addr = inet_addr(globals.rsvp_router_addr); //*(unsigned int*)packet_conf->read(RC_CONTROL_OUT_IF_ADDR);

    packet_conf->setadd(RC_CONTROL_OUT_IF_ADDR, inet_addr(globals.rsvp_local_if_addr));
    packet_conf->setadd(RC_TUNNEL_ID,           (unsigned short)connection_id);
    packet_conf->setadd(RC_FILTER_SRC_ADDR,     *((unsigned int*)packet_conf->read(RC_SENDER_SRC_ADDR)));
    packet_conf->setadd(RC_FILTER_SRC_PORT,     *((unsigned short*)packet_conf->read(RC_SENDER_SRC_ID)));
    packet_conf->setadd(RC_ERROR_NODE_ADDR,     inet_addr(globals.rsvp_local_if_addr));
    packet_conf->setadd(RC_ERROR_FLAGS,         (unsigned char)0);
    packet_conf->setadd(RC_ERROR_CODE,          (unsigned char)2);
    packet_conf->setadd(RC_ERROR_VALUE,         (unsigned short)0);

    packet_conf->setadd(RC_MSG_ID_ID, globals.get_message_id());

    configuration = *packet_conf;

    rsvp_packet patherr;
    patherr.set_configuration(packet_conf);

    if(globals.protocol_vers == IETF_RSVP)
    {
        patherr.add_object(CLASS_SESSION_IETF);
        patherr.add_object(CLASS_ERROR_SPEC);
        patherr.add_object(CLASS_SENDER_TEMPLATE);
        patherr.add_object(CLASS_TOKEN_SENDER_TSPEC);
    }
    else
    {
        patherr.add_object(CLASS_MESSAGE_ID);
        patherr.add_object(CLASS_SESSION_OIF);
        patherr.add_object(CLASS_ERROR_SPEC);
        patherr.add_object(CLASS_SENDER_TEMPLATE);
        patherr.add_object(CLASS_SONETSDH_SENDER_TSPEC);

        // let's NOT ignore bidirectionalism :)
        if(packet_conf->ispresent(RC_UPSTREAM_LABEL) == VAL_TRUE)
            patherr.add_object(CLASS_UPSTREAM_LABEL);

        if(globals.protocol_vers == OIFUNI20)
        {
            if(patherr.add_object(CLASS_CALL_ID_2) < 0)
                if(patherr.add_object(CLASS_CALL_ID_1) < 0)
                    patherr.add_object(CLASS_CALL_ID_0);
        }
    }

    VERBOSE(3, "send RSVP PATHERR to %s\n", int_2_addr(dest_addr));
    send_rsvp_message(&patherr, MTYPE_PATHERR, dest_addr);

    path_state = PATH_STATE_DOWN;
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_resv(rsvp_configuration* packet_conf)
{
    if(packet_conf->ispresent(RC_ADMIN_STATUS_RTAD_FLAGS) == VAL_TRUE)
    {
        //@todo check why the object isn't there!
        unsigned char rtad = *(unsigned char*)packet_conf->read(RC_ADMIN_STATUS_RTAD_FLAGS);
        VERBOSE(7, "admin status present, rtad = 0x%x\n", rtad);

        if((rtad & 0x0F) == 9)
        {
            VERBOSE(5, "receiving RESV with admin status R&D flags\n");
            /* "reflect" & "delete" flags in Admin Status object
            - we should delete the path! */
            PathTear_RD_flags_response(packet_conf);
            return;
        }
        else if((rtad & 0x0F) == 1)
        {
            VERBOSE(5, "receiving RESV with admin status D flag\n");
            rsvp_configuration del_conf;
            del_conf = path_configuration;
            
            del_conf.add_missing(conf_defaults);
            if(globals.protocol_vers == IETF_RSVP)
            {
                /* removing unnecessary HOP object fields */
                del_conf.remove(RC_DATA_OUT_IF_HANDLE);
                del_conf.remove(RC_DATA_IN_IF_HANDLE);
                del_conf.remove(RC_DATA_IN_IF_ADDR);
            }
        
            rsvp_packet ptear;
            ptear.set_configuration(&del_conf);
    
            ptear.add_object(CLASS_SESSION_IETF);
            ptear.add_object(CLASS_IPV4_IF_ID_RSVP_HOP);
            ptear.add_object(CLASS_SENDER_TEMPLATE);
            //ptear.add_object(CLASS_TOKEN_SENDER_TSPEC); //UNI2.0Eth obsolete
            ptear.add_object(CLASS_ETHERNET_SENDER_TSPEC);

            VERBOSE(3, "send RSVP PATHTEAR to %s\n", int_2_addr(router_address));
            send_rsvp_message(&ptear, MTYPE_PATHTEAR, router_address);
            
            ip_route ipr;
            ipr.delete_route(dest_address);

            sleep(1);

            mpls_ioctl mpls;
            mpls.delete_out_label(key);

            label           = 0;
            key             = 0;
            dest_address    = 0;

            path_state      = PATH_STATE_DOWN;
            access.signal_plus();
            return;
        }
    }

    if(path_state == PATH_STATE_UP)
        return;

    path_state = PATH_STATE_RESV_RECV;

    // we should check config validity, but we assume, that it's OK (since we've received RESV)
    label = htonl(*((unsigned int*)packet_conf->read(RC_LABEL)));
    mpls_ioctl mpls;
    key = mpls.set_out_label(label, globals.rsvp_local_if_name,
            *((unsigned int*)packet_conf->read(RC_CONTROL_OUT_IF_ADDR)));

    VERBOSE(7, "going to add mpls route\n");
    ip_route ipr;
    ipr.add_mpls_route(dest_address, resolve_router_addr(*packet_conf), key);
    VERBOSE(7, "mpls route added\n");

    // respond with RESV_CONF
    rsvp_packet resvconf;
    unsigned int dest_addr;

    if(globals.protocol_vers != IETF_RSVP)
    {
        VERBOSE(5, "we are responding the resv packet with resvconf\n");
        dest_addr = *(unsigned int*)packet_conf->read(RC_CONTROL_OUT_IF_ADDR);

        packet_conf->setadd(RC_CONTROL_OUT_IF_ADDR,    inet_addr(globals.rsvp_local_if_addr));
        packet_conf->setadd(RC_ERROR_NODE_ADDR,        inet_addr(globals.rsvp_local_if_addr));
        packet_conf->setadd(RC_ERROR_FLAGS,            (unsigned char)0);
        packet_conf->setadd(RC_ERROR_CODE,             (unsigned char)0);
        packet_conf->setadd(RC_ERROR_VALUE,            (unsigned short)0);
        packet_conf->setadd(RC_RESV_CONFIRM_RECV_ADDR, dest_addr);
        packet_conf->setadd(RC_MSG_ID_ID, globals.get_message_id());

        configuration.update(*packet_conf);

        resvconf.set_configuration(packet_conf);

        resvconf.add_object(CLASS_MESSAGE_ID);
        resvconf.add_object(CLASS_SESSION_OIF);
        resvconf.add_object(CLASS_ERROR_SPEC);
        resvconf.add_object(CLASS_RESV_CONFIRM);
        resvconf.add_object(CLASS_STYLE);

        if(globals.protocol_vers == OIFUNI20)
        {
            //resvconf.add_object(CLASS_TOKEN_FLOWSPEC); //UNI2.0Eth obsolete
            resvconf.add_object(CLASS_ETHERNET_FLOWSPEC);
        }
        else if(globals.protocol_vers == OIFUNI10)
            resvconf.add_object(CLASS_SONETSDH_FLOWSPEC);

        resvconf.add_object(CLASS_FILTER_SPEC);
        resvconf.add_object(CLASS_GENERALIZED_LABEL);

	VERBOSE(3, "send RSVP RESVCONF to %s\n", int_2_addr(dest_addr));
        send_rsvp_message(&resvconf, MTYPE_RESVCONF, dest_addr);
    }
    path_state = PATH_STATE_UP;
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_patherr(rsvp_configuration* packet_conf)
{
    mpls_ioctl mpls;
    if(label != 0)
        mpls.delete_in_label(label);

    path_state  = PATH_STATE_DOWN;
    label       = 0;
    key         = 0;
    // maybe it would be good to save the information contained in the packet? Someday... :)
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_resverr(rsvp_configuration* packet_conf)
{
    path_state  = PATH_STATE_DOWN;
    label       = 0;
    // maybe it would be good to save the information contained in the packet? Someday... :)
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_pathtear(rsvp_configuration* packet_conf)
{
    path_state = PATH_STATE_PATHTEAR_RECV;
    mpls_ioctl mpls;
    mpls.delete_in_label(label);

    label   = 0;
    key     = 0;
    // again, it would be nice to know, why the path went down. Again, someday... :)
    path_state = PATH_STATE_DOWN;
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_resvtear(rsvp_configuration* packet_conf)
{
    mpls_ioctl mpls;
    path_state = PATH_STATE_RESVTEAR_RECV;
    mpls.delete_out_label(key);

    label   = 0;
    key     = 0;
    // again, it would be nice to know, why the path went down. Again, someday... :)
    path_state = PATH_STATE_DOWN;
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_resvconf(rsvp_configuration* packet_conf)
{
    // let's ignore it, for now
}

//------------------------------------------------------------------------------

void rsvp_connection::respond_ackreq(rsvp_configuration* packet_conf)
{
    unsigned int dest_addr;
    /* @todo we will finish here, on implementation of ack response to ack requests in packets.
    Invocation of this method is already in place
    we should copy configuration (we are not allowed to alter packet_conf check
    if valid data is set and proper objects are added - this was copied from patherr */

    VERBOSE(7, "we are responding the ack request\n");

    rsvp_configuration new_conf = *packet_conf;

//    dest_addr = *(unsigned int*)new_conf.read(RC_EXTENDED_IPV4_ADDR);

    /* @todo hot-fix! We should get the address probably from IPv4 header or
    some other source - maybe the line below?... */
    dest_addr = resolve_router_addr(new_conf);

    new_conf.set(RC_MSG_ID_FLAGS, (unsigned char)0);

    rsvp_packet ack;
    ack.set_configuration(&new_conf);

    ack.add_object(CLASS_MESSAGE_ID_ACK);

    VERBOSE(3, "send RSVP ACK to %s\n", int_2_addr(dest_addr));
    send_rsvp_message(&ack, MTYPE_ACK, dest_addr);
}

//------------------------------------------------------------------------------

void rsvp_connection::get_packet(int msg_type, void* buf, int buf_len, rsvp_configuration* conf, int nack)
/* conf not NULL means, that dispatcher has been asked (?) */
{
    VERBOSE(7, "waiting for access (access semaphore)\n");
    access.wait();
    VERBOSE(7, "passed through access semaphore\n");

    rsvp_configuration* packet_conf;
    rsvp_packet         packet;

    if(conf != NULL)
        packet_conf = new rsvp_configuration(*conf);
    else
        packet_conf = packet.parse_data(buf, buf_len);

    VERBOSE(7, "configuration done\n");

    // answer with ack packet if ack request is included
    if(packet_conf->ispresent(RC_MSG_ID_FLAGS))
        if((*(unsigned char*)packet_conf->read(RC_MSG_ID_FLAGS) == 1) && conf == NULL)
        {
            VERBOSE(5, "ressponding ack\n");
            respond_ackreq(packet_conf);
        }

    if(check_if_packet_valid(msg_type) == VAL_FALSE)
    {
        access.signal_plus();
        VERBOSE(5, "Invalid packet for this connection %i\n", msg_type);
        return;
    }

    if(path_direction == DIRECTION_UNDEFINED && msg_type == MTYPE_PATH)
        path_direction = DIRECTION_INCOMING;

    /* HERE! Do the check: if we should ask the interface, send this packet to dispatcher
    and make him ask the interface then, when the interface responds, it can make dispatcher
    send this packet to this connection again, but without this check
    (i.e. via other method, which will just invoke respond_xxx method)
    dispatcher has to check, if he has an interface to ask - if not,
    it should indicate this via return value, or sth */

    bool done = true;
    switch(path_direction)
    {
    case DIRECTION_OUTGOING:
        switch(msg_type)
        {
        case MTYPE_RESV:
            VERBOSE(3, "receive RSVP RESV\n");
            respond_resv(packet_conf);
            break;
        case MTYPE_PATHERR:
            VERBOSE(3, "receive RSVP PATHERR\n");
            respond_patherr(packet_conf);
            break;
        case MTYPE_RESVTEAR:
            VERBOSE(3, "receive RSVP RESVTEAR\n");
            respond_resvtear(packet_conf);
            break;
        default:
            done = false;
        }
        if(done)
            dispatcher->ask_interface(connection_id, *packet_conf, msg_type);
        break;
    case DIRECTION_INCOMING:
        switch(msg_type)
        {
        case MTYPE_PATH:	    
            VERBOSE(8, "rsvp_connection::get_packet - MTYPE_PATH, state is %d, conf is %d\n", path_state, conf);
            if(conf != NULL)
            {
                // PATH message activated and delivered by mngt
                if(nack == 0)
                    respond_path(packet_conf);
                else
                    respond_path_nack(packet_conf);
            }
            else
            {
                VERBOSE(3, "receive RSVP PATH\n");
    
                if(path_state == PATH_STATE_NULL)
                {
                    path_state  = PATH_STATE_PATH_RECV;
                    if(dispatcher->ask_interface(connection_id, *packet_conf, msg_type) != 0)
                        if(globals.translator == true)
                            respond_path_nack(packet_conf);
                        else
                            respond_path(packet_conf);
                }
                else if(path_state == PATH_STATE_UP)
                    respond_path(packet_conf);
                else
                    respond_path_nack(packet_conf);
            }
            break;
        case MTYPE_RESVERR:
            VERBOSE(3, "receive RSVP RESVERR\n");
            respond_resverr(packet_conf);
            dispatcher->ask_interface(connection_id, *packet_conf, msg_type);
            break;
        case MTYPE_PATHTEAR:
            VERBOSE(3, "receive RSVP PATHTEAR\n");
            respond_pathtear(packet_conf);
            dispatcher->ask_interface(connection_id, *packet_conf, msg_type);
            break;
        case MTYPE_RESVCONF:
            VERBOSE(3, "receive RSVP RESVCONF\n");
            respond_resvconf(packet_conf); // no dispatcher asking at the moment :)
            break;
        }
    }
    access.signal_plus();
}

//------------------------------------------------------------------------------

int rsvp_connection::check_if_packet_valid(int msg_type)
{

    VERBOSE(8, "check_if_packet_valid: msg_type=%d, path_direction=%d, path_state=%d\n", msg_type, path_direction, path_state);
    
    if(msg_type == MTYPE_BUNDLE || msg_type == MTYPE_ACK ||
       msg_type == MTYPE_SREFRESH || msg_type == MTYPE_HELLO)
            return VAL_FALSE;

    if(path_direction == DIRECTION_OUTGOING)
        if(msg_type == MTYPE_PATH || msg_type == MTYPE_RESVERR ||
           msg_type == MTYPE_PATHTEAR || msg_type == MTYPE_RESVCONF)
            return VAL_FALSE;

    if(path_direction == DIRECTION_INCOMING)
        if(msg_type == MTYPE_RESV || msg_type == MTYPE_PATHERR ||
           msg_type == MTYPE_RESVTEAR)
            return VAL_FALSE;

    if(path_direction == DIRECTION_UNDEFINED)
        if(msg_type != MTYPE_PATH)
            return VAL_FALSE;

    if(path_state == PATH_STATE_NULL || path_state == PATH_STATE_DOWN)
        if(msg_type != MTYPE_PATH && msg_type != MTYPE_PATHERR)
            return VAL_FALSE;

    if(path_state == PATH_STATE_PATH_SENT)
        if(msg_type != MTYPE_RESV && msg_type != MTYPE_PATHERR)
            return VAL_FALSE;

    return VAL_TRUE;
}

//------------------------------------------------------------------------------

int rsvp_connection::create_path(const char* router_addr)
{
    return create_path(inet_addr(router_addr));
}

//------------------------------------------------------------------------------

void rsvp_connection::adding_create_path_objects(rsvp_packet* mes, rsvp_configuration* conf)
{
    mes->add_object(CLASS_GENERALIZED_LABEL_REQUEST);
    mes->add_object(CLASS_SENDER_TEMPLATE);
    if(conf->ispresent(RC_UPSTREAM_LABEL) == VAL_TRUE)
       mes->add_object(CLASS_UPSTREAM_LABEL);

    if(globals.translator == false && conf->ispresent(RC_ERO) == true)
       mes->add_object(CLASS_ERO);
       
    unsigned char  switching_type =  *(unsigned char*)conf->read(RC_LSP_SWITCHING_TYPE);

    if(globals.protocol_vers == IETF_RSVP)
    {
        mes->add_object(CLASS_SESSION_IETF);
        
        /* different switching types require different HOP object versions */
        //if(switching_type >= 100)
            mes->add_object(CLASS_IPV4_IF_ID_RSVP_HOP); /* no-packet switching */
        //else
        //    mes->add_object(CLASS_IPV4_RSVP_HOP); /* packet switching */
        mes->add_object(CLASS_TIME_VALUES);
        mes->add_object(CLASS_SESSION_ATTRIBUTE);
        mes->add_object(CLASS_TOKEN_SENDER_TSPEC);
    }
    else
    {
        mes->add_object(CLASS_MESSAGE_ID);
        mes->add_object(CLASS_SESSION_OIF);
        mes->add_object(CLASS_IPV4_IF_ID_RSVP_HOP);
        mes->add_object(CLASS_TIME_VALUES);
        mes->add_object(CLASS_GENERALIZED_UNI);
        mes->add_object(CLASS_GU_SOURCE_TNA_ADDRESS);
        mes->add_object(CLASS_GU_DESTINATION_TNA_ADDRESS);
        mes->add_object(CLASS_GU_SERVICE_LEVEL);
        mes->add_object(CLASS_SESSION_ATTRIBUTE);
        mes->add_object(CLASS_NOTIFY_REQUEST);
        mes->add_object(CLASS_ADMIN_STATUS);

        if(globals.protocol_vers == OIFUNI20)
        {
            //mes->add_object(CLASS_TOKEN_SENDER_TSPEC); // UNI2.0Eth obsolete
            mes->add_object(CLASS_ETHERNET_SENDER_TSPEC);
        }
        else if(globals.protocol_vers == OIFUNI10)
            mes->add_object(CLASS_SONETSDH_SENDER_TSPEC);

        if(globals.protocol_vers == OIFUNI20)
        {
            if(mes->add_object(CLASS_CALL_ID_2) < 0)
                if(mes->add_object(CLASS_CALL_ID_1) < 0)
                      mes->add_object(CLASS_CALL_ID_0); ;
        }
    }
    
}

//------------------------------------------------------------------------------

int rsvp_connection::create_path(unsigned int router_addr)
{
    access.wait();
    VERBOSE(1, "creating path\n");

    router_address  = router_addr;
    path_direction  = DIRECTION_OUTGOING;
    path_state      = PATH_STATE_INITIATED;

    rsvp_configuration path_conf = configuration;
    
    // if rsvp session source address present then use it in all fields
    if(path_conf.ispresent(RC_EXTENDED_IPV4_ADDR) == true)
    {
        unsigned int src_addr =  *(unsigned int*)path_conf.read(RC_EXTENDED_IPV4_ADDR);
        path_conf.setadd(RC_SENDER_SRC_ADDR,       (unsigned int)src_addr);
    }

    if(path_conf.ispresent(RC_CONTROL_OUT_IF_ADDR) == true)
    {
        unsigned int src_addr =  *(unsigned int*)path_conf.read(RC_CONTROL_OUT_IF_ADDR);
        path_conf.setadd(RC_DATA_OUT_IF_ADDR,      (unsigned int)src_addr);   
        path_conf.setadd(RC_DATA_IN_IF_ADDR,       (unsigned int)src_addr);
    }
      
    if(globals.translator == true)
    {
        path_conf.setadd(RC_SESSION_DEST_ADDR,   inet_addr(globals.default_path_dest));
    
        path_conf.setadd(RC_EXTENDED_IPV4_ADDR,  inet_addr(globals.rsvp_local_if_addr));
        path_conf.setadd(RC_DATA_OUT_IF_ADDR,    inet_addr(globals.rsvp_local_if_addr));
        path_conf.setadd(RC_DATA_IN_IF_ADDR,     inet_addr(globals.rsvp_local_if_addr));
        path_conf.setadd(RC_SENDER_SRC_ADDR,     inet_addr(globals.rsvp_local_if_addr));
        path_conf.setadd(RC_CONTROL_OUT_IF_ADDR, inet_addr(globals.rsvp_local_if_addr));
    }
       
    path_conf.add_missing(conf_defaults);
    
    // variable used by rsvp_connection::respond_resv method
    dest_address = *(unsigned int*)path_conf.read(RC_TR_D_ADDR);
    path_conf.setadd(RC_MSG_ID_ID, globals.get_message_id());  
    
    if(globals.protocol_vers == IETF_RSVP)
    {
        /* removing unnecessary HOP object fields */
        path_conf.remove(RC_DATA_OUT_IF_HANDLE);
        path_conf.remove(RC_DATA_IN_IF_HANDLE);
        path_conf.remove(RC_DATA_IN_IF_ADDR);
    }
    
    if(globals.protocol_vers == OIFUNI20)
    {
        /* changes for final UNI2.0Eth */
        if(path_conf.ispresent(RC_CONTROL_OUT_IF_HANDLE) == true)
            path_conf.setadd(RC_UPSTREAM_LABEL,  *(unsigned int*)path_conf.read(RC_CONTROL_OUT_IF_HANDLE));
        
        path_conf.setadd(RC_CALL_ID, (unsigned int)0); // a trick for activation of an empty CALL_ID object
        path_conf.setadd(RC_NOTIFY_ADDRESS,        inet_addr(globals.rsvp_local_if_addr));
        /* end of changes for final UNI2.0Eth */
    }
      
    configuration.update(path_conf);

    rsvp_packet path;  
    path.set_configuration(&path_conf);
      
    adding_create_path_objects(&path, &path_conf);
       
    //if(path_conf.ispresent(RC_ADMIN_STATUS_RTAD_FLAGS) == true)
    //    path.add_object(CLASS_ADMIN_STATUS);
    path_configuration = path_conf;

    VERBOSE(3, "send RSVP PATH to %s\n", int_2_addr(router_address));
    
    send_rsvp_message(&path, MTYPE_PATH, router_address);
    path_state = PATH_STATE_PATH_SENT;
    access.signal_plus();
    return 0;
}

//------------------------------------------------------------------------------

void* rsvp_connection::refresh_path(rsvp_connection* o)
{
    int tick_count = 0;
    int till_state_up_count = 0;
    
    while(1)
    {
        if(o->path_state == PATH_STATE_DOWN)
        {
            o->refresh_tread_state = 0;
            rsvp_dispatcher* disp = o->dispatcher;
            // remove old pathes (teardowned)
            for(int x=0; x<disp->registered_conns_count; x++)
                if(disp->registered_conns[x].conn->get_path_state() == PATH_STATE_DOWN 
                   && disp->registered_conns[x].conn->refresh_tread_state == 0)
                {
                    delete disp->registered_conns[x].conn;
                    disp->unregister_connection(x);
                }
            return NULL;
        }
        
        if(o->path_state != PATH_STATE_UP && o->path_state != PATH_STATE_DOWN)
            till_state_up_count++;
        
        // after some second of timeout PATH_STATE_SENT is changed to PATH_STATE_DOWN and CREATE NACK is sent to the client
        if(till_state_up_count > globals.path_setup_timeout && o->path_direction == DIRECTION_OUTGOING)
        {
            VERBOSE(5, "Path setup timeout\n");
            o->access.wait();
            o->dispatcher->ask_interface(o->connection_id, o->path_configuration, MTYPE_PATHERR);
            o->path_state = PATH_STATE_DOWN;
            o->refresh_tread_state = 0;
            o->access.signal_plus();
            VERBOSE(7, "semaphore signalled\n");
            return NULL;
        }

        if(tick_count % o->refresh_interval == 0)
        {
            o->access.wait();      
            if(o->path_state == PATH_STATE_UP && o->path_direction == DIRECTION_OUTGOING && o->router_address != 0)
            {
                VERBOSE(5, "Refreshing path with label %d and ID %d\n", o->label, o->connection_id);
    
                o->path = new rsvp_packet();

                o->path->set_configuration(&o->path_configuration);
                o->adding_create_path_objects(o->path, &o->configuration);
            
                VERBOSE(3, "send RSVP REFRESH PATH to %s\n", int_2_addr(o->router_address));
                o->send_rsvp_message(o->path, MTYPE_PATH,  o->router_address);
    
                VERBOSE(7, "deleting path object\n");
                delete(o->path);
                o->path = NULL;
            }
            o->access.signal_plus();
            VERBOSE(7, "semaphore signalled\n");
        }
        sleep(1);
        tick_count++;
    }
    return NULL;
}

//------------------------------------------------------------------------------

int rsvp_connection::destroy_path()
{
    access.wait();
    if(globals.protocol_vers != IETF_RSVP)
    {
        if(path_direction == DIRECTION_OUTGOING)
        {
            VERBOSE(6, "sending path with admin status R&D flags\n");

            router_address = resolve_router_addr(configuration);

            rsvp_configuration pathrd_conf = configuration;

            pathrd_conf.setadd(RC_MSG_ID_ID,                globals.get_message_id());
            pathrd_conf.setadd(RC_ADMIN_STATUS_RTAD_FLAGS,  (unsigned char)0x09); // R&D flags

            rsvp_packet pathrd;
            pathrd.set_configuration(&pathrd_conf);

            pathrd.add_object(CLASS_MESSAGE_ID);
            pathrd.add_object(CLASS_SESSION_OIF);
            pathrd.add_object(CLASS_IPV4_IF_ID_RSVP_HOP);
            pathrd.add_object(CLASS_TIME_VALUES);
            pathrd.add_object(CLASS_GENERALIZED_LABEL_REQUEST);
            pathrd.add_object(CLASS_GENERALIZED_UNI);
            pathrd.add_object(CLASS_GU_SOURCE_TNA_ADDRESS);
            pathrd.add_object(CLASS_GU_DESTINATION_TNA_ADDRESS);
            pathrd.add_object(CLASS_GU_SERVICE_LEVEL);
            pathrd.add_object(CLASS_SENDER_TEMPLATE);
            pathrd.add_object(CLASS_ADMIN_STATUS);

            if(globals.protocol_vers == OIFUNI20)
            {
                //pathrd.add_object(CLASS_TOKEN_SENDER_TSPEC); // UNI2.0Eth obsolete
                pathrd.add_object(CLASS_ETHERNET_SENDER_TSPEC);
            }
            else if(globals.protocol_vers == OIFUNI10)
                pathrd.add_object(CLASS_SONETSDH_SENDER_TSPEC);

            if(pathrd_conf.ispresent(RC_UPSTREAM_LABEL) == VAL_TRUE)
                pathrd.add_object(CLASS_UPSTREAM_LABEL);

            if(globals.protocol_vers == OIFUNI20)
            {
                if(pathrd.add_object(CLASS_CALL_ID_2) < 0)
                    if(pathrd.add_object(CLASS_CALL_ID_1) < 0)
                        pathrd.add_object(CLASS_CALL_ID_0);
            }

            VERBOSE(3, "send RSVP PATH with admin status R&D flags to %s\n", int_2_addr(router_address));
            send_rsvp_message(&pathrd, MTYPE_PATH, router_address);
            access.signal_plus();
            return 0;
        }
        if(path_direction == DIRECTION_INCOMING)
        {
            rsvp_configuration packet_conf = configuration;

            packet_conf.setadd(RC_ADMIN_STATUS_RTAD_FLAGS,  (unsigned char)0x09); // R&D flags

            unsigned int label_buf = htonl(label);
            packet_conf.setadd(RC_LABEL, &label_buf, sizeof(label_buf));
            configuration = packet_conf;

            rsvp_packet resv;
            resv.set_configuration(&packet_conf);

            adding_resv_objects(&resv);
            resv.add_object(CLASS_ADMIN_STATUS);

            VERBOSE(3, "send RSVP RESV with admin status R&D flags to %s\n", int_2_addr(router_address));
            send_rsvp_message(&resv, MTYPE_RESV, router_address);
            
            access.signal_plus();

            return 0;
        }
    }
    else
    {
        VERBOSE(1, "deleting path\n");

        rsvp_configuration del_conf;
        del_conf = configuration;
        del_conf.add_missing(conf_defaults);
        del_conf.remove(RC_UPSTREAM_LABEL);

        /* removing unnecessary HOP object fields */
        del_conf.remove(RC_DATA_OUT_IF_HANDLE);
        del_conf.remove(RC_DATA_IN_IF_HANDLE);
        del_conf.remove(RC_DATA_IN_IF_ADDR);

        del_conf.set(RC_MSG_ID_ID, globals.get_message_id());

        if(path_direction == DIRECTION_OUTGOING)
        {
            if(configuration.ispresent(RC_UPSTREAM_LABEL) == false)
            {
                rsvp_packet ptear;
                ptear.set_configuration(&del_conf);
        
                ptear.add_object(CLASS_SESSION_IETF);
                ptear.add_object(CLASS_IPV4_RSVP_HOP);
                ptear.add_object(CLASS_SENDER_TEMPLATE);
                ptear.add_object(CLASS_TOKEN_SENDER_TSPEC);
    
                VERBOSE(3, "send RSVP PATHTEAR to %s\n", int_2_addr(router_address));
                send_rsvp_message(&ptear, MTYPE_PATHTEAR, router_address);
            }
            else
            {
                VERBOSE(6, "sending path with admin status R&D flags\n");
                path_configuration.setadd(RC_ADMIN_STATUS_RTAD_FLAGS,  (unsigned char)0x09); // R&D flags
                rsvp_packet path;
                path.set_configuration(&path_configuration);
                adding_create_path_objects(&path, &path_configuration);
                path.add_object(CLASS_ADMIN_STATUS);
                      
                router_address = resolve_router_addr(configuration);
                VERBOSE(3, "send RSVP PATH with admin status R&D flags to %s\n", int_2_addr(router_address));
                send_rsvp_message(&path, MTYPE_PATH, router_address);
                access.signal_plus();

                return 0;
            }
        }
        if(path_direction == DIRECTION_INCOMING)
        {
            if(configuration.ispresent(RC_UPSTREAM_LABEL) == false)
            {
                rsvp_packet rtear; 
                del_conf.remove(RC_REFRESH_PERIOD); 
                rtear.set_configuration(&del_conf); 
                adding_resv_objects(&rtear);
                VERBOSE(3, "send RSVP RESVTEAR to %s\n", int_2_addr(router_address));
                send_rsvp_message(&rtear, MTYPE_RESVTEAR, router_address);
             }
             else
             {
                rsvp_configuration packet_conf = configuration;

                packet_conf.setadd(RC_ADMIN_STATUS_RTAD_FLAGS,  (unsigned char)0x09); // R&D flags
    
                unsigned int label_buf = htonl(label);
                packet_conf.setadd(RC_LABEL, &label_buf, sizeof(label_buf));
                configuration = packet_conf;
    
                rsvp_packet resv;
                resv.set_configuration(&packet_conf);
    
                adding_resv_objects(&resv);
                resv.add_object(CLASS_ADMIN_STATUS);
    
                VERBOSE(3, "send RSVP RESV with admin status R&D flags to %s\n", int_2_addr(router_address));
                send_rsvp_message(&resv, MTYPE_RESV, router_address);
                
                access.signal_plus();
    
                return 0;
             }
            
        }
        if(path_direction == DIRECTION_INCOMING || path_direction == DIRECTION_OUTGOING)
        {
            path_state = PATH_STATE_PATHTEAR_SENT;

            ip_route ipr;
            ipr.delete_route(dest_address);

            sleep(1);

            mpls_ioctl mpls;
            mpls.delete_out_label(key);

            label           = 0;
            key             = 0;
            dest_address    = 0;

            path_state      = PATH_STATE_DOWN;
            access.signal_plus();
            return 0;
        }

    }
    access.signal_plus();
    return ERROR_CANNOT_DESTROY;
}

//------------------------------------------------------------------------------

void rsvp_connection::send_rsvp_message(rsvp_packet* packet, int mtype, int dest_addr)
{
    char* data[1];
    data[0] = (char*)malloc(8192);

    int data_size = packet->get_data(*data, 8192);
    VERBOSE(6, "data size is %d\n", data_size);

    data_size = packet->make_rsvp_packet((void**)data, data_size, mtype);
    VERBOSE(6, "trying to send a packet to dest_addr of %s, data size is %d\n",
             int_2_addr(dest_addr), data_size);

    if(dispatcher->send(*data, data_size, dest_addr) < 0)
        {VERBOSE(0, "dispatcher couldn't send the packet\n");}
    else if (mtype != MTYPE_ACK)   /* ack message don't increment a counter */
        globals.inc_message_id();   /* message has been sended
                                       and we are able to increment a counter */
    VERBOSE(6, "packet sent\n");
    free(data[0]);
}

//------------------------------------------------------------------------------

char* rsvp_connection::get_path_state(char* ptr)
{
    char* buf = NULL;
    access.wait();
    if(path_state == PATH_STATE_NULL)
        buf = strdup("Null");
    if(path_state == PATH_STATE_INITIATED)
        buf = strdup("Initiated");
    if(path_state == PATH_STATE_PATH_SENT)
        buf = strdup("Path sent");
    if(path_state == PATH_STATE_PATH_RECV)
        buf = strdup("Path received");
    if(path_state == PATH_STATE_RESV_RECV)
        buf = strdup("Resv received");
    if(path_state == PATH_STATE_RESV_SENT)
        buf = strdup("Resv sent");
    if(path_state == PATH_STATE_UP)
        buf = strdup("Up");
    if(path_state == PATH_STATE_RESVTEAR_RECV)
        buf = strdup("ResvTear received");
    if(path_state == PATH_STATE_RESVTEAR_SENT)
        buf = strdup("ResvTear sent");
    if(path_state == PATH_STATE_PATHTEAR_SENT)
        buf = strdup("PathTear sent");
    if(path_state == PATH_STATE_PATHTEAR_RECV)
        buf = strdup("PathTear received");
    if(path_state == PATH_STATE_DOWN)
        buf = strdup("Down");
    if(buf == NULL)
        buf = strdup("Path state unknown");
    access.signal_plus();
    return buf;
}

//------------------------------------------------------------------------------

int rsvp_connection::get_path_state()
{
    switch(path_state)
    { 
    case PATH_STATE_PATH_RECV:
        return PATH_STATE_UP;
        break;
    case PATH_STATE_RESV_RECV:
        return PATH_STATE_UP;
        break;
    case PATH_STATE_RESV_SENT:
        return PATH_STATE_UP;
        break;
    case PATH_STATE_RESVTEAR_RECV:
    case PATH_STATE_RESVTEAR_SENT:
    case PATH_STATE_PATHTEAR_SENT:
    case PATH_STATE_PATHTEAR_RECV:
         return PATH_STATE_DOWN;
         break;
    default:
        return path_state;
    }
}

//------------------------------------------------------------------------------

int rsvp_connection::get_label()
{
    return label;
}

//------------------------------------------------------------------------------

#endif
