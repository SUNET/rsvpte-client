#ifndef CONFIGURATION_HEADER_FILE
#define CONFIGURATION_HEADER_FILE 

//-------------------------------------------------------------------------------------------
// configuration's objects ids

/*      object ID name          ID     // data_type      <----------- object description --------------> */

#define OBJECT_DST_ADDR         1      // 3               rsvp session destination address
#define OBJECT_SRC_ADDR         2      // 3               rsvp session source address
#define OBJECT_OUT_ADDR         3      // 3               agent's outgoing interface (if many)
#define OBJECT_REFRESH          4      // 3               path refresh period
#define OBJECT_TB_RATE          5      // 5               token bucket rate
#define OBJECT_TB_SIZE          6      // 5               token bucket size
#define OBJECT_PEAK             7      // 5               peak data rate
#define OBJECT_UP_LABEL         8      // 3               upstream label (for bidirectional paths)
#define OBJECT_LSP_ENC          9      // 1               lsp encoding type
#define OBJECT_LSP_SW           10     // 1               lsp switching type
#define OBJECT_LSP_GPID         11     // 2               lsp gpid
#define OBJECT_PATH_NAME        12     // 6               path name
#define OBJECT_ERO_ADDR         13     // 3               ERO address
#define OBJECT_ERO_PREF         14     // 1               ERO prefix length
#define OBJECT_ERO_SL           15     // 1               ERO strict/loose
#define OBJECT_HOP_ADDR         16     // 3               RSVP Hop address (TLVs, because first hop is in OBJECT_OUT_ADDR)
#define OBJECT_ERROR_ADDR       17     // 3               Error node address
#define OBJECT_ERROR_FLAGS      18     // 1               error flags
#define OBJECT_ERROR_CODE       19     // 1               error code
#define OBJECT_ERROR_VALUE      20     // 2               error value
#define OBJECT_MULTI_NUM        21     // 3               multiple part packet - number of packet (0..N-1)
#define OBJECT_MULTI_COUNT      22     // 3               multiple part packet - total count of packets (N)
#define OBJECT_TUNNEL_ID        23     // 2               tunnel id
#define OBJECT_TR_D_ADDR        24     // 3               traffic destination address
#define OBJECT_TR_RD_ADDR       25     // 3               traffic reverse destination address
#define OBJECT_OUT_IF_HANDLE    26     // 3               data forward/reverse interface id
#define OBJECT_SRC_TNA          27     // 3               data source TNA
#define OBJECT_DST_TNA          28     // 3               data destination TNA
#define OBJECT_CALL_ID_1        29     // 3               call id - part 1
#define OBJECT_CALL_ID_2        30     // 3               call id - part 2
#define OBJECT_SENDER_LSP_ID    31     // 2               sender LSP id
#define OBJECT_SETUP_PRIOR      32     // 1               setup priority
#define OBJECT_HOLD_PRIOR       33     // 1               hold priority
#define OBJECT_PATH_STATE       34     // 2               status of the path - for status reporting purpose: STATUS ACK
#define OBJECT_PATH_SERV_LEVEL  35     // 1               protection service level
#define OBJECT_ETHERNET_CIR            36    // 5         ethernet committed information rate 
#define OBJECT_ETHERNET_CBS            37    // 5         ethernet committed burst size
#define OBJECT_ETHERNET_EIR            38    // 5         ethernet excess information rate
#define OBJECT_ETHERNET_EBS            39    // 5         ethernet committed burst size
#define OBJECT_ETHERNET_SW_GRANUALITY  40    // 2         type of ethernet link
#define OBJECT_ETHERNET_INDEX          41    // 1         index of bandwidth allocated for given QoS
#define OBJECT_ETHERNET_PROFILE        42    // 1         flags of color mode and coupling

#define OBJECT_COUNT 42

//===============================================================================================

class configuration
{
public:
   /* configuration's objects       //<------ object ID ------> data_type  # <----------- object description --------------> */

