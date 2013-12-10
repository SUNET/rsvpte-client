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
 
#ifndef RSVP_PACKET_BODY_FILE
#define RSVP_PACKET_BODY_FILE

#include "rsvp_packet.h"

extern rsvp_globals globals;

rsvp_packet::rsvp_packet()
{
    int t;
    packet_data = NULL;
    packet_data_size = 0;
    configuration = NULL;
    for(t=0;t<MAX_OBJECTS;t++)
    {
        objects[t].cnum = 0;
        objects[t].ctnum = 0;
        objects[t].scnum = 0;
        objects[t].sctnum = 0;
    }
    infinity = htonl(0x7f800000);
    for(t=0;t<MAX_REQS;t++)
    {
        requirements[t].cnum = 0;
        requirements[t].ctnum = 0;
        requirements[t].scnum = 0;
        requirements[t].sctnum = 0;
        requirements[t].req = NULL;
        requirements[t].req_count = 0;
        requirements[t].and_or = 0;
    }
    for(t=0;t<NUM_OBJECTS;t++)
    {
        objects_order[t].cnum = 0;
        objects_order[t].ctnum = 0;
        objects_order[t].scnum = 0;
        objects_order[t].sctnum = 0;
    }
    add_req(CLASS_SESSION_IETF, RC_SESSION_DEST_ADDR);  // ip tunnel end point address
    add_req(CLASS_SESSION_IETF, RC_TUNNEL_ID);			
    add_req(CLASS_SESSION_IETF, RC_EXTENDED_IPV4_ADDR); // extended tunel id

    add_req(CLASS_SESSION_OIF, RC_SESSION_DEST_ADDR);
    add_req(CLASS_SESSION_OIF, RC_TUNNEL_ID);
    add_req(CLASS_SESSION_OIF, RC_EXTENDED_IPV4_ADDR);

    add_req(CLASS_IPV4_RSVP_HOP, RC_CONTROL_OUT_IF_ADDR);	// ip hop address
    add_req(CLASS_IPV4_RSVP_HOP, RC_CONTROL_OUT_IF_HANDLE);	// interface handle

//    add_req(CLASS_IPV4_IF_ID_RSVP_HOP, RC_RSVP_HOP_TLVS);
    add_req(CLASS_IPV4_IF_ID_RSVP_HOP, RC_DATA_OUT_IF_ADDR); // data channel ip address
    add_req(CLASS_IPV4_IF_ID_RSVP_HOP, RC_CONTROL_OUT_IF_ADDR);
    add_req(CLASS_IPV4_IF_ID_RSVP_HOP, RC_CONTROL_OUT_IF_HANDLE);

    add_req(CLASS_TIME_VALUES, RC_REFRESH_PERIOD);

    add_req(CLASS_ERROR_SPEC, RC_ERROR_NODE_ADDR);
    add_req(CLASS_ERROR_SPEC, RC_ERROR_FLAGS);
    add_req(CLASS_ERROR_SPEC, RC_ERROR_CODE);
    add_req(CLASS_ERROR_SPEC, RC_ERROR_VALUE);

    add_req(CLASS_SCOPE, RC_SCOPE_ADDR);

    add_req(CLASS_STYLE, RC_WILDCARD_FILTER, 1);
    add_req(CLASS_STYLE, RC_FIXED_FILTER, 1);
    add_req(CLASS_STYLE, RC_SHARED_EXPLICIT, 1);

    add_req(CLASS_TOKEN_FLOWSPEC, RC_TOKEN_BUCKET_RATE);
    add_req(CLASS_TOKEN_FLOWSPEC, RC_TOKEN_BUCKET_SIZE);
    add_req(CLASS_TOKEN_FLOWSPEC, RC_PEAK_DATA_RATE);
    add_req(CLASS_TOKEN_FLOWSPEC, RC_MINIMUM_POLICED_UNIT);
    add_req(CLASS_TOKEN_FLOWSPEC, RC_MAXIMUM_PACKET_SIZE);

    add_req(CLASS_SONETSDH_FLOWSPEC, RC_SSDH_SIGNAL_TYPE);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_SSDH_RCC);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_SSDH_NCC);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_SSDH_NVC);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_SSDH_MULTIPLIER);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_SSDH_TRANSPARENCY);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_SSDH_PROFILE);
    
    add_req(CLASS_ETHERNET_FLOWSPEC, RC_ETHERNET_CIR);
    add_req(CLASS_ETHERNET_FLOWSPEC, RC_ETHERNET_CBS);
    add_req(CLASS_ETHERNET_FLOWSPEC, RC_ETHERNET_EIR);
    add_req(CLASS_ETHERNET_FLOWSPEC, RC_ETHERNET_EBS);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_ETHERNET_SW_GRANUALITY);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_ETHERNET_INDEX);
    add_req(CLASS_SONETSDH_FLOWSPEC, RC_ETHERNET_PROFILE);

    add_req(CLASS_FILTER_SPEC, RC_FILTER_SRC_ADDR); 
    add_req(CLASS_FILTER_SPEC, RC_FILTER_SRC_PORT); 

    add_req(CLASS_SENDER_TEMPLATE, RC_SENDER_SRC_ADDR); // tunnel sender address
    add_req(CLASS_SENDER_TEMPLATE, RC_SENDER_SRC_ID);   // LSP id

    add_req(CLASS_TOKEN_SENDER_TSPEC, RC_TOKEN_BUCKET_RATE);
    add_req(CLASS_TOKEN_SENDER_TSPEC, RC_TOKEN_BUCKET_SIZE);
    add_req(CLASS_TOKEN_SENDER_TSPEC, RC_PEAK_DATA_RATE);
    add_req(CLASS_TOKEN_SENDER_TSPEC, RC_MINIMUM_POLICED_UNIT);
    add_req(CLASS_TOKEN_SENDER_TSPEC, RC_MAXIMUM_PACKET_SIZE);

    add_req(CLASS_SONETSDH_SENDER_TSPEC, RC_SSDH_SIGNAL_TYPE);
    add_req(CLASS_SONETSDH_SENDER_TSPEC, RC_SSDH_RCC);
    add_req(CLASS_SONETSDH_SENDER_TSPEC, RC_SSDH_NCC);
    add_req(CLASS_SONETSDH_SENDER_TSPEC, RC_SSDH_NVC);
    add_req(CLASS_SONETSDH_SENDER_TSPEC, RC_SSDH_MULTIPLIER);
    add_req(CLASS_SONETSDH_SENDER_TSPEC, RC_SSDH_TRANSPARENCY);
    add_req(CLASS_SONETSDH_SENDER_TSPEC, RC_SSDH_PROFILE);
    
    add_req(CLASS_ETHERNET_SENDER_TSPEC, RC_ETHERNET_CIR);
    add_req(CLASS_ETHERNET_SENDER_TSPEC, RC_ETHERNET_CBS);
    add_req(CLASS_ETHERNET_SENDER_TSPEC, RC_ETHERNET_EIR);
    add_req(CLASS_ETHERNET_SENDER_TSPEC, RC_ETHERNET_EBS);
    add_req(CLASS_ETHERNET_SENDER_TSPEC, RC_ETHERNET_SW_GRANUALITY);
    add_req(CLASS_ETHERNET_SENDER_TSPEC, RC_ETHERNET_INDEX);
    add_req(CLASS_ETHERNET_SENDER_TSPEC, RC_ETHERNET_PROFILE);

    add_req(CLASS_RESV_CONFIRM, RC_RESV_CONFIRM_RECV_ADDR);

    add_req(CLASS_GENERALIZED_LABEL, RC_LABEL);

    add_req(CLASS_GENERALIZED_LABEL_REQUEST, RC_LSP_ENC_TYPE);
    add_req(CLASS_GENERALIZED_LABEL_REQUEST, RC_LSP_SWITCHING_TYPE);
    add_req(CLASS_GENERALIZED_LABEL_REQUEST, RC_LSP_GPID);

    add_req(CLASS_ERO, RC_ERO);

    add_req(CLASS_HELLO_REQUEST, RC_HELLO_SRC_INSTANCE);
    add_req(CLASS_HELLO_REQUEST, RC_HELLO_DST_INSTANCE);

    add_req(CLASS_HELLO_ACK, RC_HELLO_SRC_INSTANCE);
    add_req(CLASS_HELLO_ACK, RC_HELLO_DST_INSTANCE);

    add_req(CLASS_MESSAGE_ID, RC_MSG_ID_FLAGS);
    add_req(CLASS_MESSAGE_ID, RC_MSG_ID_EPOCH);
    add_req(CLASS_MESSAGE_ID, RC_MSG_ID_ID);

    add_req(CLASS_MESSAGE_ID_ACK, RC_MSG_ID_FLAGS);
    add_req(CLASS_MESSAGE_ID_ACK, RC_MSG_ID_EPOCH);
    add_req(CLASS_MESSAGE_ID_ACK, RC_MSG_ID_ID);

    add_req(CLASS_MESSAGE_ID_NACK, RC_MSG_ID_FLAGS);
    add_req(CLASS_MESSAGE_ID_NACK, RC_MSG_ID_EPOCH);
    add_req(CLASS_MESSAGE_ID_NACK, RC_MSG_ID_ID);

    add_req(CLASS_UPSTREAM_LABEL, RC_UPSTREAM_LABEL);

    add_req(CLASS_RESTART_CAP, RC_RESTART_TIME);
    add_req(CLASS_RESTART_CAP, RC_RECOVERY_TIME);

    add_req(CLASS_ADMIN_STATUS, RC_ADMIN_STATUS_RTAD_FLAGS);

    add_req(CLASS_SESSION_ATTRIBUTE, RC_SESSION_NAME);
    add_req(CLASS_SESSION_ATTRIBUTE, RC_SETUP_PRIORITY);
    add_req(CLASS_SESSION_ATTRIBUTE, RC_HOLDING_PRIORITY);
    add_req(CLASS_SESSION_ATTRIBUTE, RC_LOCAL_PROTECTION_FLAG);
    add_req(CLASS_SESSION_ATTRIBUTE, RC_LABEL_RECORDING_FLAG);
    add_req(CLASS_SESSION_ATTRIBUTE, RC_SE_STYLE_FLAG);

    add_req(CLASS_GENERALIZED_UNI, RC_TNA_SRC_ADDR, 1);
    add_req(CLASS_GENERALIZED_UNI, RC_TNA_DST_ADDR, 1);
    add_req(CLASS_GENERALIZED_UNI, RC_SERVICE_LEVEL, 1);

    add_req(CLASS_GU_SOURCE_TNA_ADDRESS,      RC_TNA_SRC_ADDR);
    add_req(CLASS_GU_DESTINATION_TNA_ADDRESS, RC_TNA_DST_ADDR);
    add_req(CLASS_GU_SERVICE_LEVEL,           RC_SERVICE_LEVEL);

    add_req(CLASS_CALL_ID_0, RC_CALL_ID);

    add_req(CLASS_CALL_ID_1, RC_CALL_ID_ADDR_TYPE);
    add_req(CLASS_CALL_ID_1, RC_CALL_ID_SRC_TN_ADDR);
    add_req(CLASS_CALL_ID_1, RC_CALL_ID_LOCAL_ID1);
    add_req(CLASS_CALL_ID_1, RC_CALL_ID_LOCAL_ID2);

    add_req(CLASS_CALL_ID_2, RC_CALL_ID_ADDR_TYPE);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_IS);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_ITU_CC1);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_ITU_CC2);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_UAPC1);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_UAPC2);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_SRC_TN_ADDR);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_LOCAL_ID1);
    add_req(CLASS_CALL_ID_2, RC_CALL_ID_LOCAL_ID2);
    
    add_req(CLASS_MESSAGE_ID_LIST, RC_SREFRESH_MESSAGE_ID);
    add_req(CLASS_NOTIFY_REQUEST, RC_NOTIFY_ADDRESS);

    add_order(CLASS_MESSAGE_ID_ACK);
    add_order(CLASS_MESSAGE_ID_NACK);
    add_order(CLASS_MESSAGE_ID);
    add_order(CLASS_MESSAGE_ID_LIST);
    add_order(CLASS_HELLO_REQUEST);
    add_order(CLASS_HELLO_ACK);
    add_order(CLASS_RESTART_CAP);
    add_order(CLASS_SESSION_OIF);
    add_order(CLASS_SESSION_IETF);
    add_order(CLASS_IPV4_IF_ID_RSVP_HOP);
    add_order(CLASS_IPV4_RSVP_HOP);
    add_order(CLASS_TIME_VALUES);
    add_order(CLASS_GENERALIZED_LABEL_REQUEST);
    add_order(CLASS_SESSION_ATTRIBUTE);
    add_order(CLASS_ERROR_SPEC);
    add_order(CLASS_NOTIFY_REQUEST);
    add_order(CLASS_CALL_ID_0);
    add_order(CLASS_CALL_ID_1);
    add_order(CLASS_CALL_ID_2);
    add_order(CLASS_RESV_CONFIRM);
    add_order(CLASS_ADMIN_STATUS);
    add_order(CLASS_GENERALIZED_UNI);
    add_order(CLASS_GU_DESTINATION_TNA_ADDRESS);
    add_order(CLASS_GU_SOURCE_TNA_ADDRESS);
    add_order(CLASS_GU_SERVICE_LEVEL);
    add_order(CLASS_SENDER_TEMPLATE);
    add_order(CLASS_SONETSDH_SENDER_TSPEC);
    add_order(CLASS_TOKEN_SENDER_TSPEC);
    add_order(CLASS_ETHERNET_SENDER_TSPEC);
    add_order(CLASS_UPSTREAM_LABEL);
    add_order(CLASS_STYLE);
    add_order(CLASS_TOKEN_FLOWSPEC);
    add_order(CLASS_SONETSDH_FLOWSPEC);
    add_order(CLASS_ETHERNET_FLOWSPEC);
    add_order(CLASS_FILTER_SPEC);
    add_order(CLASS_GENERALIZED_LABEL);

    add_order(CLASS_SCOPE);  // order is unknown right now (at least for me)
    add_order(CLASS_ERO);

}

