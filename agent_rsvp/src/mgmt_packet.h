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
 
#ifndef MGMT_PACKET_HEADER_FILE
#define MGMT_PACKET_HEADER_FILE

class mgmt_packet;

#include <netinet/in.h>
#include <string.h>
#include "rsvp_configuration.h"
#include "rsvp_error.h"
#include "verbose.h"
#include "configuration.h"

#define TYPE_REQ        1
#define TYPE_ACK        2
#define TYPE_NACK       3

#define COMMAND_CREATE  1
#define COMMAND_DELETE  2
#define COMMAND_STATUS  3
#define COMMAND_TRANSIT 4

#define DATA_TYPE_UC    1
#define DATA_TYPE_US    2
#define DATA_TYPE_UI    3
#define DATA_TYPE_UL    4
#define DATA_TYPE_F     5
#define DATA_TYPE_CS    6
#define DATA_TYPE_V     7

#define OBJECT_DST_ADDR     1  //     3               rsvp session destination address
#define OBJECT_SRC_ADDR     2  //     3               rsvp session source address
#define OBJECT_OUT_ADDR     3  //     3               agent's outgoing interface (if many)
#define OBJECT_REFRESH      4  //     3               path refresh period
#define OBJECT_TB_RATE      5  //     5               token bucket rate
#define OBJECT_TB_SIZE      6  //     5               token bucket size
#define OBJECT_PEAK         7  //     5               peak data rate
#define OBJECT_UP_LABEL     8  //     3               upstream label (for bidirectional paths)
#define OBJECT_LSP_ENC      9  //     1               lsp encoding type
#define OBJECT_LSP_SW       10 //     1               lsp switching type
#define OBJECT_LSP_GPID     11 //     2               lsp gpid
#define OBJECT_PATH_NAME    12 //     6               path name
#define OBJECT_ERO_ADDR     13 //     3               ERO address
#define OBJECT_ERO_PREF     14 //     1               ERO prefix length
#define OBJECT_ERO_SL       15 //     1               ERO strict/loose
#define OBJECT_HOP_ADDR     16 //     3               RSVP Hop address (TLVs, because first hop is in OBJECT_OUT_ADDR)
#define OBJECT_ERROR_ADDR   17 //     3               Error node address
#define OBJECT_ERROR_FLAGS  18 //     1               error flags
#define OBJECT_ERROR_CODE   19 //     1               error code
#define OBJECT_ERROR_VALUE  20 //     2               error value
#define OBJECT_MULTI_NUM    21 //     3               multiple part packet - number of packet (0..N-1)
#define OBJECT_MULTI_COUNT  22 //     3               multiple part packet - total count of packets (N)
#define OBJECT_TUNNEL_ID    23 //     2               tunnel id
#define OBJECT_TR_D_ADDR    24 //     3               traffic destination address
#define OBJECT_TR_RD_ADDR   25 //     3               traffic reverse destination address
#define OBJECT_OUT_IF_HANDLE    26 //     3               data forward/reverse interface id
#define OBJECT_SRC_TNA          27 //     3               data source TNA
#define OBJECT_DST_TNA          28 //     3               data destination TNA
#define OBJECT_CALL_ID_1        29 //     3               call id - part 1
#define OBJECT_CALL_ID_2        30 //     3               call id - part 2
#define OBJECT_SENDER_LSP_ID    31 //     2               sender LSP id
#define OBJECT_SETUP_PRIOR      32 //     1               setup priority
#define OBJECT_HOLD_PRIOR       33 //     1               hold priority
#define OBJECT_PATH_STATE       34 //     2               status of the path - for status reporting purpose: STATUS ACK
#define OBJECT_PATH_SERV_LEVEL  35 //     1               protection service level
#define OBJECT_ETHERNET_CIR            36 //    5         ethernet committed information rate 
#define OBJECT_ETHERNET_CBS            37 //    5         ethernet committed burst size
#define OBJECT_ETHERNET_EIR            38 //    5         ethernet excess information rate
#define OBJECT_ETHERNET_EBS            39 //    5         ethernet committed burst size
#define OBJECT_ETHERNET_SW_GRANUALITY  40 //    2         type of ethernet link
#define OBJECT_ETHERNET_INDEX          41 //    1         index of bandwidth allocated for given QoS
#define OBJECT_ETHERNET_PROFILE        42 //    1         flags of color mode and coupling

class mgmt_packet
{
    private:
        void* contents;
        int contents_length;
        int objects_count;
        rsvp_configuration* configuration;
        unsigned char packet_type;
        unsigned char packet_comm;
        unsigned short packet_id;

        /* add specific object types to packet */
        int add_unsigned_int(unsigned short id, unsigned int value);
        int add_ipaddr(unsigned short id, unsigned int addr);
        int add_unsigned_short(unsigned short id, unsigned short value);
        int add_unsigned_char(unsigned short id, unsigned char value);
        int add_float(unsigned short id, float value);
        int add_string(unsigned short id, const char* name, int size);


        void init();

    public:
        mgmt_packet(rsvp_configuration* conf, unsigned short id, unsigned char type, unsigned char comm, int multi_num = -1, int multi_count = -1);
	mgmt_packet(unsigned short id, unsigned char type, unsigned char comm);
        void* get_data(int* size);

        mgmt_packet(char* cont, int length);
        rsvp_configuration get_conf();
        unsigned char get_type();
        unsigned char get_comm();
        unsigned short get_id();

        ~mgmt_packet();
};

#endif
