/***************************************************************************
 *   Copyright (C) 2007 by Damian Parniewicz PSNC                          *
 *   damianp@man.poznan.pl                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <float.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "agent_api.h"

using namespace std;

//===============================================================================================

agent_api::agent_api(const char* addr, unsigned short port)
{
    init((unsigned int)inet_addr((const char*)addr), port);
}

agent_api::agent_api(unsigned int addr, unsigned short port)
{
    init(addr, port);
}

agent_api::agent_api()
{
    init((unsigned int)inet_addr((const char*)"127.0.0.1"), (unsigned short)35000);
}

agent_api::~agent_api()
{
    api_disconnect();
    running = false;
}

//--------------------------------------------------------------------------------------------------

void agent_api::init(unsigned int addr, unsigned short port)
{
    next_message_id = 0;
    running = true;

    agent_addr = addr;
    agent_port = port;
    
    socket_desc = -1;

    api_connect();
    
    pthread_create(&socket_listener_tid, NULL, (void*(*)(void*))(socket_listener), this);
}

//--------------------------------------------------------------------------------------------------

void agent_api::api_connect()
{
    if(socket_desc >= 0)
        return;
   
    socket_desc = socket(PF_INET, SOCK_STREAM, 6);
    printf("socket_desc: %d\n", socket_desc);

    struct sockaddr_in si_addr;
    si_addr.sin_family = AF_INET;
    si_addr.sin_port = htons(agent_port);
    si_addr.sin_addr.s_addr = agent_addr;

    int t = connect(socket_desc, (struct sockaddr*)&si_addr, sizeof(si_addr));
    printf("socket connected : %d, socket_desc: %d\n", t, socket_desc);
    if(t != 0)
    {
        printf("Connection refused !!!\n");
        close(socket_desc);
    }
}

//--------------------------------------------------------------------------------------------------

void agent_api::api_disconnect()
{
    if(socket_desc < 0)
        return;

    printf("Close connection!\n");
    close(socket_desc);
    socket_desc = -1;
}


//--------------------------------------------------------------------------------------------------
int agent_api::send_message(unsigned char command, unsigned char message_type, configuration* conf, int message_id)
{
    int id = (message_id >= 0) ? message_id : next_message_id++;
    mgmt_packet mgmt = mgmt_packet(conf, id, message_type, command);
    bool success = send(&mgmt);
    return success ? id : -1;
}

//-------------------------------------------------------------------------------------------------

bool agent_api::send(mgmt_packet* packet)
{
    if(socket_desc < 0)
        api_connect();
    
    int data_size = 0;

    void* data = packet->get_data(&data_size);

    int bytes = write(socket_desc, data, data_size);
    if(bytes != data_size)
        return false;
    return true;
}

//--------------------------------------------------------------------------------------------------

void agent_api::socket_listener(agent_api* api)
{
    api->receive_messages();
}


//--------------------------------------------------------------------------------------------------
int agent_api::receive_messages()
{
    void* data = NULL;
    unsigned int data_size = 1500;
    int bytes;
    data = realloc(data, data_size);
    
    // wait for packet (receive in loop until a request)
    while(running)
    {
        printf("listening...\n");
        if(socket_desc < 0)
        {
           sleep(1);
           continue;
        }

        bytes = read(socket_desc, data, data_size);
        printf("read %d bytes from socket\n", bytes);

        if(bytes > 0)
        {
            mgmt_packet* mgmt = new mgmt_packet((char*)data, bytes);
            unsigned char packet_type = mgmt->get_packet_type();
            unsigned char packet_comm = mgmt->get_packet_comm();
            unsigned short packet_id = mgmt->get_packet_id();
            configuration  conf = mgmt->get_conf();
            delete mgmt;

            receive_message(packet_comm, packet_type, &conf, packet_id);
        }
        else
        {
            printf("Closing connection\n");
            close(socket_desc);
            socket_desc = -1;
        }
    }
    printf("End of listening loop !!!");
    free(data);
    data_size = 0;
    data = NULL;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//// 
////        TEST and EXAMPLES of AGENT_API usage
//// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

//===================================================================================================
configuration* prepare_conf()
{
  configuration* conf = new configuration();
  
  conf->dst           = inet_addr("1.2.3.4");     conf->used[OBJECT_DST_ADDR]       = true;
  conf->src           = inet_addr("5.6.7.8");     conf->used[OBJECT_SRC_ADDR]       = true;
  conf->out_if_addr   = inet_addr("9.10.11.12");  conf->used[OBJECT_OUT_ADDR]       = true;
  conf->refresh       = 1;                        conf->used[OBJECT_REFRESH]        = true;
  conf->tb_rate       = 2.1;                      conf->used[OBJECT_TB_RATE]        = true;
  conf->tb_size       = 3.2;                      conf->used[OBJECT_TB_SIZE]        = true;
  conf->peak_rate     = 4.3;                      conf->used[OBJECT_PEAK]           = true;
  conf->up_label      = 5;                        conf->used[OBJECT_UP_LABEL]       = true;
  conf->enc           = 6;                        conf->used[OBJECT_LSP_ENC]        = true;
  conf->switch_type   = 7;                        conf->used[OBJECT_LSP_SW]         = true;
  conf->gpid          = 8;                        conf->used[OBJECT_LSP_GPID]       = true;
  strcpy(conf->name,    "qwertyuiop");            conf->used[OBJECT_PATH_NAME]      = true;
  conf->ero           = inet_addr("13.14.15.16"); conf->used[OBJECT_ERO_ADDR]       = true;
  conf->ero_pref_len  = 9;                        conf->used[OBJECT_ERO_PREF]       = true;
  conf->ero_sl        = true;                     conf->used[OBJECT_ERO_SL]         = true;
  conf->hop           = inet_addr("17.18.19.20"); conf->used[OBJECT_HOP_ADDR]       = true;
  conf->err_node      = inet_addr("21.22.23.24"); conf->used[OBJECT_ERROR_ADDR]     = true;
  conf->err_flag      = 10;                       conf->used[OBJECT_ERROR_FLAGS]    = true;
  conf->err_code      = 11;                       conf->used[OBJECT_ERROR_CODE]     = true;  
  conf->err_val       = 12;                       conf->used[OBJECT_ERROR_VALUE]    = true;
  conf->mpart_number  = 13;                       conf->used[OBJECT_MULTI_NUM]      = true;
  conf->mpart_count   = 14;                       conf->used[OBJECT_MULTI_COUNT]    = true;
  conf->tun_id        = 15;                       conf->used[OBJECT_TUNNEL_ID]      = true;
  conf->traff_dst     = inet_addr("24.25.26.27"); conf->used[OBJECT_TR_RD_ADDR]     = true;
  conf->traff_rev_dst = inet_addr("28.29.30.31"); conf->used[OBJECT_TR_D_ADDR]      = true;
  conf->interface_id  = 16;                       conf->used[OBJECT_OUT_IF_HANDLE]  = true;
  conf->src_tna       = inet_addr("32.33.34.35"); conf->used[OBJECT_SRC_TNA]        = true;
  conf->dst_tna       = inet_addr("36.37.38.39"); conf->used[OBJECT_DST_TNA]        = true;
  conf->call_id_1     = 17;                       conf->used[OBJECT_CALL_ID_1]      = true;
  conf->call_id_2     = 18;                       conf->used[OBJECT_CALL_ID_2]      = true;
  conf->lsp_id        = 19;                       conf->used[OBJECT_SENDER_LSP_ID]  = true;
  conf->setup_prior   = 20;                       conf->used[OBJECT_SETUP_PRIOR]    = true;
  conf->hold_prior    = 21;                       conf->used[OBJECT_HOLD_PRIOR]     = true;
  conf->path_state    = 22;                       conf->used[OBJECT_PATH_STATE]     = true;
  conf->serv_level    = 23;                       conf->used[OBJECT_PATH_SERV_LEVEL]= true;
  conf->eth_cir       = 24.24;                    conf->used[OBJECT_ETHERNET_CIR]   = true;
  conf->eth_cbs       = 25.25;                    conf->used[OBJECT_ETHERNET_CBS]   = true;
  conf->eth_eir       = 26.26;                    conf->used[OBJECT_ETHERNET_EIR]   = true;
  conf->eth_ebs       = 27.27;                    conf->used[OBJECT_ETHERNET_EBS]   = true;
  conf->eth_granuality= 28;                       conf->used[OBJECT_ETHERNET_SW_GRANUALITY]     = true;
  conf->eth_index     = 29;                       conf->used[OBJECT_ETHERNET_INDEX] = true;
  conf->eth_profile   = 30;                       conf->used[OBJECT_ETHERNET_PROFILE] = true;
  
  return conf;
};

//--------------------------------------------------------------------------------------------------

//----------------------------------------------
#define OBJECT_TEST_MACRO(object_field, object_id, requested_value) \
  if(conf->used[object_id] == true)            \
     if(object_field != requested_value) {     \
        printf("  Object id: %i, received value: %i, espected value: %i\n", object_id, object_field, requested_value); \
        return false;                          \
     };
     
#define OBJECT_TEST_F_MACRO(object_field, object_id, requested_value) \
  if(conf->used[object_id] == true)                                   \
     if ( fabsf(object_field-requested_value) < ( (fabsf(object_field) < fabsf(requested_value) ? fabsf(requested_value) : fabsf(object_field)) * FLT_EPSILON) )   {         \
        printf("  Object id: %i, received value: %f, espected value: %f\n", object_id, object_field, requested_value); \
        return false;                          \
     }                                        \
  else return false;
   
//----------------------------------------------
   
bool verify_conf(configuration* conf)
{
  bool success = false;
  
  conf->print();

  OBJECT_TEST_MACRO(conf->dst,           OBJECT_DST_ADDR,       inet_addr("1.2.3.4"));
  OBJECT_TEST_MACRO(conf->src,           OBJECT_SRC_ADDR,       inet_addr("5.6.7.8"));
  OBJECT_TEST_MACRO(conf->out_if_addr,   OBJECT_OUT_ADDR,       inet_addr("9.10.11.12"));
  OBJECT_TEST_MACRO(conf->refresh,       OBJECT_REFRESH,        1);
  //OBJECT_TEST_F_MACRO(conf->tb_rate,       OBJECT_TB_RATE,        2.1);
  //OBJECT_TEST_F_MACRO(conf->tb_size,       OBJECT_TB_SIZE,        3.2);
  //OBJECT_TEST_F_MACRO(conf->peak_rate,     OBJECT_PEAK,           4.3);
  OBJECT_TEST_MACRO(conf->up_label,      OBJECT_UP_LABEL,       5);
  OBJECT_TEST_MACRO(conf->enc,           OBJECT_LSP_ENC,        6);
  OBJECT_TEST_MACRO(conf->switch_type,   OBJECT_LSP_SW,         7);
  OBJECT_TEST_MACRO(conf->gpid,          OBJECT_LSP_GPID,       8);

  if(conf->used[OBJECT_PATH_NAME] == true)
  {                                            
     if(strcmp(conf->name,"qwertyuiop") != 0)       
        return false;                      
  }                                            
  else return false; 

  OBJECT_TEST_MACRO(conf->ero,           OBJECT_ERO_ADDR,       inet_addr("13.14.15.16"));
  OBJECT_TEST_MACRO(conf->ero_pref_len,  OBJECT_ERO_PREF,       9);
  OBJECT_TEST_MACRO(conf->ero_sl,        OBJECT_ERO_SL,         true);
  OBJECT_TEST_MACRO(conf->hop,           OBJECT_HOP_ADDR,       inet_addr("17.18.19.20"));
  OBJECT_TEST_MACRO(conf->err_node,      OBJECT_ERROR_ADDR,     inet_addr("21.22.23.24"));
  OBJECT_TEST_MACRO(conf->err_flag,      OBJECT_ERROR_FLAGS,    10);
  OBJECT_TEST_MACRO(conf->err_code,      OBJECT_ERROR_CODE,     11);
  OBJECT_TEST_MACRO(conf->err_val,       OBJECT_ERROR_VALUE,    12);
  OBJECT_TEST_MACRO(conf->mpart_number,  OBJECT_MULTI_NUM,      13);
  OBJECT_TEST_MACRO(conf->mpart_count,   OBJECT_MULTI_COUNT,    14);
  OBJECT_TEST_MACRO(conf->tun_id,        OBJECT_TUNNEL_ID,      15);
  OBJECT_TEST_MACRO(conf->traff_dst,     OBJECT_TR_RD_ADDR,     inet_addr("24.25.26.27"));
  OBJECT_TEST_MACRO(conf->traff_rev_dst, OBJECT_TR_D_ADDR,      inet_addr("28.29.30.31"));
  OBJECT_TEST_MACRO(conf->interface_id,  OBJECT_OUT_IF_HANDLE,  16);
  OBJECT_TEST_MACRO(conf->src_tna,       OBJECT_SRC_TNA,        inet_addr("32.33.34.35"));
  OBJECT_TEST_MACRO(conf->dst_tna,       OBJECT_DST_TNA,        inet_addr("36.37.38.39"));
  OBJECT_TEST_MACRO(conf->call_id_1,     OBJECT_CALL_ID_1,      17);
  OBJECT_TEST_MACRO(conf->call_id_2,     OBJECT_CALL_ID_2,      18);
  OBJECT_TEST_MACRO(conf->lsp_id,        OBJECT_SENDER_LSP_ID,  19);
  OBJECT_TEST_MACRO(conf->setup_prior,   OBJECT_SETUP_PRIOR,    20);
  OBJECT_TEST_MACRO(conf->hold_prior,    OBJECT_HOLD_PRIOR,     21);
  OBJECT_TEST_MACRO(conf->path_state,    OBJECT_PATH_STATE,     22);
  OBJECT_TEST_MACRO(conf->serv_level,    OBJECT_PATH_SERV_LEVEL,23);
  //OBJECT_TEST_F_MACRO(conf->eth_cir,       OBJECT_ETHERNET_CIR,   24.24);
  //OBJECT_TEST_F_MACRO(conf->eth_cbs,       OBJECT_ETHERNET_CBS,   25.25);
  //OBJECT_TEST_F_MACRO(conf->eth_eir,       OBJECT_ETHERNET_EIR,   26.26);
  //OBJECT_TEST_MACRO(conf->eth_ebs,       OBJECT_ETHERNET_EBS,   27.27);
  OBJECT_TEST_MACRO(conf->eth_granuality,OBJECT_ETHERNET_SW_GRANUALITY, 28);
  OBJECT_TEST_MACRO(conf->eth_index,     OBJECT_ETHERNET_INDEX, 29);
  OBJECT_TEST_MACRO(conf->eth_profile,   OBJECT_ETHERNET_PROFILE, 30);

  return true;
};

//--------------------------------------------------------------------------------------------------

class agent_api_child : public agent_api
{
public:
    agent_api_child(const char* addr, unsigned short port=35000) : agent_api((const char*)addr,port) {};

    void receive_message(unsigned char command, unsigned char message_type, configuration* conf, unsigned short message_id)
    {
          printf("Received message: %d, %d, %d\n", command, message_type, message_id);
          if(message_type == TYPE_REQ)
          {
              printf("Received new request\n");
              printf("Send ack\n");
              send_message(command, TYPE_ACK, conf, message_id);
          }
          if(message_type == TYPE_ACK)
              printf("Received request ack\n");
          if(message_type == TYPE_NACK)
              printf("Received request nack\n");
          conf->print();
    };
};

//--------------------------------------------------------------------------------------------------

void test1()
{
    printf("START OF TEST 1\n");
    configuration* conf = prepare_conf();
    conf->print();

    mgmt_packet* mgmt = new mgmt_packet(conf, 0,0,0);
    int size = 0;
    char* data = (char*)mgmt->get_data(&size);

    mgmt_packet* mgmt2 = new mgmt_packet(data, size);
    configuration conf2 = mgmt2->get_conf();
    if(verify_conf(&conf2))
        printf("TEST 1 IS PASSED SUCCESSFULLY\n");
    else
        printf("TEST 1 IS FAILED\n");
}

//--------------------------------------------------------------------------------------------------

void test2()
{
    printf("START OF TEST 2\n");

    configuration* conf = new configuration();
    conf->tun_id = 2;
    conf->dst = inet_addr("10.136.136.7"),35000;
    conf->used[OBJECT_TUNNEL_ID] = true;
    conf->used[OBJECT_DST_ADDR] = true;

    int request_id;
    
    agent_api_child api((const char*)"150.254.160.215");

    request_id = api.send_message(COMMAND_CREATE, TYPE_REQ, conf);
    if(request_id >= 0) printf("Create request is sent with id: %d\n", request_id);
    else printf("Request sent is unsuccessful\n");

    sleep(10);

    request_id = api.send_message(COMMAND_STATUS, TYPE_REQ, conf);
    if(request_id >= 0) printf("Status request is sent with id: %d\n", request_id);
    else printf("Request sent is unsuccessful\n");

    sleep(10);

    request_id = api.send_message(COMMAND_DELETE, TYPE_REQ, conf);
    if(request_id >= 0) printf("Delete request is sent with id: %d\n", request_id);
    else printf("Request sent is unsuccessful\n");
    
    sleep(10);
    
    api.api_disconnect();

    printf("END OF TEST 2\n");

}

//--------------------------------------------------------------------------------------------------

void test3()
{
    printf("START OF TEST 3\n");
    agent_api_child api((const char*)"150.254.160.215");

    while(true)
        sleep(10);

    printf("END OF TEST 3\n");
}

//--------------------------------------------------------------------------------------------------

void test4()
{
    printf("START OF TEST 4\n");
    configuration* conf = prepare_conf();
    
    agent_api_child api((const char*)"150.254.160.215");

    int request_id = api.send_message(COMMAND_CREATE, TYPE_REQ, conf);
    if(request_id >= 0) printf("Create request is sent with id: %d\n", request_id);
    else printf("Request sent is unsuccessful\n");
    
    sleep(10);
    
    api.api_disconnect();

    printf("END OF TEST 4\n");
}

//===================================================================================================
int main(int argc, char *argv[])
{
    printf("start of testing!\n");
    test1();
    //test2();
    //test3();
    //test4();
    printf("end of testing!\n");
    return EXIT_SUCCESS;

}