rsvp_packet::~rsvp_packet()
{
    free(packet_data);
    free(configuration);
}

void rsvp_packet::add_req(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum,char* obj_desc, unsigned char prop_num, int and_or)
{
    int t;
    t = 0;
    while(t < MAX_REQS)
    {
        if(requirements[t].cnum == cnum && requirements[t].ctnum == ctnum && requirements[t].scnum == scnum && requirements[t].sctnum == sctnum)
        {
            requirements[t].req = (unsigned char*)realloc(requirements[t].req, requirements[t].req_count+1);
            requirements[t].req[requirements[t].req_count] = prop_num;
            requirements[t].req_count++;
            requirements[t].and_or = and_or;
            return;
        }
        if(requirements[t].cnum == 0 && requirements[t].ctnum == 0 && requirements[t].scnum == 0 && requirements[t].sctnum == 0)
        {
            requirements[t].cnum = cnum;
            requirements[t].ctnum = ctnum;
            requirements[t].scnum = scnum;
            requirements[t].sctnum = sctnum;
            requirements[t].req = (unsigned char*)realloc(requirements[t].req, 1);
            requirements[t].req[0] = prop_num;
            requirements[t].req_count = 1;
            requirements[t].and_or = and_or;
            return;
        }
        t++;
    }
    return;
}

void rsvp_packet::add_order(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum, char* obj_desc)
{
    int t;
    for(t=0;t<NUM_OBJECTS;t++)
    {
        if(objects_order[t].cnum == 0)
        {
            objects_order[t].cnum = cnum;
            objects_order[t].ctnum = ctnum;
            objects_order[t].scnum = scnum;
            objects_order[t].sctnum = sctnum;
            return;
        }
    }
    return;
}

int rsvp_packet::check_addable(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum)
{
    int t, x, w;
    t = x = w = 0;
    while(t < MAX_REQS)
    {
        if(requirements[t].cnum == cnum && requirements[t].ctnum == ctnum && requirements[t].scnum == scnum && requirements[t].sctnum == sctnum)
        {
            for(x=0;x<requirements[t].req_count;x++)
                if(configuration->ispresent(requirements[t].req[x]) == VAL_TRUE)
                    w++;
            if(requirements[t].and_or == 0)
            {
                if(w == requirements[t].req_count)
                    return VAL_TRUE;
                else
                    return VAL_FALSE;
            }
            else
            {
                if(w > 0)
                    return VAL_TRUE;
                else
                    return VAL_FALSE;
            }
        }
        t++;
    }
    return ERROR_VAL_NOT_PRES;
}

int rsvp_packet::get_object_order(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum)
{
    int t;
    for(t=0;t<NUM_OBJECTS;t++)
    {
        if(objects_order[t].cnum == cnum && objects_order[t].ctnum == ctnum && objects_order[t].scnum == scnum && objects_order[t].sctnum == sctnum)
            return t;
    }
    return -1;
}

int rsvp_packet::add_int(unsigned int value)
{
    unsigned int val;
    unsigned char* buf;
    val = htonl(value);
    buf = (unsigned char*)realloc(packet_data, packet_data_size+sizeof(unsigned int));
    if(buf == NULL)
        return ERROR_MALLOC;
    else
        packet_data = buf;
    memcpy(packet_data+packet_data_size, &val, sizeof(unsigned int));
    packet_data_size += sizeof(unsigned int);
    return 0;
}

int rsvp_packet::add_addr(unsigned int value)
{
    unsigned int val;
    unsigned char* buf;
    val = value;
    buf = (unsigned char*)realloc(packet_data, packet_data_size+sizeof(unsigned int));
    if(buf == NULL)
        return ERROR_MALLOC;
    else
        packet_data = buf;
    memcpy(packet_data+packet_data_size, &val, sizeof(unsigned int));
    packet_data_size += sizeof(unsigned int);
    return 0;
}

int rsvp_packet::add_short(unsigned short value)
{
    unsigned short val;
    unsigned char* buf;
    val = htons(value);
    buf = (unsigned char*)realloc(packet_data, packet_data_size+sizeof(unsigned short));
    if(buf == NULL)
        return ERROR_MALLOC;
    else
        packet_data = buf;
    memcpy(packet_data+packet_data_size, &val, sizeof(unsigned short));
    packet_data_size += sizeof(unsigned short);
    return 0;
}

int rsvp_packet::add_char(unsigned char value)
{
    unsigned char* buf;
    buf = (unsigned char*)realloc(packet_data, packet_data_size+sizeof(unsigned char));
    if(buf == NULL)
        return ERROR_MALLOC;
    else
        packet_data = buf;
    memcpy(packet_data+packet_data_size, &value, sizeof(unsigned char));
    packet_data_size += sizeof(unsigned char);
    return 0;
}

int rsvp_packet::add_chars(char* value, int count)
{
    unsigned char* buf;
    int len;
    if(count != -1)
        len = count;
    else
    {
        len = strlen(value);
        if(len<0)
            return ERROR_STRLEN;
    }
    buf = (unsigned char*)realloc(packet_data, packet_data_size+(len*sizeof(char)));
    if(buf == NULL)
        return ERROR_MALLOC;
    else
        packet_data = buf;
    memcpy(packet_data+packet_data_size, value, (len*sizeof(char)));
    packet_data_size += (len*sizeof(char));
    return 0;
}

