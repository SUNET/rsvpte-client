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
 
#ifndef RSVP_CONFIGURATION_HEADER_FILE
#define RSVP_CONFIGURATION_HEADER_FILE

class rsvp_configuration;

#include <string.h>
#include <stdlib.h>
#include "verbose.h"
#include "rsvp_error.h"
#include "configuration.h"

#define RC_SESSION_DEST_ADDR            1  // UI
#define RC_TUNNEL_ID                    2  // US
#define RC_EXTENDED_IPV4_ADDR           3  // UI
#define RC_CONTROL_OUT_IF_ADDR          4  // UI
#define RC_CONTROL_OUT_IF_HANDLE        5  // UI
#define RC_REFRESH_PERIOD               6  // UI
#define RC_SCOPE_ADDR                   7  // UI | TAB
#define RC_WILDCARD_FILTER              8  // UC  // all three flags
#define RC_FIXED_FILTER                 9  // UC  // with positive values
#define RC_SHARED_EXPLICIT              10 // UC  // indicate an unknown option vector value
#define RC_TOKEN_BUCKET_RATE            11 // F
#define RC_TOKEN_BUCKET_SIZE            12 // F
#define RC_PEAK_DATA_RATE               13 // F
#define RC_MINIMUM_POLICED_UNIT         14 // UI
#define RC_MAXIMUM_PACKET_SIZE          15 // UI
#define RC_RATE                         16 // F
#define RC_SLACK_TERM                   17 // UI
#define RC_FILTER_SRC_ADDR              18 // UI
#define RC_FILTER_SRC_PORT              19 // US
#define RC_SENDER_SRC_ADDR              20 // UI
#define RC_SENDER_SRC_ID                21 // US
#define RC_RESV_CONFIRM_RECV_ADDR       22 // UI
#define RC_LABEL                        23 // VS
#define RC_LSP_ENC_TYPE                 24 // UC
#define RC_LSP_SWITCHING_TYPE           25 // UC
#define RC_LSP_GPID                     26 // US
#define RC_HELLO_SRC_INSTANCE           27 // UI
#define RC_HELLO_DST_INSTANCE           28 // UI
#define RC_HELLO_REQ_ACK                29 // UC  // REQ = 1, ACK = 2
#define RC_RESTART_TIME                 30 // UI
#define RC_RECOVERY_TIME                31 // UI
#define RC_SETUP_PRIORITY               32 // UC
#define RC_HOLDING_PRIORITY             33 // UC
#define RC_LOCAL_PROTECTION_FLAG        34 // UC
#define RC_LABEL_RECORDING_FLAG         35 // UC
#define RC_SE_STYLE_FLAG                36 // UC
#define RC_SESSION_NAME                 37 // CS
#define RC_ERO                          38 // VS | TAB
struct rc_ero
{
    unsigned int address;
    unsigned char prefix_length;
    unsigned char loose;
};
// todo what to do with this? vvvvvvvv - disabled
#define RC_RSVP_HOP_TLVS                39 // UI | TAB - nieużywane - używane do CLASS_IPV4_IF_ID_RSVP_HOP
#define RC_ERROR_NODE_ADDR              40 // UI
#define RC_ERROR_FLAGS                  41 // UC
#define RC_ERROR_CODE                   42 // UC
#define RC_ERROR_VALUE                  43 // US
#define RC_SSDH_SIGNAL_TYPE             44 // UC
#define RC_SSDH_RCC                     45 // UC
#define RC_SSDH_NCC                     46 // US
#define RC_SSDH_NVC                     47 // US
#define RC_SSDH_MULTIPLIER              48 // US
#define RC_SSDH_TRANSPARENCY            49 // UI
#define RC_SSDH_PROFILE                 50 // UI
#define RC_MSG_ID_FLAGS                 51 // UC
#define RC_MSG_ID_EPOCH                 52 // UI (24 lowest bits used)
#define RC_MSG_ID_ID                    53 // UI
#define RC_MSG_ID_ACK_NACK              54 // UC  // ACK = 1, NACK = 2
#define RC_UPSTREAM_LABEL               55 // VS
#define RC_ADMIN_STATUS_RTAD_FLAGS      56 // UC  // four lower bits get values of RTAD flags of ADMIN_STATUS Object
#define RC_TNA_SRC_ADDR                 57 // UI
#define RC_TNA_DST_ADDR                 58 // UI
#define RC_DATA_OUT_IF_ADDR             59 // UI
#define RC_DATA_OUT_IF_HANDLE           60 // UI
#define RC_DATA_IN_IF_ADDR              61 // UI
#define RC_DATA_IN_IF_HANDLE            62 // UI
#define RC_TR_D_ADDR                    63 // UI
#define RC_TR_RD_ADDR                   64 // UI
#define RC_CALL_ID                      65 // UI
#define RC_CALL_ID_ADDR_TYPE            66 // UC
#define RC_CALL_ID_SRC_TN_ADDR          67 // UI
#define RC_CALL_ID_LOCAL_ID1            68 // UI
#define RC_CALL_ID_LOCAL_ID2            69 // UI
#define RC_CALL_ID_IS                   70 // UI  // only 3 lower bytes are used, the rest is ignored
#define RC_CALL_ID_ITU_CC1              71 // UI
#define RC_CALL_ID_ITU_CC2              72 // US
#define RC_CALL_ID_UAPC1                73 // US
#define RC_CALL_ID_UAPC2                74 // UI
#define RC_PATH_STATUS                  75 // UI // status of the path -for status reporting purpose
#define RC_SERVICE_LEVEL                76 // UC
// please see http://www.ietf.org/internet-drafts/draft-ietf-ccamp-ethernet-traffic-parameters-02.txt
#define RC_ETHERNET_CIR                 77 // F  
#define RC_ETHERNET_CBS                 78 // F
#define RC_ETHERNET_EIR                 79 // F
#define RC_ETHERNET_EBS                 80 // F
#define RC_ETHERNET_SW_GRANUALITY       81 // US
#define RC_ETHERNET_INDEX               82 // UC
#define RC_ETHERNET_PROFILE             83 // UC
#define RC_SREFRESH_MESSAGE_ID          84 // UI // mulitiple used field
#define RC_NOTIFY_ADDRESS               85 // UI

