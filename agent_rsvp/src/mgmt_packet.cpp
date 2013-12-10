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
 
#ifndef MGMT_PACKET_BODY_FILE
#define MGMT_PACKET_BODY_FILE

#include "mgmt_packet.h"

void mgmt_packet::init()
{
    contents = NULL;
    contents_length = 0;
    configuration = NULL;
    packet_type = 0;
    objects_count = 0;
}

mgmt_packet::mgmt_packet(unsigned short id, unsigned char type, unsigned char comm)
{
    unsigned char* cptr;
    unsigned short* sptr;
    void* buf;
    
    init();
    packet_id = id;
    packet_type = type;
    packet_comm = comm;
    add_unsigned_int(OBJECT_MULTI_NUM,   0);
    add_unsigned_int(OBJECT_MULTI_COUNT, 0);
    
    cptr = (unsigned char*)malloc(contents_length + 5);
    sptr = (unsigned short*)cptr;
    memcpy(cptr+5, contents, contents_length);
    contents_length += 5;
    sptr[0] = htons(packet_id);
    cptr[2] = packet_type;
    cptr[3] = packet_comm;
    cptr[4] = objects_count;

    free(contents);
    contents = (void*)cptr;
}

mgmt_packet::mgmt_packet(rsvp_configuration* conf, unsigned short id, unsigned char type, unsigned char comm, int multi_num, int multi_count)
{
    unsigned char* cptr;
    unsigned short* sptr;
    void* buf;
    unsigned int value;

    init();
    configuration = new rsvp_configuration();
    *configuration = *conf;
    packet_id = id;
    packet_type = type;
    packet_comm = comm;

    if(multi_num >= 0 && multi_count > 0 && multi_num < multi_count)
    {
        add_unsigned_int(OBJECT_MULTI_NUM,   multi_num);
        add_unsigned_int(OBJECT_MULTI_COUNT, multi_count);
    }
    if(configuration->ispresent(RC_SESSION_DEST_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_SESSION_DEST_ADDR);
        add_ipaddr(OBJECT_DST_ADDR, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_EXTENDED_IPV4_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_EXTENDED_IPV4_ADDR);
        add_ipaddr(OBJECT_SRC_ADDR, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_CONTROL_OUT_IF_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_CONTROL_OUT_IF_ADDR);
        add_ipaddr(OBJECT_OUT_ADDR, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_REFRESH_PERIOD) == VAL_TRUE)
    {
        buf = configuration->read(RC_REFRESH_PERIOD);
        add_unsigned_int(OBJECT_REFRESH,*(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_TOKEN_BUCKET_RATE) == VAL_TRUE)
    {
        buf = configuration->read(RC_TOKEN_BUCKET_RATE);
        add_float(OBJECT_TB_RATE,*(float*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_TOKEN_BUCKET_SIZE) == VAL_TRUE)
    {
        buf = configuration->read(RC_TOKEN_BUCKET_SIZE);
        add_float(OBJECT_TB_SIZE,*(float*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_PEAK_DATA_RATE) == VAL_TRUE)
    {
        buf = configuration->read(RC_PEAK_DATA_RATE);
        add_float(OBJECT_PEAK,*(float*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_UPSTREAM_LABEL) == VAL_TRUE)
    {
        int size;
        buf = configuration->read(RC_UPSTREAM_LABEL, 0, &size);
        add_unsigned_int(OBJECT_UP_LABEL,*(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_LSP_ENC_TYPE) == VAL_TRUE)
    {
        buf = configuration->read(RC_LSP_ENC_TYPE);
        add_unsigned_char(OBJECT_LSP_ENC,*(unsigned char*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_LSP_SWITCHING_TYPE) == VAL_TRUE)
    {
        buf = configuration->read(RC_LSP_SWITCHING_TYPE);
        add_unsigned_char(OBJECT_LSP_SW,*(unsigned char*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_LSP_GPID) == VAL_TRUE)
    {
        buf = configuration->read(RC_LSP_GPID);
        add_unsigned_short(OBJECT_LSP_GPID,*(unsigned short*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_SESSION_NAME) == VAL_TRUE)
    {
        buf = configuration->read(RC_SESSION_NAME);
        add_string(OBJECT_PATH_NAME, (char*)buf, strlen((char*)buf));
        free(buf);
    }
    if(configuration->ispresent(RC_ERO) == VAL_TRUE)
    {
        int t, size;
        struct rc_ero* ero;

        t = 0;
        while(configuration->ispresent(RC_ERO, t) == VAL_TRUE)
        {
            ero = (struct rc_ero*)configuration->read(RC_ERO, t, &size);
            add_ipaddr(OBJECT_ERO_ADDR, ero->address);
            add_unsigned_char(OBJECT_ERO_PREF,ero->prefix_length);
            add_unsigned_char(OBJECT_ERO_SL,ero->loose);
            free(ero);
            t++;
        }
    }
    if(configuration->ispresent(RC_RSVP_HOP_TLVS) == VAL_TRUE)
    {
        int t;

        t = 0;
        while(configuration->ispresent(RC_RSVP_HOP_TLVS, t) == VAL_TRUE)
        {
            buf = configuration->read(RC_RSVP_HOP_TLVS, t);
            add_ipaddr(OBJECT_HOP_ADDR, *(unsigned int*)buf);
            free(buf);
            t++;
        }
    }
    if(configuration->ispresent(RC_ERROR_NODE_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_ERROR_NODE_ADDR);
        add_ipaddr(OBJECT_ERROR_ADDR, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_ERROR_FLAGS) == VAL_TRUE)
    {
        buf = configuration->read(RC_ERROR_FLAGS);
        add_unsigned_char(OBJECT_ERROR_FLAGS,*(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_ERROR_CODE) == VAL_TRUE)
    {
        buf = configuration->read(RC_ERROR_CODE);
        add_unsigned_char(OBJECT_ERROR_CODE,*(unsigned char*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_ERROR_VALUE) == VAL_TRUE)
    {
        buf = configuration->read(RC_ERROR_VALUE);
        add_unsigned_short(OBJECT_ERROR_VALUE,*(unsigned short*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_TUNNEL_ID) == VAL_TRUE)
    {
        buf = configuration->read(RC_TUNNEL_ID);
        add_unsigned_short(OBJECT_TUNNEL_ID,*(unsigned short*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_TR_D_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_TR_D_ADDR);
        add_ipaddr(OBJECT_TR_D_ADDR, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_TR_RD_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_TR_RD_ADDR);
        add_ipaddr(OBJECT_TR_RD_ADDR, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_CONTROL_OUT_IF_HANDLE) == VAL_TRUE)
    {
        buf = configuration->read(RC_CONTROL_OUT_IF_HANDLE);
        add_unsigned_int(OBJECT_OUT_IF_HANDLE,*(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_TNA_SRC_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_TNA_SRC_ADDR);
        add_ipaddr(OBJECT_SRC_TNA, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_TNA_DST_ADDR) == VAL_TRUE)
    {
        buf = configuration->read(RC_TNA_DST_ADDR);
        add_ipaddr(OBJECT_DST_TNA, *(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_CALL_ID_LOCAL_ID1) == VAL_TRUE)
    {
        buf = configuration->read(RC_CALL_ID_LOCAL_ID1);
        add_unsigned_int(OBJECT_CALL_ID_1,*(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_CALL_ID_LOCAL_ID2) == VAL_TRUE)
    {
        buf = configuration->read(RC_CALL_ID_LOCAL_ID2);
        add_unsigned_int(OBJECT_CALL_ID_2,*(unsigned int*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_SENDER_SRC_ID) == VAL_TRUE)
    {
        buf = configuration->read(RC_SENDER_SRC_ID);
        add_unsigned_short(OBJECT_SENDER_LSP_ID,*(unsigned short*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_SETUP_PRIORITY) == VAL_TRUE)
    {
        buf = configuration->read(RC_SETUP_PRIORITY);
        add_unsigned_char(OBJECT_SETUP_PRIOR,*(unsigned char*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_HOLDING_PRIORITY) == VAL_TRUE)
    {
        buf = configuration->read(RC_HOLDING_PRIORITY);
        add_unsigned_char(OBJECT_HOLD_PRIOR,*(unsigned char*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_PATH_STATUS) == VAL_TRUE)
    {
        buf = configuration->read(RC_PATH_STATUS);
        add_unsigned_short(OBJECT_PATH_STATE,*(unsigned short*)buf);
        free(buf);
    }   
    if(configuration->ispresent(RC_SERVICE_LEVEL) == VAL_TRUE)
    {
        buf = configuration->read(RC_SERVICE_LEVEL);
        add_unsigned_char(OBJECT_PATH_SERV_LEVEL,*(unsigned char*)buf);
        free(buf);
    } 
    if(configuration->ispresent(RC_ETHERNET_CIR) == VAL_TRUE)
    {
        buf = configuration->read(RC_ETHERNET_CIR);
        add_float(OBJECT_ETHERNET_CIR,*(float*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_ETHERNET_CBS) == VAL_TRUE)
    {
        buf = configuration->read(RC_ETHERNET_CBS);
        add_float(OBJECT_ETHERNET_CBS,*(float*)buf);
        free(buf);
    }  
    if(configuration->ispresent(RC_ETHERNET_EIR) == VAL_TRUE)
    {
        buf = configuration->read(RC_ETHERNET_EIR);
        add_float(OBJECT_ETHERNET_EIR,*(float*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_ETHERNET_EBS) == VAL_TRUE)
    {
        buf = configuration->read(RC_ETHERNET_EBS);
        add_float(OBJECT_ETHERNET_EBS,*(float*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_ETHERNET_SW_GRANUALITY) == VAL_TRUE)
    {
        buf = configuration->read(RC_ETHERNET_SW_GRANUALITY);
        add_unsigned_short(OBJECT_ETHERNET_SW_GRANUALITY,*(unsigned short*)buf);
        free(buf);
    } 
    if(configuration->ispresent(RC_ETHERNET_INDEX) == VAL_TRUE)
    {
        buf = configuration->read(RC_ETHERNET_INDEX);
        add_unsigned_char(OBJECT_ETHERNET_INDEX,*(unsigned char*)buf);
        free(buf);
    }
    if(configuration->ispresent(RC_ETHERNET_PROFILE) == VAL_TRUE)
    {
        buf = configuration->read(RC_ETHERNET_PROFILE);
        add_unsigned_char(OBJECT_ETHERNET_PROFILE,*(unsigned char*)buf);
        free(buf);
    }

    cptr = (unsigned char*)malloc(contents_length + 5);
    sptr = (unsigned short*)cptr;
    memcpy(cptr+5, contents, contents_length);
    contents_length += 5;
    sptr[0] = htons(packet_id);
    cptr[2] = packet_type;
    cptr[3] = packet_comm;
    cptr[4] = objects_count;

    free(contents);
    contents = (void*)cptr;
}

mgmt_packet::mgmt_packet(char* cont, int length)
{
    unsigned char* cbuf;
    unsigned short* sbuf;
    unsigned int* ibuf;
    float* fbuf;
    char* csbuf;
    unsigned short o_type;
    unsigned char o_data_type;
    unsigned char o_size;
    struct rc_ero* ero;
    int ero_addr_count;
    int ero_pref_count;
    int ero_sl_count;
    int ero_count;
    int len, t;

    init();
    contents = malloc(length);
    contents_length = length;
    memcpy(contents, cont, length);

    ero_addr_count = 0;
    ero_pref_count = 0;
    ero_sl_count = 0;
    ero_count = 0;
    ero = NULL;
    free(configuration);
    configuration = new rsvp_configuration();
    sbuf = (unsigned short*)contents;
    cbuf = (unsigned char*)contents;

    packet_id = ntohs(sbuf[0]);
    packet_type = cbuf[2];
    packet_comm = cbuf[3];
    objects_count = cbuf[4];
    len = 5;
    for(t=0;t<objects_count;t++)
    {
        cbuf = cbuf+len;
        sbuf = (unsigned short*)cbuf;
        o_type = ntohs(sbuf[0]);
        o_data_type = cbuf[2];
        o_size = cbuf[3];
        VERBOSE(7, "creating mgmt_packet: analysing object %d\n", o_type);
        switch(o_type)
        {
        case 1:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_SESSION_DEST_ADDR, ibuf[1]);
            break;
        case 2:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_EXTENDED_IPV4_ADDR, ibuf[1]);
            break;
        case 3:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_CONTROL_OUT_IF_ADDR, ibuf[1]);
            configuration->add(RC_DATA_IN_IF_ADDR, ibuf[1]);
            configuration->add(RC_DATA_OUT_IF_ADDR, ibuf[1]);
            break;
        case 4:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_REFRESH_PERIOD, ntohl(ibuf[1]));
            break;
        case 5:
            fbuf = (float*)cbuf;
            configuration->add(RC_TOKEN_BUCKET_RATE, fbuf[1]);
            break;
        case 6:
            fbuf = (float*)cbuf;
            configuration->add(RC_TOKEN_BUCKET_SIZE, fbuf[1]);
            break;
        case 7:
            fbuf = (float*)cbuf;
            configuration->add(RC_PEAK_DATA_RATE, fbuf[1]);
            break;
        case 8:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_UPSTREAM_LABEL, ntohl(ibuf[1]));
            break;
        case 9:
            configuration->add(RC_LSP_ENC_TYPE, cbuf[4]);
            break;
        case 10:
            configuration->add(RC_LSP_SWITCHING_TYPE, cbuf[4]);
            break;
        case 11:
            configuration->add(RC_LSP_GPID, ntohs(sbuf[2]));
            break;
        case 12:
            csbuf = (char*)malloc(o_size+1);
            memcpy(csbuf, cbuf+4, o_size);
            csbuf[o_size] = 0;
            configuration->add(RC_SESSION_NAME, csbuf);
            free(csbuf);
            break;
        case 13:
            ero_addr_count++;
            if(ero_addr_count > ero_count)
            {
                ero_count++;
                ero = (struct rc_ero*)realloc(ero, sizeof(struct rc_ero) * (ero_count+1));
            }
            ero[ero_addr_count-1].address = ((unsigned int*)cbuf)[1];
            break;
        case 14:
            ero_pref_count++;
            if(ero_pref_count > ero_count)
            {
                ero_count++;
                ero = (struct rc_ero*)realloc(ero, sizeof(struct rc_ero) * (ero_count+1));
            }
            ero[ero_pref_count-1].prefix_length = cbuf[4];
            break;
        case 15:
            ero_sl_count++;
            if(ero_sl_count > ero_count)
            {
                ero_count++;
                ero = (struct rc_ero*)realloc(ero, sizeof(struct rc_ero) * (ero_count+1));
            }
            ero[ero_sl_count-1].loose = cbuf[4];
            break;
        case 16:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_RSVP_HOP_TLVS, ibuf[1]);
            break;
        case 17:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_ERROR_NODE_ADDR, ibuf[1]);
            break;
        case 18:
            configuration->add(RC_ERROR_FLAGS, cbuf[4]);
            break;
        case 19:
            configuration->add(RC_ERROR_CODE, cbuf[4]);
            break;
        case 20:
            configuration->add(RC_ERROR_VALUE, ntohs(sbuf[2]));
            break;
        case 21:
            VERBOSE(0, "this object (OBJECT_MULTI_NUM) should appear only in status ack message sent from agent to GUI!\n");
            break;
        case 22:
            VERBOSE(0, "this object (OBJECT_MULTI_COUNT) should appear only in status ack message sent from agent to GUI!\n");
            break;
        case 23:
            VERBOSE(7, "mgmt_packet adding TUNNEL_ID (%d) to configuration\n", ntohs(sbuf[2]));
            configuration->add(RC_TUNNEL_ID, ntohs(sbuf[2]));
            break;
        case 24:
            ibuf = (unsigned int*)cbuf;
            VERBOSE(7, "adding RC_TR_D_ADDR to configuration\n");
            configuration->add(RC_TR_D_ADDR, ibuf[1]);
            break;
        case 25:
            ibuf = (unsigned int*)cbuf;
            VERBOSE(7, "adding RC_TR_RD_ADDR to configuration\n");
            configuration->add(RC_TR_RD_ADDR, ibuf[1]);
            break;
        case 26:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_CONTROL_OUT_IF_HANDLE, ntohl(ibuf[1]));
            configuration->add(RC_DATA_OUT_IF_HANDLE, ntohl(ibuf[1]));
            configuration->add(RC_DATA_IN_IF_HANDLE, ntohl(ibuf[1])); 
            break;
        case 27:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_TNA_SRC_ADDR, ibuf[1]);
            configuration->add(RC_CALL_ID_SRC_TN_ADDR, ibuf[1]);
            break;
        case 28:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_TNA_DST_ADDR, ibuf[1]);
            break;
        case 29:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_CALL_ID_LOCAL_ID1, ntohl(ibuf[1]));
            break;
        case 30:
            ibuf = (unsigned int*)cbuf;
            configuration->add(RC_CALL_ID_LOCAL_ID2, ntohl(ibuf[1]));
            break;
        case 31:
            configuration->add(RC_SENDER_SRC_ID, ntohs(sbuf[2]));
            break;
        case 32:
            configuration->add(RC_SETUP_PRIORITY, cbuf[4]);
            break;
        case 33:
            configuration->add(RC_HOLDING_PRIORITY, cbuf[4]);
            break;
        case 34:
            configuration->add(RC_PATH_STATUS, ntohs(sbuf[2]));
            break;
        case OBJECT_PATH_SERV_LEVEL:
            configuration->add(RC_SERVICE_LEVEL, cbuf[4]);
            break;
        case OBJECT_ETHERNET_CIR:
            fbuf = (float*)cbuf;
            configuration->add(RC_ETHERNET_CIR, fbuf[1]);
            break;
        case OBJECT_ETHERNET_CBS:
            fbuf = (float*)cbuf;
            configuration->add(RC_ETHERNET_CBS, fbuf[1]);
            break;
        case OBJECT_ETHERNET_EIR:
            fbuf = (float*)cbuf;
            configuration->add(RC_ETHERNET_EIR, fbuf[1]);
            break;
        case OBJECT_ETHERNET_EBS:
            fbuf = (float*)cbuf;
            configuration->add(RC_ETHERNET_EBS, fbuf[1]);
            break;
        case OBJECT_ETHERNET_SW_GRANUALITY:
            configuration->add(RC_ETHERNET_SW_GRANUALITY, ntohs(sbuf[2]));
            break;
        case OBJECT_ETHERNET_INDEX:
            configuration->add(RC_ETHERNET_INDEX, cbuf[4]);
            break;
        case OBJECT_ETHERNET_PROFILE:
            configuration->add(RC_ETHERNET_PROFILE, cbuf[4]);
            break;
         

        default:
            VERBOSE(0, "unknown mgmt object type: %d\n", o_type);
            break;
        }
        
        switch(o_data_type)
        {
        case DATA_TYPE_UC:
            len = 5; break;
        case DATA_TYPE_US:
            len = 6; break;
        case DATA_TYPE_UI:
            len = 8; break;
        case DATA_TYPE_UL:
            len = 12; break;
        case DATA_TYPE_F:
            len = 8; break;
        case DATA_TYPE_CS:
            len = o_size + 4; break;
        case DATA_TYPE_V:
            len = o_size + 4; break;
        }
    }

    if(ero_addr_count != ero_pref_count || ero_pref_count != ero_sl_count || ero_sl_count != ero_count)
    {
        if(ero_addr_count < ero_count)
            ero_count = ero_addr_count;
        if(ero_pref_count < ero_count)
            ero_count = ero_pref_count;
        if(ero_sl_count < ero_count)
            ero_count = ero_sl_count;
    }
    for(t=0;t<ero_count;t++)
        configuration->add(RC_ERO, &(ero[t]), sizeof(struct rc_ero));
}

mgmt_packet::~mgmt_packet()
{
    free(configuration);
    free(contents);
}

//--------------------------------------------------------------------------------------------------

int mgmt_packet::add_unsigned_int(unsigned short id, unsigned int value)
{
    unsigned char* cptr;
    unsigned short* sptr;
    unsigned int* iptr;

    contents = realloc(contents, contents_length+8);
    if(contents == NULL)
        return -1;
    cptr = ((unsigned char*)contents)+contents_length;
    contents_length += 8;
    sptr = (unsigned short*)cptr;
    iptr = (unsigned int*)cptr;

    sptr[0] = htons(id);
    cptr[2] = 3;
    cptr[3] = 0;
    iptr[1] = htonl(value);
    objects_count++;
    return 0;
}

int mgmt_packet::add_ipaddr(unsigned short id, unsigned int addr)
{
    unsigned char* cptr;
    unsigned short* sptr;
    unsigned int* iptr;

    contents = realloc(contents, contents_length+8);
    if(contents == NULL)
        return -1;
    cptr = ((unsigned char*)contents)+contents_length;
    contents_length += 8;
    sptr = (unsigned short*)cptr;
    iptr = (unsigned int*)cptr;

    sptr[0] = htons(id);
    cptr[2] = 3;
    cptr[3] = 0;
    iptr[1] = addr;
    objects_count++;
    return 0;
}

int mgmt_packet::add_unsigned_short(unsigned short id, unsigned short value)
{
    unsigned char* cptr;
    unsigned short* sptr;

    contents = realloc(contents, contents_length+6);
    if(contents == NULL)
        return -1;
    cptr = ((unsigned char*)contents)+contents_length;
    contents_length += 6;
    sptr = (unsigned short*)cptr;

    sptr[0] = htons(id);
    cptr[2] = 2;
    cptr[3] = 0;
    sptr[2] = htons(value);
    objects_count++;
    return 0;
}

int mgmt_packet::add_unsigned_char(unsigned short id, unsigned char value)
{
    unsigned char* cptr;
    unsigned short* sptr;

    contents = realloc(contents, contents_length+5);
    if(contents == NULL)
        return -1;
    cptr = ((unsigned char*)contents)+contents_length;
    contents_length += 5;
    sptr = (unsigned short*)cptr;

    sptr[0] = htons(id);
    cptr[2] = 1;
    cptr[3] = 0;
    memcpy(&(cptr[4]), &value, 1);
    objects_count++;
    return 0;
}

int mgmt_packet::add_float(unsigned short id, float value)
{
    unsigned char* cptr;
    unsigned short* sptr;
    unsigned int* iptr;

    contents = realloc(contents, contents_length+8);
    if(contents == NULL)
        return -1;
    cptr = ((unsigned char*)contents)+contents_length;
    contents_length += 8;
    sptr = (unsigned short*)cptr;
    iptr = (unsigned int*)cptr;

    sptr[0] = htons(id);
    cptr[2] = 5;
    cptr[3] = 0;
    memcpy(&(cptr[4]), &value, 4);
    objects_count++;
    return 0;
}

int mgmt_packet::add_string(unsigned short id, const char* name, int size)
{
    unsigned char* cptr;
    unsigned short* sptr;

    contents = realloc(contents, contents_length+4+size);
    if(contents == NULL)
        return -1;
    cptr = ((unsigned char*)contents)+contents_length;
    contents_length += 4+size;
    sptr = (unsigned short*)cptr;

    sptr[0] = htons(id);
    cptr[2] = 6;
    cptr[3] = size;
    memcpy(&(cptr[4]), name, size);
    objects_count++;
    return 0;
}

//--------------------------------------------------------------------------------------------------

void* mgmt_packet::get_data(int* size)
{
    if(size != NULL)
        *size = contents_length;
    return contents;
}

rsvp_configuration mgmt_packet::get_conf()
{
    VERBOSE(7, "get_conf\n");
    return *configuration;
}

unsigned char mgmt_packet::get_type()
{
    return packet_type;
}

unsigned char mgmt_packet::get_comm()
{
    return packet_comm;
}

unsigned short mgmt_packet::get_id()
{
    return packet_id;
}

#endif