int rsvp_packet::add_float(float value)
{
    double val;
    unsigned char* buf;
    char val_buf[4];
    val = value;
    buf = (unsigned char*)realloc(packet_data, packet_data_size+4);
    if(buf == NULL)
        return ERROR_MALLOC;
    else
        packet_data = buf;
    if(isinf(val) != 0)
        memcpy(packet_data+packet_data_size, &infinity, 4);
    else
    {
        write_ieee_float(val_buf, value);
        memcpy(packet_data+packet_data_size, &val_buf, 4);
    }
    packet_data_size += 4;
    return 0;
}

int rsvp_packet::add_void(void* value, int size)
{
    unsigned char* buf;
    buf = (unsigned char*)realloc(packet_data, packet_data_size+size);
    if(buf == NULL)
        return ERROR_MALLOC;
    else
        packet_data = buf;
    memcpy(packet_data+packet_data_size, value, size);
    packet_data_size += size;
    return 0;
}

int rsvp_packet::set_short(unsigned short value, int offset)
{
    unsigned short val;
    unsigned char* buf;
    val = htons(value);
    if(packet_data_size+2 < offset)
        return ERROR_BUF_2_SMALL;
    memcpy(packet_data+offset, &val, sizeof(unsigned short));
    return 0;
}

unsigned int rsvp_packet::get_int(int* offset)
{
    *offset += sizeof(unsigned int);
    return ntohl(*(unsigned int*)(packet_data+(*offset)-sizeof(unsigned int)));
}

unsigned int rsvp_packet::get_addr(int* offset)
{
    *offset += sizeof(unsigned int);
    return *(unsigned int*)(packet_data+(*offset)-sizeof(unsigned int));
}

unsigned short rsvp_packet::get_short(int* offset)
{
    *offset += sizeof(unsigned short);
    return ntohs(*(unsigned short*)(packet_data+(*offset)-sizeof(unsigned short)));
}

unsigned char rsvp_packet::get_char(int* offset)
{
    *offset += sizeof(unsigned char);
    return *(unsigned char*)(packet_data+(*offset)-sizeof(unsigned char));
}

char* rsvp_packet::get_chars(int* offset, int count)
{
    return (char*)get_void(offset, count*sizeof(char));
}

float rsvp_packet::get_float(int* offset)
{
    char buf[4];
    float val;
    memcpy(buf, packet_data+(*offset), 4);
    *offset += 4;
    if(memcmp(&infinity, buf, 4) == 0)
        return (float)INFINITY;
    else
    {
        val = read_ieee_float(buf);
        return val;
    }
}

void* rsvp_packet::get_void(int* offset, int size)
{
    char* buf;
    buf = (char*)malloc(size);
    memcpy(buf, packet_data+(*offset), size);
    *offset += size;
    return buf;
}


int rsvp_packet::parse_1_7_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_SESSION_DEST_ADDR, get_addr(offs))) != 0) return t;
    get_short(offs);
    if((t = configuration->add(RC_TUNNEL_ID, get_short(offs))) != 0) return t;
    if((configuration->add(RC_EXTENDED_IPV4_ADDR, get_addr(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_1_11_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_SESSION_DEST_ADDR, get_addr(offs))) != 0) return t;
    get_short(offs);
    if((t = configuration->add(RC_TUNNEL_ID, get_short(offs))) != 0) return t;
    if((configuration->add(RC_EXTENDED_IPV4_ADDR, get_addr(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_3_1_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_CONTROL_OUT_IF_ADDR, get_addr(offs))) != 0) return t;
    if((t = configuration->add(RC_CONTROL_OUT_IF_HANDLE, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_3_3_0_0(int offset, int data_size)
{
    int offs[1];
    int t;
    unsigned short a, b;
    offs[0] = offset;
    if((t = configuration->add(RC_CONTROL_OUT_IF_ADDR, get_addr(offs))) != 0) return t;
    if((t = configuration->add(RC_CONTROL_OUT_IF_HANDLE, get_int(offs))) != 0) return t;
    while(offs[0] < offset + data_size)
    {
        a = get_short(offs);
        b = get_short(offs);
        VERBOSE(7, "a == %d, b == %d, data_size = %d\n", a, b, data_size);
        if(a == 1 && b == 8)
        {
            if((t = configuration->add(RC_DATA_OUT_IF_ADDR, get_addr(offs))) != 0) return t;
        }
        if((a == 3 || a == 4) && b == 12)
        {
            VERBOSE(7, "adding RC_DATA_OUT_IF_ADDR and RC_DATA_OUT_IF_HANDLE\n");
            if((t = configuration->add(RC_DATA_OUT_IF_ADDR, get_addr(offs))) != 0) return t;
            if((t = configuration->add(RC_DATA_OUT_IF_HANDLE, get_int(offs))) != 0) return t;
            VERBOSE(7, "added\n");
        }
        if(a == 5 && b == 12)
        {
            VERBOSE(7, "adding RC_DATA_IN_IF_ADDR and RC_DATA_IN_IF_HANDLE\n");
            if((t = configuration->add(RC_DATA_IN_IF_ADDR, get_addr(offs))) != 0) return t;
            if((t = configuration->add(RC_DATA_IN_IF_HANDLE, get_int(offs))) != 0) return t;
            VERBOSE(7, "added\n");
        }
        if(a != 4 && a != 5 && a != 1)
        {
            for(t=0;t<b-4;t++)
                get_char(offs);
        }
    }
    return 0;
}

int rsvp_packet::parse_5_1_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_REFRESH_PERIOD, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_6_1_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_ERROR_NODE_ADDR, get_addr(offs))) != 0) return t;
    if((t = configuration->add(RC_ERROR_FLAGS, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_ERROR_CODE, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_ERROR_VALUE, get_short(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_7_1_0_0(int offset, int data_size)
{
    int offs[1];
    int t;
    offs[0] = offset;
    while(offs[0] < offset + data_size)
    {
        if((t = configuration->add(RC_SCOPE_ADDR, get_addr(offs))) != 0) return t;
    }
    return 0;
}

int rsvp_packet::parse_8_1_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned short buf;
    unsigned char flags, opt_vect;
    offs[0] = offset;
    flags = get_char(offs); // although, none assigned yet [RFC2205]
    buf = get_short(offs);
    opt_vect = get_char(offs);
    if(opt_vect == 0x11)
    {
        if((t = configuration->add(RC_WILDCARD_FILTER, (unsigned char)1)) != 0) return t;
        return 0;
    }
    if(opt_vect = 0x0A)
    {
        if((t = configuration->add(RC_FIXED_FILTER, (unsigned char)1)) != 0) return t;
        return 0;
    }
    if(opt_vect = 0x12)
    {
        if((t = configuration->add(RC_SHARED_EXPLICIT, (unsigned char)1)) != 0) return t;
        return 0;
    }
    if((t = configuration->add(RC_WILDCARD_FILTER, (unsigned char)1)) != 0) return t;
    if((t = configuration->add(RC_FIXED_FILTER, (unsigned char)1)) != 0) return t;
    if((t = configuration->add(RC_SHARED_EXPLICIT, (unsigned char)1)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_9_2_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned short a, b, c, d, ef, g, j;
    unsigned char h, i;
    offs[0] = offset;
    a = get_short(offs);
    b = get_short(offs);
    c = get_short(offs);
    d = get_short(offs);
    ef = get_short(offs);
    g = get_short(offs);
    if(a == 0x0000 && b == 7 && c == 0x0500 && d == 6 && ef == 0x7f00 && g == 5) // FLOWSPEC with Controlled-Load service
    {
        if((t = configuration->add(RC_TOKEN_BUCKET_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_TOKEN_BUCKET_SIZE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_PEAK_DATA_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_MINIMUM_POLICED_UNIT, get_int(offs))) != 0) return t;
        if((t = configuration->add(RC_MAXIMUM_PACKET_SIZE, get_int(offs))) != 0) return t;
        return 0;
    }
    if(a == 0x0000 && b == 10 && c == 0x0200 && d == 9 && ef == 0x7f00 && g == 5) // FLOWSPEC with Guaranteed service
    {
        if((t = configuration->add(RC_TOKEN_BUCKET_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_TOKEN_BUCKET_SIZE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_PEAK_DATA_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_MINIMUM_POLICED_UNIT, get_int(offs))) != 0) return t;
        if((t = configuration->add(RC_MAXIMUM_PACKET_SIZE, get_int(offs))) != 0) return t;
        h = get_char(offs);
        i = get_char(offs);
        j = get_short(offs);
        if(h == 130 && i == 0 && j == 2)
        {
            if((t = configuration->add(RC_RATE, get_float(offs))) != 0) return t;
            if((t = configuration->add(RC_SLACK_TERM, get_int(offs))) != 0) return t;
        }
        return 0;
    }
    return 0;
}

int rsvp_packet::parse_9_4_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_SSDH_SIGNAL_TYPE, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_RCC, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_NCC, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_NVC, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_MULTIPLIER, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_TRANSPARENCY, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_PROFILE, get_int(offs))) != 0) return t;
    return 0;
    return 0;
}

int rsvp_packet::parse_9_6_0_0(int offset)
{
    int offs[1];
    int t, type, length;
    offs[0] = offset;
    if((t = configuration->add(RC_ETHERNET_SW_GRANUALITY, get_short(offs))) != 0) return t;
    get_short(offs);         // MTU
    type = get_short(offs);  // Ethernet Bandwidth Profile (value: 129)
    length = get_short(offs);
    if(type == 129 && length == 24)
    {
        if((t = configuration->add(RC_ETHERNET_PROFILE, get_char(offs))) != 0) return t;
        if((t = configuration->add(RC_ETHERNET_INDEX, get_char(offs))) != 0) return t;
        get_short(offs);     // reserved bytes
        if((t = configuration->add(RC_ETHERNET_CIR, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_ETHERNET_CBS, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_ETHERNET_EIR, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_ETHERNET_EBS, get_float(offs))) != 0) return t;
    }
    return 0;
}

int rsvp_packet::parse_10_7_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_FILTER_SRC_ADDR, get_addr(offs))) != 0) return t;
    get_short(offs);
    if((t = configuration->add(RC_FILTER_SRC_PORT, get_short(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_11_7_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_SENDER_SRC_ADDR, get_addr(offs))) != 0) return t;
    get_short(offs);
    if((t = configuration->add(RC_SENDER_SRC_ID, get_short(offs))) != 0) return t;
}

