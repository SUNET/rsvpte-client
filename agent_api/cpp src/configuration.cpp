
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include "configuration.h"


char* int_2_addr(unsigned int addr)
{
    struct in_addr in;
    in.s_addr = addr;
    return strdup(inet_ntoa(in));
}

configuration::configuration()
{
    /* initialize all objects */
    dst = 0;
    src = 0;
    out_if_addr = 0;
    refresh = 0;
    tb_rate = 0.0;
    tb_size = 0.0;
    peak_rate = 0.0;
    up_label = 0;
    enc = 0;
    switch_type = 0;
    gpid = 0;
    name[0] = 0;
    ero = 0;
    ero_pref_len = 0;
    ero_sl = false;
    hop = 0;
    err_node = 0;
    err_flag = 0;
    err_code = 0;
    err_val = 0;
    mpart_number = 0;
    mpart_count = 0;
    tun_id = 0;
    traff_dst = 0;
    traff_rev_dst = 0;
    interface_id = 0;
    src_tna = 0;
    dst_tna = 0;
    call_id_1 = 0;
    call_id_2 = 0;
    lsp_id = 0;
    setup_prior = 0;
    hold_prior = 0;
    path_state = 0;
    serv_level = 0;
    eth_cir = 0;
    eth_cbs = 0;
    eth_eir = 0;
    eth_ebs = 0;
    eth_granuality = 0;
    eth_index = 0;
    eth_profile = 0;
   
    // iterate over all objects ids
    for (int i=1; i<OBJECT_COUNT+1; i++)
        used[i] = false;  // by the default all objects are not used
} 

void configuration::print()
{
   if(used[OBJECT_DST_ADDR] == true)
       printf("  Dst address: %s\n", int_2_addr(dst));
   if(used[OBJECT_SRC_ADDR] == true)
       printf("  Src address: %s\n", int_2_addr(src));
   if(used[OBJECT_OUT_ADDR] == true)
       printf("  Out if address: %s\n", int_2_addr(out_if_addr));
   if(used[OBJECT_REFRESH] == true)
       printf("  Refresh: %i\n", refresh);
   if(used[OBJECT_TB_RATE] == true)
       printf("  TB rate: %f\n", tb_rate);
   if(used[OBJECT_TB_SIZE] == true)
       printf("  TB size: %f\n", tb_size);
   if(used[OBJECT_PEAK] == true)
       printf("  Peak rate: %f\n", peak_rate);
   if(used[OBJECT_UP_LABEL] == true)
       printf("  Up label: %i\n", up_label); 
   if(used[OBJECT_LSP_ENC] == true)
       printf("  Encoding: %i\n", enc);
   if(used[OBJECT_LSP_SW] == true)
       printf("  Switching type: %i\n", switch_type);
   if(used[OBJECT_LSP_GPID] == true)
       printf("  Gpid: %i\n", gpid);
   if(used[OBJECT_PATH_NAME] == true)
       printf("  Path name: %s\n", name);
   if(used[OBJECT_ERO_ADDR] == true)
       printf("  ERO address: %s\n", int_2_addr(ero)); 
   if(used[OBJECT_ERO_PREF] == true)
       printf("  ERO prefix length: %i\n", ero_pref_len);
   if(used[OBJECT_ERO_SL] == true)
       printf("  ERO strict/loose: %i\n", (int)ero_sl);
   if(used[OBJECT_HOP_ADDR] == true)
       printf("  Hop address: %s\n", int_2_addr(hop));
   if(used[OBJECT_ERROR_ADDR] == true)
       printf("  Error node address: %s\n", int_2_addr(err_node));
   if(used[OBJECT_ERROR_FLAGS] == true)
       printf("  Error flag: %i\n", err_flag);
   if(used[OBJECT_ERROR_CODE] == true)
       printf("  Error code: %i\n", err_code);
   if(used[OBJECT_ERROR_VALUE] == true)
       printf("  Error value: %i\n", err_val);
   if(used[OBJECT_MULTI_NUM] == true)
       printf("  Mpart number: %i\n", mpart_number);
   if(used[OBJECT_MULTI_COUNT] == true)
       printf("  Mpart count: %i\n", mpart_count);
   if(used[OBJECT_TUNNEL_ID] == true)
       printf("  Tunnel ID: %i\n", tun_id);
   if(used[OBJECT_TR_D_ADDR] == true)
       printf("  Traffic dest address: %s\n", int_2_addr(traff_dst));
   if(used[OBJECT_TR_RD_ADDR] == true)
       printf("  Reverse trafic dest address: %s\n", int_2_addr(traff_rev_dst));
   if(used[OBJECT_OUT_IF_HANDLE] == true)
       printf("  Interface ID: %i\n", interface_id);
   if(used[OBJECT_SRC_TNA] == true)
       printf("  Src TNA: %s\n", int_2_addr(src_tna));
   if(used[OBJECT_DST_TNA] == true)
       printf("  Dst TNA: %s\n", int_2_addr(dst_tna));
   if(used[OBJECT_CALL_ID_1] == true)
       printf("  Call ID 1: %i\n", call_id_1);
   if(used[OBJECT_CALL_ID_2] == true)
       printf("  Call ID 2: %i\n", call_id_2);
   if(used[OBJECT_SENDER_LSP_ID] == true)
       printf("  Sender LSP ID: %i\n", lsp_id);
   if(used[OBJECT_SETUP_PRIOR] == true)
       printf("  Setup priorited: %i\n", setup_prior);
   if(used[OBJECT_HOLD_PRIOR] == true)
       printf("  Hold priorited: %i\n", hold_prior); 
   if(used[OBJECT_PATH_STATE] == true)
       printf("  Path state: %i\n", path_state); 
   if(used[OBJECT_PATH_SERV_LEVEL] == true)
       printf("  Path service level: %i\n", serv_level);
   if(used[OBJECT_ETHERNET_CIR] == true)
       printf("  Ethernet CIR: %f\n", eth_cir);
   if(used[OBJECT_ETHERNET_CBS] == true)
       printf("  Ethernet CBS: %f\n", eth_cbs);
   if(used[OBJECT_ETHERNET_EIR] == true)
       printf("  Ethernet EIR: %f\n", eth_eir);
   if(used[OBJECT_ETHERNET_EBS] == true)
       printf("  Ethernet EBS: %f\n", eth_ebs);
   if(used[OBJECT_ETHERNET_SW_GRANUALITY] == true)
       printf("  Ethernet granuality: %i\n", eth_granuality);
   if(used[OBJECT_ETHERNET_INDEX] == true)
       printf("  Ethernet index: %i\n", eth_index);
   if(used[OBJECT_ETHERNET_PROFILE] == true)
       printf("  Ethernet profile: %i\n", eth_profile);
   printf("\n");
}