   unsigned int dst;                //OBJECT_DST_ADDR         1   3        # rsvp session destination address
   unsigned int src;                //OBJECT_SRC_ADDR         2   3        # rsvp session source address   (extended tunnel id)
   unsigned int out_if_addr;        //OBJECT_OUT_ADDR         3   3        # agent's outgoing interface (HOP neighbor address)
   unsigned int refresh;            //OBJECT_REFRESH          4   3        # path refresh period
   float tb_rate;                   //OBJECT_TB_RATE          5   5        # token bucket rate
   float tb_size;                   //OBJECT_TB_SIZE          6   5        # token bucket size
   float peak_rate;                 //OBJECT_PEAK             7   5        # peak data rate
   unsigned int up_label;           //OBJECT_UP_LABEL         8   3        # upstream label (for bidirectional paths)
   unsigned char enc;               //OBJECT_LSP_ENC          9   1        # lsp encoding type
   unsigned char switch_type;       //OBJECT_LSP_SW           10  1        # lsp switching type
   unsigned short gpid;             //OBJECT_LSP_GPID         11  2        # lsp gpid
   char name[50];                   //OBJECT_PATH_NAME        12  6        # path name
   unsigned int ero;                //OBJECT_ERO_ADDR         13  3        # ERO address
   unsigned char ero_pref_len;      //OBJECT_ERO_PREF         14  1        # ERO prefix length
   bool ero_sl;                     //OBJECT_ERO_SL           15  1        # ERO strict/loose
   unsigned int hop;                //OBJECT_HOP_ADDR         16  3        # RSVP Hop address (not usable)
   unsigned int err_node;           //OBJECT_ERROR_ADDR       17  3        # error node address
   unsigned char err_flag;          //OBJECT_ERROR_FLAGS      18  1        # error flags
   unsigned char err_code;          //OBJECT_ERROR_CODE       19  1        # error code
   unsigned short err_val;          //OBJECT_ERROR_VALUE      20  2        # error value
   unsigned int mpart_number;       //OBJECT_MULTI_NUM        21  3        # multiple part message - packet number (0..N-1) 
   unsigned int mpart_count;        //OBJECT_MULTI_COUNT      22  3        # multiple part message - total packet count (N)
   unsigned short tun_id;           //OBJECT_TUNNEL_ID        23  2        # tunnel id
   unsigned int traff_dst;          //OBJECT_TR_D_ADDR        24  3        # traffic destination address 
   unsigned int traff_rev_dst;      //OBJECT_TR_RD_ADDR       25  3        # traffic reverse destination address 
   unsigned int interface_id;       //OBJECT_OUT_IF_HANDLE    26  3        # data forward/reverse interface id
   unsigned int src_tna;            //OBJECT_SRC_TNA          27  3        # source TNA
   unsigned int dst_tna;            //OBJECT_DST_TNA          28  3        # destination TNA
   unsigned int call_id_1;          //OBJECT_CALL_ID_1        29  3        # call id - part 1
   unsigned int call_id_2;          //OBJECT_CALL_ID_2        30  3        # call id - part 2
   unsigned short lsp_id;           //OBJECT_SENDER_LSP_ID    31  2        # sender LSP id
   unsigned char setup_prior;       //OBJECT_SETUP_PRIOR      32  1        # setup priority
   unsigned char hold_prior;        //OBJECT_HOLD_PRIOR       33  1        # hold priority
   unsigned short path_state;       //OBJECT_PATH_STATE       34  2        # path state (STATUS ACK only)
   unsigned char serv_level;        //OBJECT_PATH_SERV_LEVEL  35  1        # protection service level
   float eth_cir;                   //OBJECT_ETHERNET_CIR            36 5  # ethernet committed information rate 
   float eth_cbs;                   //OBJECT_ETHERNET_CBS            37 5  # ethernet committed burst size
   float eth_eir;                   //OBJECT_ETHERNET_EIR            38 5  # ethernet excess information rate
   float eth_ebs;                   //OBJECT_ETHERNET_EBS            39 5  # ethernet committed burst size
   unsigned short eth_granuality;   //OBJECT_ETHERNET_SW_GRANUALITY  40 2  # type of ethernet link
   unsigned char eth_index;         //OBJECT_ETHERNET_INDEX          41 1  # index of bandwidth allocated for given QoS
   unsigned char eth_profile;       //OBJECT_ETHERNET_PROFILE        42 1  # flags of color mode and coupling
   
   /* determine which objects are used (are active) */
   bool used[OBJECT_COUNT+1]; // for each object id: true if object is used otherwsie false 

   /* constructor */
   configuration();

   /* print configuration */
   void print();
};

#endif /* CONFIGURATION_HEADER_FILE */
