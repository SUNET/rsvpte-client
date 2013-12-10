
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "mgmt_packet.h"


//===============================================================================================

void mgmt_packet::init()
{
    contents = NULL;
    contents_length = 0;
    packet_type = 0;
    objects_count = 0;
}

//--------------------------------------------------------------------------------------------------

mgmt_packet::mgmt_packet(configuration* _conf, unsigned short id, unsigned char type, unsigned char comm)
{
    unsigned char* cptr;
    unsigned short* sptr;
    void* buf;

    init();
    packet_id = id;
    packet_type = type;
    packet_comm = comm;
    
    if(_conf != NULL)
    {
        conf = *_conf;

        if(conf.used[OBJECT_DST_ADDR] == true)
            add_ipaddr(OBJECT_DST_ADDR, conf.dst);
        if(conf.used[OBJECT_SRC_ADDR] == true)
            add_ipaddr(OBJECT_SRC_ADDR, conf.src);
        if(conf.used[OBJECT_OUT_ADDR] == true)
            add_ipaddr(OBJECT_OUT_ADDR, conf.out_if_addr);
        if(conf.used[OBJECT_REFRESH] == true)
            add_unsigned_int(OBJECT_REFRESH,conf.refresh);
        if(conf.used[OBJECT_TB_RATE] == true)
            add_float(OBJECT_TB_RATE,conf.tb_rate);
        if(conf.used[OBJECT_TB_SIZE] == true)
            add_float(OBJECT_TB_SIZE,conf.tb_size);
        if(conf.used[OBJECT_PEAK] == true)
            add_float(OBJECT_PEAK,conf.peak_rate);
        if(conf.used[OBJECT_UP_LABEL] == true)
            add_unsigned_int(OBJECT_UP_LABEL,conf.up_label);
        if(conf.used[OBJECT_LSP_ENC] == true)
            add_unsigned_char(OBJECT_LSP_ENC,conf.enc);
        if(conf.used[OBJECT_LSP_SW] == true)
            add_unsigned_char(OBJECT_LSP_SW,conf.switch_type);
        if(conf.used[OBJECT_LSP_GPID] == true)
            add_unsigned_short(OBJECT_LSP_GPID,conf.gpid);
        if(conf.used[OBJECT_PATH_NAME] == true)
            add_string(OBJECT_PATH_NAME, conf.name, strlen(conf.name));
        if(conf.used[OBJECT_ERO_ADDR] == true)
            add_ipaddr(OBJECT_ERO_ADDR, conf.ero);
        if(conf.used[OBJECT_ERO_PREF] == true)
            add_unsigned_char(OBJECT_ERO_PREF,conf.ero_pref_len);
        if(conf.used[OBJECT_ERO_SL] == true)
            if(conf.ero_sl == true)
                add_unsigned_char(OBJECT_ERO_SL,1);
            else
                add_unsigned_char(OBJECT_ERO_SL,0);
        if(conf.used[OBJECT_HOP_ADDR] == true)
            add_ipaddr(OBJECT_HOP_ADDR, conf.hop);
        if(conf.used[OBJECT_ERROR_ADDR] == true)
            add_ipaddr(OBJECT_ERROR_ADDR, conf.err_node);
        if(conf.used[OBJECT_ERROR_FLAGS] == true)
            add_unsigned_char(OBJECT_ERROR_FLAGS,conf.err_flag);
        if(conf.used[OBJECT_ERROR_CODE] == true)
            add_unsigned_char(OBJECT_ERROR_CODE,conf.err_code);
        if(conf.used[OBJECT_ERROR_VALUE] == true)
            add_unsigned_short(OBJECT_ERROR_VALUE,conf.err_val);
        if(conf.used[OBJECT_MULTI_NUM] == true)
            add_unsigned_int(OBJECT_MULTI_NUM,conf.mpart_number);
        if(conf.used[OBJECT_MULTI_COUNT] == true)
            add_unsigned_int(OBJECT_MULTI_COUNT,conf.mpart_count);
        if(conf.used[OBJECT_TUNNEL_ID] == true)
            add_unsigned_short(OBJECT_TUNNEL_ID,conf.tun_id);
        if(conf.used[OBJECT_TR_D_ADDR] == true)
            add_ipaddr(OBJECT_TR_D_ADDR, conf.traff_dst);
        if(conf.used[OBJECT_TR_RD_ADDR] == true)
            add_ipaddr(OBJECT_TR_RD_ADDR, conf.traff_rev_dst);
        if(conf.used[OBJECT_OUT_IF_HANDLE] == true)
            add_unsigned_int(OBJECT_OUT_IF_HANDLE,conf.interface_id);
        if(conf.used[OBJECT_SRC_TNA] == true)
            add_ipaddr(OBJECT_SRC_TNA, conf.src_tna);
        if(conf.used[OBJECT_DST_TNA] == true)
            add_ipaddr(OBJECT_DST_TNA, conf.dst_tna);
        if(conf.used[OBJECT_CALL_ID_1] == true)
            add_unsigned_int(OBJECT_CALL_ID_1,conf.call_id_1);
        if(conf.used[OBJECT_CALL_ID_2] == true)
            add_unsigned_int(OBJECT_CALL_ID_2,conf.call_id_2);
        if(conf.used[OBJECT_SENDER_LSP_ID] == true)
            add_unsigned_short(OBJECT_SENDER_LSP_ID,conf.lsp_id);
        if(conf.used[OBJECT_SETUP_PRIOR] == true)
            add_unsigned_char(OBJECT_SETUP_PRIOR,conf.setup_prior);
        if(conf.used[OBJECT_HOLD_PRIOR] == true)
            add_unsigned_char(OBJECT_HOLD_PRIOR,conf.hold_prior);
        if(conf.used[OBJECT_PATH_STATE] == true)
            add_unsigned_short(OBJECT_PATH_STATE,conf.path_state);      
        if(conf.used[OBJECT_PATH_SERV_LEVEL] == true)
            add_unsigned_char(OBJECT_PATH_SERV_LEVEL,conf.serv_level);
        if(conf.used[OBJECT_ETHERNET_CIR] == true)
            add_float(OBJECT_ETHERNET_CIR,conf.eth_cir);
        if(conf.used[OBJECT_ETHERNET_CBS] == true)
            add_float(OBJECT_ETHERNET_CBS,conf.eth_cbs);    
        if(conf.used[OBJECT_ETHERNET_EIR] == true)
            add_float(OBJECT_ETHERNET_EIR,conf.eth_eir);
        if(conf.used[OBJECT_ETHERNET_EBS] == true)
            add_float(OBJECT_ETHERNET_EBS,conf.eth_ebs);
        if(conf.used[OBJECT_ETHERNET_SW_GRANUALITY] == true)
            add_unsigned_short(OBJECT_ETHERNET_SW_GRANUALITY,conf.eth_granuality);
        if(conf.used[OBJECT_ETHERNET_INDEX] == true)
            add_unsigned_char(OBJECT_ETHERNET_INDEX,conf.eth_index);
        if(conf.used[OBJECT_ETHERNET_PROFILE] == true)
            add_unsigned_char(OBJECT_ETHERNET_PROFILE,conf.eth_profile);
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

//--------------------------------------------------------------------------------------------------

mgmt_packet::mgmt_packet(char* data, int length)
{
    unsigned char* cbuf;
    unsigned short* sbuf;
    unsigned int* ibuf;
    float* fbuf;
    char* csbuf;
    unsigned short o_type;
    unsigned char o_data_type;
    unsigned char o_size;
    int len, t;
    unsigned int temp;

    init();
    contents = malloc(length);
    contents_length = length;
    memcpy(contents, data, length);

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
        switch(o_type)
        {
        case OBJECT_DST_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.dst = ibuf[1];
            conf.used[OBJECT_DST_ADDR] = true;
            break;
        case OBJECT_SRC_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.src = ibuf[1];
            conf.used[OBJECT_SRC_ADDR] = true;
            break;
        case OBJECT_OUT_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.out_if_addr = ibuf[1];
            conf.used[OBJECT_OUT_ADDR] = true;
            break;
        case OBJECT_REFRESH:
            ibuf = (unsigned int*)cbuf;
            conf.refresh = ntohl(ibuf[1]);
            conf.used[OBJECT_REFRESH] = true;
            break;
        case OBJECT_TB_RATE:
            fbuf = (float*)cbuf;
            conf.tb_rate = fbuf[1];
            conf.used[OBJECT_TB_RATE] = true;
            break;
        case OBJECT_TB_SIZE:
            fbuf = (float*)cbuf;
            conf.tb_size = fbuf[1];
            conf.used[OBJECT_TB_SIZE] = true;
            break;
        case OBJECT_PEAK:
            fbuf = (float*)cbuf;
            conf.peak_rate = fbuf[1];
            conf.used[OBJECT_PEAK] = true;
            break;
        case OBJECT_UP_LABEL:
            ibuf = (unsigned int*)cbuf;
            conf.up_label = ntohl(ibuf[1]);
            conf.used[OBJECT_UP_LABEL] = true;
            break;
        case OBJECT_LSP_ENC:
            conf.enc = cbuf[4];
            conf.used[OBJECT_LSP_ENC] = true;
            break;
        case OBJECT_LSP_SW:
            conf.switch_type = cbuf[4];
            conf.used[OBJECT_LSP_SW] = true;
            break;
        case OBJECT_LSP_GPID:
            conf.gpid = ntohs(sbuf[2]);
            conf.used[OBJECT_LSP_GPID] = true;
            break;
        case OBJECT_PATH_NAME:
            memcpy(&conf.name, cbuf+4, o_size);
            conf.name[o_size] = 0;
            conf.used[OBJECT_PATH_NAME] = true;
            break;
        case OBJECT_ERO_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.ero = ibuf[1];
            conf.used[OBJECT_ERO_ADDR] = true;
            break;
        case OBJECT_ERO_PREF:
            conf.ero_pref_len = cbuf[4];
            conf.used[OBJECT_ERO_PREF] = true;
            break;
        case OBJECT_ERO_SL:
            if(cbuf[4] == 1)
                conf.ero_sl = true;
            else
                conf.ero_sl = false;
            conf.used[OBJECT_ERO_SL] = true;
            break;
        case OBJECT_HOP_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.hop = ibuf[1];
            conf.used[OBJECT_HOP_ADDR] = true;
            break;
        case OBJECT_ERROR_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.err_node = ibuf[1];
            conf.used[OBJECT_ERROR_ADDR] = true;
            break;
        case OBJECT_ERROR_FLAGS:
            conf.err_flag = cbuf[4];
            conf.used[OBJECT_ERROR_FLAGS] = true;
            break;
        case OBJECT_ERROR_CODE:
            conf.err_code = cbuf[4];
            conf.used[OBJECT_ERROR_CODE] = true;
            break;
        case OBJECT_ERROR_VALUE:
            conf.err_val = ntohs(sbuf[2]);
            conf.used[OBJECT_ERROR_VALUE] = true;
            break;
        case OBJECT_MULTI_NUM:
            ibuf = (unsigned int*)cbuf;
            conf.mpart_number = ntohl(ibuf[1]);
            conf.used[OBJECT_MULTI_NUM] = true;
            break;
        case OBJECT_MULTI_COUNT:
            ibuf = (unsigned int*)cbuf;
            conf.mpart_count = ntohl(ibuf[1]);
            conf.used[OBJECT_MULTI_COUNT] = true;
            break;
        case OBJECT_TUNNEL_ID:
            conf.tun_id = ntohs(sbuf[2]);
            conf.used[OBJECT_TUNNEL_ID] = true;
            break;
        case OBJECT_TR_D_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.traff_dst = ibuf[1];
            conf.used[OBJECT_TR_D_ADDR] = true;
            break;
        case OBJECT_TR_RD_ADDR:
            ibuf = (unsigned int*)cbuf;
            conf.traff_rev_dst = ibuf[1];
            conf.used[OBJECT_TR_RD_ADDR] = true;
            break;
        case OBJECT_OUT_IF_HANDLE:
            ibuf = (unsigned int*)cbuf;
            conf.interface_id = ntohl(ibuf[1]);
            conf.used[OBJECT_OUT_IF_HANDLE] = true;
            break;
        case OBJECT_SRC_TNA:
            ibuf = (unsigned int*)cbuf;
            conf.src_tna = ibuf[1];
            conf.used[OBJECT_SRC_TNA] = true;
            break;
        case OBJECT_DST_TNA:
            ibuf = (unsigned int*)cbuf;
            conf.dst_tna = ibuf[1];
            conf.used[OBJECT_DST_TNA] = true;
            break;
        case OBJECT_CALL_ID_1:
            ibuf = (unsigned int*)cbuf;
            conf.call_id_1 = ntohl(ibuf[1]);
            conf.used[OBJECT_CALL_ID_1] = true;
            break;
        case OBJECT_CALL_ID_2:
            ibuf = (unsigned int*)cbuf;
            conf.call_id_2 = ntohl(ibuf[1]);
            conf.used[OBJECT_CALL_ID_2] = true;
            break;
        case OBJECT_SENDER_LSP_ID:
            conf.lsp_id = ntohs(sbuf[2]);
            conf.used[OBJECT_SENDER_LSP_ID] = true;
            break;
        case OBJECT_SETUP_PRIOR:
            conf.setup_prior = cbuf[4];
            conf.used[OBJECT_SETUP_PRIOR] = true;
            break;
        case OBJECT_HOLD_PRIOR:
            conf.hold_prior = cbuf[4];
            conf.used[OBJECT_HOLD_PRIOR] = true;
            break;
        case OBJECT_PATH_STATE:
            conf.path_state = ntohs(sbuf[2]);
            conf.used[OBJECT_PATH_STATE] = true;
            break;   
        case OBJECT_PATH_SERV_LEVEL:
            conf.serv_level = cbuf[4];
            conf.used[OBJECT_PATH_SERV_LEVEL] = true;
            break;    
        case OBJECT_ETHERNET_CIR:
            fbuf = (float*)cbuf;
            conf.eth_cir = fbuf[1];
            conf.used[OBJECT_ETHERNET_CIR] = true;
            break;        
        case OBJECT_ETHERNET_CBS:
            fbuf = (float*)cbuf;
            conf.eth_cbs = fbuf[1];
            conf.used[OBJECT_ETHERNET_CBS] = true;
            break;
        case OBJECT_ETHERNET_EIR:
            fbuf = (float*)cbuf;
            conf.eth_eir = fbuf[1];
            conf.used[OBJECT_ETHERNET_EIR] = true;
            break;
        case OBJECT_ETHERNET_EBS:
            fbuf = (float*)cbuf;
            conf.eth_ebs = fbuf[1];
            conf.used[OBJECT_ETHERNET_EBS] = true;
            break;
        case OBJECT_ETHERNET_SW_GRANUALITY:
            conf.eth_granuality = ntohs(sbuf[2]);
            conf.used[OBJECT_ETHERNET_SW_GRANUALITY] = true;
            break;
        case OBJECT_ETHERNET_INDEX:
            conf.eth_index = cbuf[4];
            conf.used[OBJECT_ETHERNET_INDEX] = true;
            break;
        case OBJECT_ETHERNET_PROFILE:
            conf.eth_profile = cbuf[4];
            conf.used[OBJECT_ETHERNET_PROFILE] = true;
            break;

        default:
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
}

//--------------------------------------------------------------------------------------------------

mgmt_packet::~mgmt_packet()
{
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