int rsvp_packet::parse_12_2_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned short a, b, c, d, ef, g;
    offs[0] = offset;
    a = get_short(offs);
    b = get_short(offs);
    c = get_short(offs);
    d = get_short(offs);
    ef = get_short(offs);
    g = get_short(offs);
    VERBOSE(7, "parse_12_2_0_0: c is %x\n", c);
    if(a == 0x0000 && b == 7 && c == 0x0100 && d == 6 && ef == 0x7f00 && g == 5)
    {
        VERBOSE(7, "parse_12_2_0_0: c is 0x0100\n");
        if((t = configuration->add(RC_TOKEN_BUCKET_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_TOKEN_BUCKET_SIZE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_PEAK_DATA_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_MINIMUM_POLICED_UNIT, get_int(offs))) != 0) return t;
        if((t = configuration->add(RC_MAXIMUM_PACKET_SIZE, get_int(offs))) != 0) return t;
        return 0;
    }
    if(a == 0x0000 && b == 7 && c == 0x0500 && d == 6 && ef == 0x7f00 && g == 5)
    {
        VERBOSE(7, "parse_12_2_0_0: c is 0x0500\n");
        if((t = configuration->add(RC_TOKEN_BUCKET_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_TOKEN_BUCKET_SIZE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_PEAK_DATA_RATE, get_float(offs))) != 0) return t;
        if((t = configuration->add(RC_MINIMUM_POLICED_UNIT, get_int(offs))) != 0) return t;
        if((t = configuration->add(RC_MAXIMUM_PACKET_SIZE, get_int(offs))) != 0) return t;
        return 0;
    }
    return 0;
}

int rsvp_packet::parse_12_4_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_SSDH_SIGNAL_TYPE, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_RCC, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_NCC, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_NVC, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_MULTIPLIER, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_TRANSPARENCY, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_SSDH_PROFILE, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_12_6_0_0(int offset)
{
    return parse_9_6_0_0(offset);
}

int rsvp_packet::parse_15_1_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_RESV_CONFIRM_RECV_ADDR, get_addr(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_16_2_0_0(int offset, int data_size)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_LABEL, get_void(offs, data_size), data_size)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_19_4_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_LSP_ENC_TYPE, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_LSP_SWITCHING_TYPE, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_LSP_GPID, get_short(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_20_1_0_0(int offset, int data_size)
{
    int offs[1];
    int t;
    unsigned char l_type, len;
    struct rc_ero obj;
    offs[0] = offset;
    while(offs[0] < offset + data_size)
    {
        l_type = get_char(offs);
        len = get_char(offs);
        if((l_type & 0x7f) == 1 && len == 8)
        {
            obj.address = get_addr(offs);
            obj.prefix_length = get_char(offs);
            if((l_type & 0x80) != 0)
                obj.loose = VAL_TRUE;
            else
                obj.loose = VAL_FALSE;
            if((t = configuration->add(RC_ERO, &obj, sizeof(obj))) != 0) return t;
            get_char(offs);
        }
        else
            offs[0] += len - 2;
    }
    return 0;
}

int rsvp_packet::parse_22_1_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_HELLO_SRC_INSTANCE, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_HELLO_DST_INSTANCE, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_HELLO_REQ_ACK, (unsigned char)1)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_22_2_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_HELLO_SRC_INSTANCE, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_HELLO_DST_INSTANCE, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_HELLO_REQ_ACK, (unsigned char)2)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_23_1_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned int x;
    unsigned char a, b, c;
    offs[0] = offset;
    if((t = configuration->add(RC_MSG_ID_FLAGS, get_char(offs))) != 0) return t;
    a = get_char(offs);
    b = get_char(offs);
    c = get_char(offs);
    x = ((unsigned int)a << 16) + ((unsigned int)b << 8) + c;
    if((t = configuration->add(RC_MSG_ID_EPOCH, x)) != 0) return t;
    if((t = configuration->add(RC_MSG_ID_ID, (x=get_int(offs)))) != 0) return t;
    globals.message_id = x+1;
    VERBOSE(7, "global_message_id is %d\n", globals.message_id);
    return 0;
}