#define PT_UC   1
#define PT_US   2
#define PT_UI   4
#define PT_CS   8
#define PT_VS   16
#define PT_F    32

#define PT_TAB  128

class rsvp_configuration
{
private:
    void* data[256];
    int void_size[256];
    unsigned char parameter_types[256];

public:
    rsvp_configuration();
    rsvp_configuration(const rsvp_configuration& old);
    rsvp_configuration& operator=(const rsvp_configuration& old);
    ~rsvp_configuration();
    int add(unsigned char parameter, unsigned char value);
    int add(unsigned char parameter, unsigned short value);
    int add(unsigned char parameter, unsigned int value);
    int add(unsigned char parameter, const char* value);
    int add(unsigned char parameter, void* value, int size);
    int add(unsigned char parameter, float value);

    int set(unsigned char parameter, unsigned char value, int index = 0);
    int set(unsigned char parameter, unsigned short value, int index = 0);
    int set(unsigned char parameter, unsigned int value, int index = 0);
    int set(unsigned char parameter, char* value, int index = 0);
    int set(unsigned char parameter, void* value, int size, int index = 0);
    int set(unsigned char parameter, float value, int index = 0);

    int setadd(unsigned char parameter, unsigned char value, int index = 0);
    int setadd(unsigned char parameter, unsigned short value, int index = 0);
    int setadd(unsigned char parameter, unsigned int value, int index = 0);
    int setadd(unsigned char parameter, char* value, int index = 0);
    int setadd(unsigned char parameter, void* value, int size, int index = 0);
    int setadd(unsigned char parameter, float value, int index = 0);

    int remove(unsigned char parameter, int index = 0);
    int ispresent(unsigned char parameter, int index = 0);
    void* read(unsigned char parameter, int index = 0, int *size = NULL);
    int copy_from(unsigned char parameter, rsvp_configuration conf);
    int add_missing(rsvp_configuration add_from);
    int update(rsvp_configuration add_from);
};

#endif