int rsvp_packet::parse_24_1_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned int x;
    unsigned char a, b, c;
    offs[0] = offset;
    if((t = configuration->add(RC_MSG_ID_FLAGS, get_char(offs))) != 0) return t;
    a = get_char(offs);
    b = get_char(offs);
    c = get_char(offs);
    x = ((unsigned int)a << 16) + ((unsigned int)b << 8) + c;
    if((t = configuration->add(RC_MSG_ID_EPOCH, x)) != 0) return t;
    if((t = configuration->add(RC_MSG_ID_ID, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_MSG_ID_ACK_NACK, (unsigned char)1)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_24_2_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned int x;
    unsigned char a, b, c;
    offs[0] = offset;
    if((t = configuration->add(RC_MSG_ID_FLAGS, get_char(offs))) != 0) return t;
    a = get_char(offs);
    b = get_char(offs);
    c = get_char(offs);
    x = ((unsigned int)a << 16) + ((unsigned int)b << 8) + c;
    if((t = configuration->add(RC_MSG_ID_EPOCH, x)) != 0) return t;
    if((t = configuration->add(RC_MSG_ID_ID, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_MSG_ID_ACK_NACK, (unsigned char)2)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_25_1_0_0(int offset, int data_size)
{
    int offs[1];
    int t = 0;
    unsigned int x;
    unsigned char a, b, c;
    offs[0] = offset;
    get_int(offs); // get Flags (1 byte) and Epoch (3 bytes)   
    for(t=0; t<data_size; t=t+4)
       if((t = configuration->add(RC_SREFRESH_MESSAGE_ID, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_35_2_0_0(int offset, int data_size)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_UPSTREAM_LABEL, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_131_1_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_RECOVERY_TIME, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_195_1_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_NOTIFY_ADDRESS, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_196_1_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned char r, tad, v;
    v = 0;
    offs[0] = offset;
    r = get_char(offs);
    get_short(offs);
    tad = get_char(offs);
    v = v | ((r & 0x80) >> 4) | (tad & 0x07);
    if((t = configuration->add(RC_ADMIN_STATUS_RTAD_FLAGS, v)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_207_7_0_0(int offset)
{
    int offs[1];
    int t;
    unsigned char flags, nl;
    char* buf;
    offs[0] = offset;
    if((t = configuration->add(RC_SETUP_PRIORITY, get_char(offs))) != 0) return t;
    if((t = configuration->add(RC_HOLDING_PRIORITY, get_char(offs))) != 0) return t;
    flags = get_char(offs);
    if((flags & 0x01) != 0)
        if((t = configuration->add(RC_LOCAL_PROTECTION_FLAG, (unsigned char)1)) != 0) return t;
    if((flags & 0x02) != 0)
        if((t = configuration->add(RC_LABEL_RECORDING_FLAG, (unsigned char)1)) != 0) return t;
    if((flags & 0x04) != 0)
        if((t = configuration->add(RC_SE_STYLE_FLAG, (unsigned char)1)) != 0) return t;
    nl = get_char(offs);
    buf = get_chars(offs, nl);
    buf = (char*)realloc(buf, nl+1);
    if(buf == NULL)
        return ERROR_MALLOC;
    buf[nl] = 0;
    if((t = configuration->add(RC_SESSION_NAME, buf)) != 0) return t;
    return 0;
}

int rsvp_packet::parse_229_1_0_0(int offset, int data_size)
{
    int offs[1];
    int t;
    unsigned short ol;
    unsigned char ot, ost;
    offs[0] = offset;
    while(offs[0] < offset + data_size)
    {
        ol = get_short(offs);
        ot = get_char(offs);
        ost = get_char(offs);
        if(ot == 1 && ost == 1)
            if((t = parse_229_1_1_1(offs[0])) != 0)
                return t;
        if(ot == 2 && ost == 1)
            if((t = parse_229_1_2_1(offs[0])) != 0)
                return t;
        if(ot == 5 && ost == 1)
            if((t = parse_229_1_5_1(offs[0])) != 0)
               return t;
        offs[0] += ol - 4;
    }
    return 0;
}

int rsvp_packet::parse_229_1_1_1(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_TNA_SRC_ADDR, get_addr(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_229_1_2_1(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_TNA_DST_ADDR, get_addr(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_229_1_5_1(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_SERVICE_LEVEL, get_char(offs))) != 0) return t;
    get_char(offs);
    get_short(offs);
    return 0;
}

int rsvp_packet::parse_230_0_0_0(int offset)
{
    int offs[1];
    int t;
    offs[0] = offset;
    if((t = configuration->add(RC_CALL_ID, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_230_1_0_0(int offset, int data_size)
{
    int offs[1];
    int t, x;
    unsigned char addr_type;
    offs[0] = offset;
    if((t = configuration->add(RC_CALL_ID_ADDR_TYPE, addr_type = get_char(offs))) != 0) return t;
    get_char(offs);
    get_char(offs);
    get_char(offs);
    if(addr_type == 1)
    {
        if((t = configuration->add(RC_CALL_ID_SRC_TN_ADDR, get_addr(offs))) != 0) return t;
    }
    else
    {
        for(x=0;x<(data_size-12)/4;x++)
        {
            get_int(offs);                  //@todo if needed, rework it into saving just raw data (void*) without interpretation
        }
    }
    if((t = configuration->add(RC_CALL_ID_LOCAL_ID1, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_CALL_ID_LOCAL_ID2, get_int(offs))) != 0) return t;
    return 0;
}

int rsvp_packet::parse_230_2_0_0(int offset, int data_size)
{
    int offs[1];
    int t, x;
    unsigned char addr_type;
    unsigned int val;
    offs[0] = offset;
    if((t = configuration->add(RC_CALL_ID_ADDR_TYPE, addr_type = get_char(offs))) != 0) return t;
    val = get_char(offs) << 16;
    val = val + (get_char(offs) << 8);
    val = val + (get_char(offs));
    if((t = configuration->add(RC_CALL_ID_IS, val)) != 0) return t;
    if((t = configuration->add(RC_CALL_ID_ITU_CC1, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_CALL_ID_ITU_CC2, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_CALL_ID_UAPC1, get_short(offs))) != 0) return t;
    if((t = configuration->add(RC_CALL_ID_UAPC2, get_int(offs))) != 0) return t;
    if(addr_type == 1)
    {
        if((t = configuration->add(RC_CALL_ID_SRC_TN_ADDR, get_addr(offs))) != 0) return t;
    }
    else
    {
        for(x=0;x<(data_size-24)/4;x++)
        {
            get_int(offs);                  //@todo if needed, rework it into saving just raw data (void*) without interpretation
        }
    }
    if((t = configuration->add(RC_CALL_ID_LOCAL_ID1, get_int(offs))) != 0) return t;
    if((t = configuration->add(RC_CALL_ID_LOCAL_ID2, get_int(offs))) != 0) return t;
    return 0;
}


rsvp_configuration* rsvp_packet::parse_data(void* buffer, int size)
{
    int offs[1];
    unsigned short ol;
    unsigned char oc, oct;
    rsvp_configuration* ret_conf;

    ret_conf = NULL;
    free(configuration);
    configuration = new rsvp_configuration();
    free(packet_data);
    packet_data = (unsigned char*)malloc(size);
    if(packet_data == NULL)
        return NULL;
    memcpy(packet_data, buffer, size);
    packet_data_size = size;
    offs[0] = 0;

    while(offs[0] < packet_data_size)
    {
        ol = get_short(offs);
        oc = get_char(offs);
        oct = get_char(offs);
        switch(oc)
        {
        case 1:
            if(oct == 7)
                parse_1_7_0_0(offs[0]);
            if(oct == 11)
                parse_1_11_0_0(offs[0]);
            break;
        case 3:
            if(oct == 1)
                parse_3_1_0_0(offs[0]);
            if(oct == 3)
                parse_3_3_0_0(offs[0], ol - 4);
            break;
        case 5:
            if(oct == 1)
                parse_5_1_0_0(offs[0]);
            break;
        case 6:
            if(oct == 1)
                parse_6_1_0_0(offs[0]);
            break;
        case 7:
            if(oct == 1)
                parse_7_1_0_0(offs[0], ol - 4);
            break;
        case 8:
            if(oct == 1)
                parse_8_1_0_0(offs[0]);
            break;
        case 9:
            if(oct == 2)
                parse_9_2_0_0(offs[0]);
            if(oct == 4)
                parse_9_4_0_0(offs[0]);
            if(oct == 6)
                parse_9_6_0_0(offs[0]);
            break;
        case 10:
            if(oct == 7)
                parse_10_7_0_0(offs[0]);
            break;
        case 11:
            if(oct == 7)
                parse_11_7_0_0(offs[0]);
            break;
        case 12:
            if(oct == 2)
                parse_12_2_0_0(offs[0]);
            if(oct == 4)
                parse_12_4_0_0(offs[0]);
            if(oct == 6)
                parse_12_6_0_0(offs[0]);
            break;
        case 15:
            if(oct == 1)
                parse_15_1_0_0(offs[0]);
            break;
        case 16:
            if(oct == 2)
                parse_16_2_0_0(offs[0], ol - 4);
            break;
        case 19:
            if(oct == 4)
                parse_19_4_0_0(offs[0]);
            break;
        case 20:
            if(oct == 1)
                parse_20_1_0_0(offs[0], ol - 4);
            break;
        case 22:
            if(oct == 1)
                parse_22_1_0_0(offs[0]);
            if(oct == 2)
                parse_22_2_0_0(offs[0]);
            break;
        case 23:
            if(oct == 1)
                parse_23_1_0_0(offs[0]);
            break;
        case 24:
            if(oct == 1)
                parse_24_1_0_0(offs[0]);
            if(oct == 2)
                parse_24_2_0_0(offs[0]);
            break;
        case 25:
            if(oct == 1)
               parse_25_1_0_0(offs[0], ol - 4);
        case 35:
            if(oct == 2)
                parse_35_2_0_0(offs[0], ol - 4);
            break;
        case 131:
            if(oct == 1)
                parse_131_1_0_0(offs[0]);
            break;
        case 195:
            if(oct == 1)
                parse_195_1_0_0(offs[0]);
            break;
        case 196:
            if(oct == 1)
                parse_196_1_0_0(offs[0]);
            break;
        case 207:
            if(oct == 7)
                parse_207_7_0_0(offs[0]);
            break;
        case 229:
            if(oct == 1)
                parse_229_1_0_0(offs[0], ol - 4);
            break;
        case 230:
            if(oct == 0)
                parse_230_0_0_0(offs[0]);
            if(oct == 1)
                parse_230_1_0_0(offs[0], ol - 4);
            if(oct == 2)
                parse_230_2_0_0(offs[0], ol - 4);
            break;
        }
        offs[0] += ol - 4;
    }
    ret_conf = new rsvp_configuration();
    ret_conf = configuration;
    return ret_conf;
}

int rsvp_packet::get_data(void* buffer, int size)
{
    int t, r;
    free(packet_data);
    packet_data = NULL;
    packet_data_size = 0;
    t = 0;
    while(objects[t].cnum != 0 && t < MAX_OBJECTS)
    {
        switch(objects[t].cnum)
        {
        case 24:
            if(objects[t].ctnum == 1)
                if((r = add_24_1_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 2)
                if((r = add_24_2_0_0()) != 0)
                    return r;
            break;
        case 23:
            if(objects[t].ctnum == 1)
                if((r = add_23_1_0_0()) != 0)
                    return r;
            break;
        case 22:
            if(objects[t].ctnum == 1)
                if((r = add_22_1_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 2)
                if((r = add_22_2_0_0()) != 0)
                    return r;
            break;
        case 131:
            if(objects[t].ctnum == 1)
                if((r = add_131_1_0_0()) != 0)
                    return r;
            break;
        case 1:
            if(objects[t].ctnum == 7)
                if((r = add_1_7_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 11)
                if((r = add_1_11_0_0()) != 0)
                    return r;
            break;
        case 3:
            if(objects[t].ctnum == 1)
                if((r = add_3_1_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 3)
                if((r = add_3_3_0_0()) != 0)
                    return r;
            break;
        case 5:
            if(objects[t].ctnum == 1)
                if((r = add_5_1_0_0()) != 0)
                    return r;
            break;
        case 19:
            if(objects[t].ctnum == 4)
                if((r = add_19_4_0_0()) != 0)
                    return r;
            break;
        case 6:
            if(objects[t].ctnum == 1)
                if((r = add_6_1_0_0()) != 0)
                    return r;
            break;
        case 15:
            if(objects[t].ctnum == 1)
                if((r = add_15_1_0_0()) != 0)
                    return r;
            break;
        case 195:
            if(objects[t].ctnum == 1)
                if((r = add_195_1_0_0()) != 0)
                    return r;
            break;
        case 196:
            if(objects[t].ctnum == 1)
                if((r = add_196_1_0_0()) != 0)
                    return r;
            break;
        case 229:
            if(objects[t].ctnum == 1 && objects[t].scnum == 0 && objects[t].sctnum == 0)
                if((r = add_229_1_0_0()) != 0)
                    return r;
            break;
        case 11:
            if(objects[t].ctnum == 7)
                if((r = add_11_7_0_0()) != 0)
                    return r;
            break;
        case 12:
            if(objects[t].ctnum == 2)
                if((r = add_12_2_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 4)
                if((r = add_12_4_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 6)
                if((r = add_12_6_0_0()) != 0)
                    return r;
            break;
        case 35:
            if(objects[t].ctnum == 2)
                if((r = add_35_2_0_0()) != 0)
                    return r;
            break;
        case 8:
            if(objects[t].ctnum == 1)
                if((r = add_8_1_0_0()) != 0)
                    return r;
            break;
        case 9:
            if(objects[t].ctnum == 2)
                if((r = add_9_2_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 4)
                if((r = add_9_4_0_0()) != 0)
                    return r;
             if(objects[t].ctnum == 6)
                if((r = add_9_6_0_0()) != 0)
                    return r;
            break;
        case 10:
            if(objects[t].ctnum == 7)
                if((r = add_10_7_0_0()) != 0)
                    return r;
            break;
        case 16:
            if(objects[t].ctnum == 2)
                if((r = add_16_2_0_0()) != 0)
                    return r;
            break;
        case 230:                               // UNI2.0 CALL_ID - specified in RFC3474
            if(objects[t].ctnum == 0)
                if((r = add_230_0_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 1)
                if((r = add_230_1_0_0()) != 0)
                    return r;
            if(objects[t].ctnum == 2)
                if((r = add_230_2_0_0()) != 0)
                    return r;
            break;






        case 7:                                 // IETF
            if(objects[t].ctnum == 1)
                if((r = add_7_1_0_0()) != 0)
                    return r;
            break;
        case 20:                                // IETF
            if(objects[t].ctnum == 1)
                if((r = add_20_1_0_0()) != 0)
                    return r;
            break;
        case 207:                               // IETF
            if(objects[t].ctnum == 7)
                if((r = add_207_7_0_0()) != 0)
                    return r;
            break;
        }
        t++;
    }
    if(size < packet_data_size)
        return ERROR_BUF_2_SMALL;
    memcpy(buffer, packet_data, packet_data_size);
    return packet_data_size;
}

int rsvp_packet::parse_headers(void** buffer, int* size, struct IPHeader* hdr)
{
    struct IPHeader head;
    unsigned char* cbuf;
    unsigned short* sbuf;
    unsigned int* ibuf;
    unsigned char mtype;
    int ihlen;
    unsigned int x, s;
    void* newbuffer;

    cbuf = (unsigned char*)(*buffer);
    sbuf = (unsigned short*)(*buffer);
    ibuf = (unsigned int*)(*buffer);

    head.version_and_header_length = cbuf[0];
    head.TOS = cbuf[1];
    head.total_length = sbuf[1];
    head.identification = sbuf[2];
    head.flags_and_fragment_offset = sbuf[3];
    head.TTL = cbuf[8];
    head.protocol = cbuf[9];
    head.checksum = sbuf[5];
    head.source_address = ibuf[3];
    head.destination_address = ibuf[4];
    ihlen = ((head.version_and_header_length) & 0x0f) * 4;

    s = 0;
    for(x=0;x<(*size - ihlen)/2;x++)
        s += (unsigned int)(htons(sbuf[x+(ihlen/2)]));
    while(s > 0xffff)
        s = ((s>>16)&0xffff) + (s&0xffff);
    if(s != 0xffff)
        return ERROR_CHECKSUM;

    mtype = cbuf[ihlen+1];
    newbuffer = malloc(*size - ihlen - 8);
    memcpy(newbuffer, ((char*)(*buffer))+ihlen+8, *size - ihlen - 8);
    free(*buffer);
    *buffer = newbuffer;
    *size = *size - ihlen - 8;

    if(hdr != NULL)
    {
        memcpy(hdr, &head, sizeof(head));
    }

    return mtype;
}

int rsvp_packet::make_rsvp_packet(void** obj_buf, int size, unsigned char msg_type)
{
    unsigned short* sbuf;
    unsigned char* cbuf;
    unsigned int x, s;
    if((size % 4) != 0)
        return ERROR_SIZE_NOT_4;
    sbuf = (unsigned short*)malloc(size+8);
    cbuf = (unsigned char*)sbuf;
    memcpy(cbuf+8, *obj_buf, size);
    free(*obj_buf);
    cbuf[0] = 0x10;
    cbuf[1] = msg_type;
    cbuf[2] = 0; // empty checksum field
    cbuf[3] = 0; // ---------||---------
    cbuf[4] = 255;
    cbuf[5] = 0;
    sbuf[3] = htons((unsigned short)(size+8)); // sbuf[3] is cbuf[6] and cbuf[7]

    s = 0;
    for(x=0;x<(size+8)/2;x++)
        s += (unsigned int)(htons(sbuf[x]));
    while(s > 0xffff)
        s = ((s>>16)&0xffff) + (s&0xffff);
    s = ~s;
    sbuf[1] = htons((unsigned short)s);

    *obj_buf = cbuf;

    return size+8;
}


int rsvp_packet::set_configuration(rsvp_configuration* conf)
{
    free(configuration);
    configuration = new rsvp_configuration();
    *configuration = *conf;
    return 0;
}


int rsvp_packet::add_1_7_0_0()
{
    add_obj_header(12, 1, 7);
    add_addr(*(unsigned int*)(configuration->read(RC_SESSION_DEST_ADDR)));
    add_short(0);
    add_short(*(unsigned short*)(configuration->read(RC_TUNNEL_ID)));
    add_addr(*(unsigned int*)(configuration->read(RC_EXTENDED_IPV4_ADDR)));
    return 0;
}

int rsvp_packet::add_1_11_0_0()
{
    add_obj_header(12, 1, 11);
    add_addr(*(unsigned int*)(configuration->read(RC_SESSION_DEST_ADDR)));
    add_short(0);
    add_short(*(unsigned short*)(configuration->read(RC_TUNNEL_ID)));
    add_addr(*(unsigned int*)(configuration->read(RC_EXTENDED_IPV4_ADDR)));
    return 0;
}

int rsvp_packet::add_3_1_0_0()
{
    add_obj_header(8, 3, 1);
    add_addr(*(unsigned int*)(configuration->read(RC_CONTROL_OUT_IF_ADDR)));
    add_int(*(unsigned int*)(configuration->read(RC_CONTROL_OUT_IF_HANDLE)));
    return 0;
}

int rsvp_packet::add_3_3_0_0()
{
    int t;
    t = 0;
//     if(configuration->ispresent(RC_RSVP_HOP_TLVS) == VAL_FALSE)
//         return ERROR_VAL_NOT_PRES;
//     while(configuration->ispresent(RC_RSVP_HOP_TLVS, t))
//         t++;
//     add_obj_header(8+t*8, 3, 3);
    if(configuration->ispresent(RC_DATA_OUT_IF_ADDR) == VAL_FALSE)
        return ERROR_VAL_NOT_PRES;
    if(configuration->ispresent(RC_DATA_OUT_IF_ADDR) == VAL_TRUE && configuration->ispresent(RC_DATA_OUT_IF_HANDLE) == VAL_FALSE)
        add_obj_header(8+8, 3, 3);
    if(configuration->ispresent(RC_DATA_OUT_IF_ADDR) == VAL_TRUE && configuration->ispresent(RC_DATA_OUT_IF_HANDLE) == VAL_TRUE)
    {
        /* DEPRECATED IN FINAL UNI2.0Eth
        if(configuration->ispresent(RC_DATA_IN_IF_ADDR) == VAL_TRUE && configuration->ispresent(RC_DATA_IN_IF_HANDLE) == VAL_TRUE)
           add_obj_header(8+12+12, 3, 3);
        else
            add_obj_header(8+12, 3, 3);
        */
        add_obj_header(8+12, 3, 3);
    }
    add_addr(*(unsigned int*)(configuration->read(RC_CONTROL_OUT_IF_ADDR)));
    //add_int(*(unsigned int*)(configuration->read(RC_CONTROL_OUT_IF_HANDLE))); /* DEPRECATED IN FINAL UNI2.0Eth */
    add_int(0);
    t = 0;
//     if(configuration->ispresent(RC_RSVP_HOP_TLVS) == VAL_TRUE)
//     {
//         while(configuration->ispresent(RC_RSVP_HOP_TLVS, t))
//         {
//             add_short(1);
//             add_short(8);
//             add_addr(*(unsigned int*)(configuration->read(RC_RSVP_HOP_TLVS, t)));
//             t++;
//         }
//         return 0;
//     }

/* DEPRECATED IN FINAL UNI2.0Eth

    if(configuration->ispresent(RC_DATA_OUT_IF_ADDR) == VAL_TRUE)
    {
        if(configuration->ispresent(RC_DATA_OUT_IF_HANDLE) == VAL_TRUE)
        {
            add_short(4);
            add_short(12);
            add_addr(*(unsigned int*)(configuration->read(RC_DATA_OUT_IF_ADDR)));
            add_int(*(unsigned int*)(configuration->read(RC_DATA_OUT_IF_HANDLE)));
        }
        else
        {
            add_short(1);
            add_short(8);
            add_addr(*(unsigned int*)(configuration->read(RC_DATA_OUT_IF_ADDR)));
            return 0;
        }
    }
    if(configuration->ispresent(RC_DATA_IN_IF_ADDR) == VAL_TRUE && configuration->ispresent(RC_DATA_IN_IF_HANDLE) == VAL_TRUE)
    {
        add_short(5);
        add_short(12);
        add_addr(*(unsigned int*)(configuration->read(RC_DATA_IN_IF_ADDR)));
        add_int(*(unsigned int*)(configuration->read(RC_DATA_IN_IF_HANDLE)));
    }
*/
    if(configuration->ispresent(RC_DATA_OUT_IF_ADDR) == VAL_TRUE)
    {
        if(configuration->ispresent(RC_DATA_OUT_IF_HANDLE) == VAL_TRUE)
        {
            add_short(3);
            add_short(12);
            add_addr(*(unsigned int*)(configuration->read(RC_DATA_OUT_IF_ADDR)));
            add_int(*(unsigned int*)(configuration->read(RC_DATA_OUT_IF_HANDLE)));
        }
    }

    return 0;
}

int rsvp_packet::add_5_1_0_0()
{
    add_obj_header(4, 5, 1);
    add_int(*(unsigned int*)(configuration->read(RC_REFRESH_PERIOD)));
    return 0;
}

int rsvp_packet::add_6_1_0_0()
{
    add_obj_header(8, 6, 1);
    add_addr(*(unsigned int*)(configuration->read(RC_ERROR_NODE_ADDR)));
    add_char(*(unsigned char*)(configuration->read(RC_ERROR_FLAGS)));
    add_char(*(unsigned char*)(configuration->read(RC_ERROR_CODE)));
    add_short(*(unsigned short*)(configuration->read(RC_ERROR_VALUE)));
    return 0;
}

int rsvp_packet::add_7_1_0_0()
{
    int t;
    if(configuration->ispresent(RC_SCOPE_ADDR) == VAL_FALSE)
        return ERROR_VAL_NOT_PRES;
    t = 0;
    while(configuration->ispresent(RC_SCOPE_ADDR, t) == VAL_TRUE)
        t++;
    add_obj_header(t*4, 7, 1);
    t = 0;
    while(configuration->ispresent(RC_SCOPE_ADDR, t) == VAL_TRUE)
        add_addr(*(unsigned int*)(configuration->read(RC_SCOPE_ADDR, t)));
    return 0;
}

int rsvp_packet::add_8_1_0_0()
{
    add_obj_header(4, 8, 1);
    add_char(0);
    add_short(0);
    if(configuration->ispresent(RC_WILDCARD_FILTER))
    if((*(unsigned char*)(configuration->read(RC_WILDCARD_FILTER))) == 1)
    {
        add_char(0x11);
        return 0;
    }
    if(configuration->ispresent(RC_FIXED_FILTER))
    if((*(unsigned char*)(configuration->read(RC_FIXED_FILTER))) == 1)
    {
        add_char(0x0A);
        return 0;
    }
    if(configuration->ispresent(RC_SHARED_EXPLICIT))
    if((*(unsigned char*)(configuration->read(RC_SHARED_EXPLICIT))) == 1)
    {
        add_char(0x12);
        return 0;
    }
    return 0;
}

int rsvp_packet::add_9_2_0_0()
{
    if(configuration->ispresent(RC_RATE) == VAL_TRUE && configuration->ispresent(RC_SLACK_TERM) == VAL_TRUE)
    {
        add_obj_header(44, 9, 2);
        add_short(0);
        add_short(10);
        add_char(2);
        add_char(0);
        add_short(9);
        add_char(127);
        add_char(0);
        add_short(5);
        add_float(*(float*)(configuration->read(RC_TOKEN_BUCKET_RATE)));
        add_float(*(float*)(configuration->read(RC_TOKEN_BUCKET_SIZE)));
        add_float(*(float*)(configuration->read(RC_PEAK_DATA_RATE)));
        add_int(*(int*)(configuration->read(RC_MINIMUM_POLICED_UNIT)));
        add_int(*(int*)(configuration->read(RC_MAXIMUM_PACKET_SIZE)));
        add_char(130);
        add_char(0);
        add_short(2);
        add_float(*(float*)(configuration->read(RC_RATE)));
        add_int(*(int*)(configuration->read(RC_SLACK_TERM)));
    }
    else
    {
        add_obj_header(32, 9, 2);
        add_short(0);
        add_short(7);
        add_char(5);
        add_char(0);
        add_short(6);
        add_char(127);
        add_char(0);
        add_short(5);
        add_float(*(float*)(configuration->read(RC_TOKEN_BUCKET_RATE)));   //todo terrible hardcoding!!! Change!!!
        add_float(*(float*)(configuration->read(RC_TOKEN_BUCKET_SIZE)));
        add_float(*(float*)(configuration->read(RC_PEAK_DATA_RATE)));
        add_int(*(int*)(configuration->read(RC_MINIMUM_POLICED_UNIT)));
        add_int(*(int*)(configuration->read(RC_MAXIMUM_PACKET_SIZE)));
    }
    return 0;
}

int rsvp_packet::add_9_4_0_0()
{
    add_obj_header(16, 9, 4);
    add_char(*(char*)(configuration->read(RC_SSDH_SIGNAL_TYPE)));
    add_char(*(char*)(configuration->read(RC_SSDH_RCC)));
    add_short(*(short*)(configuration->read(RC_SSDH_NCC)));
    add_short(*(short*)(configuration->read(RC_SSDH_NVC)));
    add_short(*(short*)(configuration->read(RC_SSDH_MULTIPLIER)));
    add_int(*(int*)(configuration->read(RC_SSDH_TRANSPARENCY)));
    add_int(*(int*)(configuration->read(RC_SSDH_PROFILE)));
    return 0;
}

int rsvp_packet::add_9_6_0_0()
{
    add_obj_header(28, 9, 6);
    add_short(*(char*)(configuration->read(RC_ETHERNET_SW_GRANUALITY)));
    add_short(1518); // MTU
    add_short(129);  // TLV Type
    add_short(24);   // TLV Length
    add_char(*(char*)(configuration->read(RC_ETHERNET_PROFILE)));
    add_char(*(char*)(configuration->read(RC_ETHERNET_INDEX)));
    add_short(0);    // reserved
    add_float(*(float*)(configuration->read(RC_ETHERNET_CIR)));
    add_float(*(float*)(configuration->read(RC_ETHERNET_CBS)));
    add_float(*(float*)(configuration->read(RC_ETHERNET_EIR)));
    add_float(*(float*)(configuration->read(RC_ETHERNET_EBS)));
    return 0;
}

int rsvp_packet::add_10_7_0_0()
{
    add_obj_header(8, 10, 7);
    add_addr(*(int*)(configuration->read(RC_FILTER_SRC_ADDR)));
    add_short(0);
    add_short(*(short*)(configuration->read(RC_FILTER_SRC_PORT)));
    return 0;
}

int rsvp_packet::add_11_7_0_0()
{
    add_obj_header(8, 11, 7);
    add_addr(*(int*)(configuration->read(RC_SENDER_SRC_ADDR)));
    add_short(0);
    add_short(*(short*)(configuration->read(RC_SENDER_SRC_ID)));
    return 0;
}

int rsvp_packet::add_12_2_0_0()
{
    add_obj_header(32, 12, 2);
    add_short(0);
    add_short(7);
    add_char(1);
    add_char(0);
    add_short(6);
    add_char(127);
    add_char(0);
    add_short(5);
    add_float(*(float*)(configuration->read(RC_TOKEN_BUCKET_RATE)));
    add_float(*(float*)(configuration->read(RC_TOKEN_BUCKET_SIZE)));
    add_float(*(float*)(configuration->read(RC_PEAK_DATA_RATE)));
    add_int(*(int*)(configuration->read(RC_MINIMUM_POLICED_UNIT)));
    add_int(*(int*)(configuration->read(RC_MAXIMUM_PACKET_SIZE)));
    return 0;
}

int rsvp_packet::add_12_4_0_0()
{
    add_obj_header(16, 12, 4);
    add_char(*(char*)(configuration->read(RC_SSDH_SIGNAL_TYPE)));
    add_char(*(char*)(configuration->read(RC_SSDH_RCC)));
    add_short(*(short*)(configuration->read(RC_SSDH_NCC)));
    add_short(*(short*)(configuration->read(RC_SSDH_NVC)));
    add_short(*(short*)(configuration->read(RC_SSDH_MULTIPLIER)));
    add_int(*(int*)(configuration->read(RC_SSDH_TRANSPARENCY)));
    add_int(*(int*)(configuration->read(RC_SSDH_PROFILE)));
    return 0;
}

int rsvp_packet::add_12_6_0_0()
{
    add_obj_header(28, 12, 6);
    add_short(*(char*)(configuration->read(RC_ETHERNET_SW_GRANUALITY)));
    add_short(1518); // MTU
    add_short(129);  // TLV Type
    add_short(24);   // TLV Length
    add_char(*(char*)(configuration->read(RC_ETHERNET_PROFILE)));
    add_char(*(char*)(configuration->read(RC_ETHERNET_INDEX)));
    add_short(0);    // reserved
    add_float(*(float*)(configuration->read(RC_ETHERNET_CIR)));
    add_float(*(float*)(configuration->read(RC_ETHERNET_CBS)));
    add_float(*(float*)(configuration->read(RC_ETHERNET_EIR)));
    add_float(*(float*)(configuration->read(RC_ETHERNET_EBS)));
    return 0;
}

int rsvp_packet::add_15_1_0_0()
{
    add_obj_header(4, 15, 1);
    add_addr(*(int*)(configuration->read(RC_RESV_CONFIRM_RECV_ADDR)));
    return 0;
}

int rsvp_packet::add_16_2_0_0()
{
    void* buf;
    int size;
    buf = configuration->read(RC_LABEL, 0, &size);
    if(size%4 != 0)
    {
        size = size + (4-(size%4));
        buf = realloc(buf, size);
        if(buf == NULL)
            return ERROR_MALLOC;
    }
    add_obj_header(size, 16, 2);
    add_void(buf, size);
    return 0;
}

int rsvp_packet::add_19_4_0_0()
{
    add_obj_header(4, 19, 4);
    add_char(*(char*)(configuration->read(RC_LSP_ENC_TYPE)));
    add_char(*(char*)(configuration->read(RC_LSP_SWITCHING_TYPE)));
    add_short(*(short*)(configuration->read(RC_LSP_GPID)));
    return 0;
}

int rsvp_packet::add_20_1_0_0()
{
    int t;
    struct rc_ero* ero;
    if(configuration->ispresent(RC_ERO) == VAL_FALSE)
        return ERROR_VAL_NOT_PRES;
    t = 0;
    while(configuration->ispresent(RC_ERO, t) == VAL_TRUE)
        t++;
    add_obj_header(t*8, 20, 1);
    t = 0;
    while(configuration->ispresent(RC_ERO, t) == VAL_TRUE)
    {
        //free(ero);
        ero = (struct rc_ero*)configuration->read(RC_ERO, t);
        if(ero->loose == 1)
            add_char(0x81);
        else
            add_char(0x01);
        add_char(8);
        add_addr(ero->address);
        add_char(ero->prefix_length);
        add_char(0);
        t++;
    }
    return 0;
}

int rsvp_packet::add_22_1_0_0()
{
    add_obj_header(8, 22, 1);
    add_int(*(int*)(configuration->read(RC_HELLO_SRC_INSTANCE)));
    add_int(*(int*)(configuration->read(RC_HELLO_DST_INSTANCE)));
    return 0;
}

int rsvp_packet::add_22_2_0_0()
{
    add_obj_header(8, 22, 2);
    add_int(*(int*)(configuration->read(RC_HELLO_SRC_INSTANCE)));
    add_int(*(int*)(configuration->read(RC_HELLO_DST_INSTANCE)));
    return 0;
}

int rsvp_packet::add_23_1_0_0()
{
    unsigned char z;
    unsigned int x;
    add_obj_header(8, 23, 1);
    add_char(*(char*)(configuration->read(RC_MSG_ID_FLAGS)));
    x = *(int*)(configuration->read(RC_MSG_ID_EPOCH));
    z = (x & 0x00ff0000) >> 16;
    add_char(z);
    z = (x & 0x0000ff00) >> 8;
    add_char(z);
    z = (x & 0x000000ff);
    add_char(z);
    add_int(*(int*)(configuration->read(RC_MSG_ID_ID)));
    return 0;
}

int rsvp_packet::add_24_1_0_0()
{
    unsigned char z;
    unsigned int x;
    add_obj_header(8, 24, 1);
    add_char(*(char*)(configuration->read(RC_MSG_ID_FLAGS)));
    x = *(int*)(configuration->read(RC_MSG_ID_EPOCH));
    z = (x & 0x00ff0000) >> 16;
    add_char(z);
    z = (x & 0x0000ff00) >> 8;
    add_char(z);
    z = (x & 0x000000ff);
    add_char(z);
    add_int(*(int*)(configuration->read(RC_MSG_ID_ID)));
    return 0;
}

int rsvp_packet::add_24_2_0_0()
{
    unsigned char z;
    unsigned int x;
    add_obj_header(8, 24, 2);
    add_char(*(char*)(configuration->read(RC_MSG_ID_FLAGS)));
    x = *(int*)(configuration->read(RC_MSG_ID_EPOCH));
    z = (x & 0x00ff0000) >> 16;
    add_char(z);
    z = (x & 0x0000ff00) >> 8;
    add_char(z);
    z = (x & 0x000000ff);
    add_char(z);
    add_int(*(int*)(configuration->read(RC_MSG_ID_ID)));
    return 0;
}

int rsvp_packet::add_35_2_0_0()
{
    void* buf;
    int size = 4;
    buf = configuration->read(RC_UPSTREAM_LABEL); //, 0, &size);
    if(size%4 != 0)
    {
        size = size + (4-(size%4));
        buf = realloc(buf, size);
        if(buf == NULL)
            return ERROR_MALLOC;
    }
    add_obj_header(size, 35, 2);
    add_int(*(int*)(configuration->read(RC_UPSTREAM_LABEL)));
    return 0;
}

int rsvp_packet::add_131_1_0_0()
{
    add_obj_header(8, 131, 1);
    add_int(*(int*)(configuration->read(RC_RESTART_TIME)));
    add_int(*(int*)(configuration->read(RC_RECOVERY_TIME)));
    return 0;
}

int rsvp_packet::add_195_1_0_0()
{
    add_obj_header(4, 195, 1);
    add_addr(*(int*)(configuration->read(RC_NOTIFY_ADDRESS)));
    return 0;
}

int rsvp_packet::add_196_1_0_0()
{
    unsigned char rtad;
    unsigned char z;
    add_obj_header(4, 196, 1);
    rtad = *(unsigned char*)(configuration->read(RC_ADMIN_STATUS_RTAD_FLAGS));
    if((rtad & 0x08) != 0)
        z = 0x80;
    else
        z = 0x00;
    add_char(z);
    add_short(0);
    z = rtad & 0x07;
    add_char(z);
    return 0;
}

int rsvp_packet::add_207_7_0_0()
{
    char* name_buf;
    int name_len;
    unsigned char flags;
    name_buf = (char*)(configuration->read(RC_SESSION_NAME));
    name_len = strlen(name_buf);
    if(name_len%4 != 0)
    {
        name_len = name_len + (4 - name_len%4);
        name_buf = (char*)realloc(name_buf, name_len);
        if(name_buf == NULL)
            return ERROR_MALLOC;
    }
    add_obj_header(name_len + 4, 207, 7);
    add_char(*(unsigned char*)(configuration->read(RC_SETUP_PRIORITY)));
    add_char(*(unsigned char*)(configuration->read(RC_HOLDING_PRIORITY)));
    flags = 0;
    if(*(unsigned char*)(configuration->read(RC_LOCAL_PROTECTION_FLAG)) != 0)
        flags = flags | 0x01;
    if(*(unsigned char*)(configuration->read(RC_LABEL_RECORDING_FLAG)) != 0)
        flags = flags | 0x02;
    if(*(unsigned char*)(configuration->read(RC_SE_STYLE_FLAG)) != 0)
        flags = flags | 0x04;
    add_char(flags);
    add_char((unsigned char)strlen(name_buf));
    add_chars(name_buf, name_len);
    return 0;
}

int rsvp_packet::add_229_1_0_0()
{
    int len_pos, t, r;
    len_pos = packet_data_size;
    add_obj_header(0, 229, 1);
    t = 0;
    while(objects[t].cnum != 0 && t < MAX_OBJECTS)
    {
        if(objects[t].cnum == 229 && objects[t].ctnum == 1 && objects[t].scnum == 2 && objects[t].sctnum == 1)
            if((r = add_229_1_2_1()) != 0)
                return r;
        if(objects[t].cnum == 229 && objects[t].ctnum == 1 && objects[t].scnum == 1 && objects[t].sctnum == 1)
            if((r = add_229_1_1_1()) != 0)
                return r;
        if(objects[t].cnum == 229 && objects[t].ctnum == 1 && objects[t].scnum == 5 && objects[t].sctnum == 1)
            if((r = add_229_1_5_1()) != 0)
                return r;
        t++;
    }
    set_short(packet_data_size - len_pos, len_pos);
    return 0;
}

int rsvp_packet::add_229_1_1_1()
{
    add_obj_header(4, 1, 1);
    add_addr(*(unsigned int*)(configuration->read(RC_TNA_SRC_ADDR)));
    return 0;
}

int rsvp_packet::add_229_1_2_1()
{
    add_obj_header(4, 2, 1);
    add_addr(*(unsigned int*)(configuration->read(RC_TNA_DST_ADDR)));
    return 0;
}

int rsvp_packet::add_229_1_5_1()
{
    add_obj_header(4, 5, 1);
    add_char(*(unsigned char*)(configuration->read(RC_SERVICE_LEVEL)));
    add_char(0);
    add_char(0);
    add_char(0);
    return 0;
}

int rsvp_packet::add_230_0_0_0()
{ 
    add_obj_header(0, 230, 0);
    return 0;
}

int rsvp_packet::add_230_1_0_0()
{
    add_obj_header(16, 230, 1);
    add_char(*(unsigned char*)(configuration->read(RC_CALL_ID_ADDR_TYPE)));
    add_char(0);
    add_char(0);
    add_char(0);
    add_addr(*(unsigned int*)(configuration->read(RC_CALL_ID_SRC_TN_ADDR)));
    add_int(*(unsigned int*)(configuration->read(RC_CALL_ID_LOCAL_ID1)));
    add_int(*(unsigned int*)(configuration->read(RC_CALL_ID_LOCAL_ID2)));
    return 0;
}

int rsvp_packet::add_230_2_0_0()
{
    unsigned int val;
    add_obj_header(28, 230, 2);
    add_char(*(unsigned char*)(configuration->read(RC_CALL_ID_ADDR_TYPE)));
    val = *(unsigned int*)configuration->read(RC_CALL_ID_IS);
    add_char((val & (0xff << 16)) >> 16);
    add_char((val & (0xff << 8)) >> 8);
    add_char(val & (0xff));
    add_int(*(unsigned int*)(configuration->read(RC_CALL_ID_ITU_CC1)));
    add_short(*(unsigned short*)(configuration->read(RC_CALL_ID_ITU_CC2)));
    add_short(*(unsigned short*)(configuration->read(RC_CALL_ID_UAPC1)));
    add_int(*(unsigned int*)(configuration->read(RC_CALL_ID_UAPC2)));
    add_addr(*(unsigned int*)(configuration->read(RC_CALL_ID_SRC_TN_ADDR)));
    add_int(*(unsigned int*)(configuration->read(RC_CALL_ID_LOCAL_ID1)));
    add_int(*(unsigned int*)(configuration->read(RC_CALL_ID_LOCAL_ID2)));
    return 0;
}

int rsvp_packet::add_obj_header(unsigned short data_size, unsigned char cnum, unsigned char ctnum)
{
    add_short(data_size + 4);
    add_char(cnum);
    add_char(ctnum);
    return 0;
}

int rsvp_packet::add_object(int cnum, int ctnum, int scnum, int sctnum, char* obj_description)
{
    unsigned int t, n, o;
    unsigned char a, b, c, d, e, f, g, h;
    if(objects[MAX_OBJECTS-1].cnum != 0)
    {
        VERBOSE(7, "we are not allowed to add %s object! - too many objects\n", obj_description);
        return ERROR_2_MANY_OBJECTS;
    }
    if(check_addable(cnum, ctnum, scnum, sctnum) != VAL_TRUE)
    {
        VERBOSE(7, "we are not allowed to add %s object!\n - not addable", obj_description);
        return ERROR_CONF_INSUFF;
    }

    t = 0;
    n = get_object_order(cnum, ctnum, scnum, sctnum);
    o = get_object_order(objects[t].cnum, objects[t].ctnum, objects[t].scnum, objects[t].sctnum);
    while((n > o) && (objects[t].cnum != 0))
    {
        t++;
        o = get_object_order(objects[t].cnum, objects[t].ctnum, objects[t].scnum, objects[t].sctnum);
    }


    e = cnum;
    f = ctnum;
    g = scnum;
    h = sctnum;
    for(n=t;n<MAX_OBJECTS-1;n++)
    {
        a = objects[n].cnum;
        b = objects[n].ctnum;
        c = objects[n].scnum;
        d = objects[n].sctnum;
        objects[n].cnum = e;
        objects[n].ctnum = f;
        objects[n].scnum = g;
        objects[n].sctnum = h;
        e = a;
        f = b;
        g = c;
        h = d;
    }
    return 0;
}

#endif

