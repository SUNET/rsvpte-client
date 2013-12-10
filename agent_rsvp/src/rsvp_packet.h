
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
 
#ifndef RSVP_PACKET_HEADER_FILE
#define RSVP_PACKET_HEADER_FILE

class rsvp_packet;

#include <math.h>
#include "floatformat.h"
#include "rsvp_configuration.h"
#include <netinet/in.h>
#include "configuration.h"
#include "tools.h"

#define MAX_OBJECTS 100
#define MAX_REQS 100
#define NUM_OBJECTS 37

											//ObjectClass
											    //ObjectClassType
												   //SubobjectClass
													  //SubobjectClassType
                                                          //description string
#define CLASS_SESSION_IETF                    1, 7, 0, 0, "CLASS_SESSION_IETF"
#define CLASS_SESSION_OIF                     1,11, 0, 0, "CLASS_SESSION_OIF"
#define CLASS_IPV4_RSVP_HOP                   3, 1, 0, 0, "CLASS_IPV4_RSVP_HOP"
#define CLASS_IPV4_IF_ID_RSVP_HOP             3, 3, 0, 0, "CLASS_IPV4_IF_ID_RSVP_HOP"
#define CLASS_TIME_VALUES                     5, 1, 0, 0, "CLASS_TIME_VALUES"
#define CLASS_ERROR_SPEC                      6, 1, 0, 0, "CLASS_ERROR_SPEC"
#define CLASS_SCOPE                           7, 1, 0, 0, "CLASS_SCOPE"
#define CLASS_STYLE                           8, 1, 0, 0, "CLASS_STYLE"
#define CLASS_TOKEN_FLOWSPEC                  9, 2, 0, 0, "CLASS_TOKEN_FLOWSPEC"
#define CLASS_SONETSDH_FLOWSPEC               9, 4, 0, 0, "CLASS_SONETSDH_FLOWSPEC"
#define CLASS_ETHERNET_FLOWSPEC               9, 6, 0, 0, "CLASS_ETHERNET_FLOWSPEC"
#define CLASS_FILTER_SPEC                    10, 7, 0, 0, "CLASS_FILTER_SPEC"
#define CLASS_SENDER_TEMPLATE                11, 7, 0, 0, "CLASS_SENDER_TEMPLATE"
#define CLASS_TOKEN_SENDER_TSPEC             12, 2, 0, 0, "CLASS_TOKEN_SENDER_TSPEC"
#define CLASS_SONETSDH_SENDER_TSPEC          12, 4, 0, 0, "CLASS_SONETSDH_SENDER_TSPEC"
#define CLASS_ETHERNET_SENDER_TSPEC          12, 6, 0, 0, "CLASS_ETHERNET_SENDER_TSPEC"
#define CLASS_RESV_CONFIRM                   15, 1, 0, 0, "CLASS_RESV_CONFIRM"
#define CLASS_GENERALIZED_LABEL              16, 2, 0, 0, "CLASS_GENERALIZED_LABEL"
#define CLASS_GENERALIZED_LABEL_REQUEST      19, 4, 0, 0, "CLASS_GENERALIZED_LABEL_REQUEST"
#define CLASS_ERO                            20, 1, 0, 0, "CLASS_ERO"
#define CLASS_HELLO_REQUEST                  22, 1, 0, 0, "CLASS_HELLO_REQUEST"
#define CLASS_HELLO_ACK                      22, 2, 0, 0, "CLASS_HELLO_ACK"
#define CLASS_MESSAGE_ID                     23, 1, 0, 0, "CLASS_MESSAGE_ID"
#define CLASS_MESSAGE_ID_ACK                 24, 1, 0, 0, "CLASS_MESSAGE_ID_ACK"
#define CLASS_MESSAGE_ID_NACK                24, 2, 0, 0, "CLASS_MESSAGE_ID_NACK"
#define CLASS_MESSAGE_ID_LIST                25, 1, 0, 0, "CLASS_MESSAGE_ID_LIST"
#define CLASS_UPSTREAM_LABEL                 35, 2, 0, 0, "CLASS_UPSTREAM_LABEL"
#define CLASS_RESTART_CAP                   131, 1, 0, 0, "CLASS_RESTART_CAP"
#define CLASS_NOTIFY_REQUEST                195, 1, 0, 0, "CLASS_NOTIFY_REQUEST"
#define CLASS_ADMIN_STATUS                  196, 1, 0, 0, "CLASS_ADMIN_STATUS"
#define CLASS_SESSION_ATTRIBUTE             207, 7, 0, 0, "CLASS_SESSION_ATTRIBUTE"
#define CLASS_GENERALIZED_UNI               229, 1, 0, 0, "CLASS_GENERALIZED_UNI"
#define CLASS_GU_SOURCE_TNA_ADDRESS         229, 1, 1, 1, "CLASS_GU_SOURCE_TNA_ADDRESS"
#define CLASS_GU_DESTINATION_TNA_ADDRESS    229, 1, 2, 1, "CLASS_GU_DESTINATION_TNA_ADDRESS"
#define CLASS_GU_SERVICE_LEVEL              229, 1, 5, 1, "CLASS_GU_SERVICE_LEVEL"
#define CLASS_CALL_ID_0                     230, 0, 0, 0, "CLASS_CALL_ID_0"
#define CLASS_CALL_ID_1                     230, 1, 0, 0, "CLASS_CALL_ID_1"
#define CLASS_CALL_ID_2                     230, 2, 0, 0, "CLASS_CALL_ID_2"


struct IPHeader
{
    unsigned char version_and_header_length;  // HL is in 32-bit words, not 8-bit octets! Multiply by 4!
    unsigned char TOS;
    unsigned short total_length;
    unsigned short identification;
    unsigned short flags_and_fragment_offset;
    unsigned char TTL;
    unsigned char protocol;
    unsigned short checksum;
    unsigned int source_address;
    unsigned int destination_address;
    // rest of the header is variable
};

class rsvp_packet
{
private:
    unsigned char* packet_data;
    int packet_data_size;

    struct
    {
        unsigned char cnum;		// object class (Class-num)
        unsigned char ctnum;	// object class type (C-type)
        unsigned char scnum;	// subobject class
        unsigned char sctnum;	// subobject class type
    }
    objects[MAX_OBJECTS], objects_order[NUM_OBJECTS];


    unsigned int infinity;

    struct
    {
        unsigned char cnum;
        unsigned char ctnum;
        unsigned char scnum;
        unsigned char sctnum;
        unsigned char* req;		// necessary configuration fields table
        int req_count;
        int and_or;  // 0 = AND (all req needed in conf), 1 = OR (minimum one req needed in conf)
    }
    requirements[MAX_REQS];

    void add_req(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum, char* obj_desc, unsigned char prop_num, int and_or=0);
    void add_order(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum, char* obj_desc);
    int check_addable(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum);
    int get_object_order(unsigned char cnum, unsigned char ctnum, unsigned char scnum, unsigned char sctnum);

    int add_int(unsigned int value);
    int add_addr(unsigned int value);
    int add_short(unsigned short value);
    int add_char(unsigned char value);
    int add_chars(char* value, int count = -1);
    int add_float(float value);
    int add_void(void* value, int size);

    int set_short(unsigned short value, int offset);

    unsigned int get_int(int* offset);
    unsigned int get_addr(int* offset);
    unsigned short get_short(int* offset);
    unsigned char get_char(int* offset);
    char* get_chars(int* offset, int count);
    float get_float(int* offset);
    void* get_void(int* offset, int size);

    int parse_1_7_0_0(int offset);
    int parse_1_11_0_0(int offset);
    int parse_3_1_0_0(int offset);
    int parse_3_3_0_0(int offset, int data_size);
    int parse_5_1_0_0(int offset);
    int parse_6_1_0_0(int offset);
    int parse_7_1_0_0(int offset, int data_size);
    int parse_8_1_0_0(int offset);
    int parse_9_2_0_0(int offset);
    int parse_9_4_0_0(int offset);
    int parse_9_6_0_0(int offset);
    int parse_10_7_0_0(int offset);
    int parse_11_7_0_0(int offset);
    int parse_12_2_0_0(int offset);
    int parse_12_4_0_0(int offset);
    int parse_12_6_0_0(int offset);
    int parse_15_1_0_0(int offset);
    int parse_16_2_0_0(int offset, int data_size);
    int parse_19_4_0_0(int offset);
    int parse_20_1_0_0(int offset, int data_size);
    int parse_22_1_0_0(int offset);
    int parse_22_2_0_0(int offset);
    int parse_23_1_0_0(int offset);
    int parse_24_1_0_0(int offset);
    int parse_24_2_0_0(int offset);
    int parse_25_1_0_0(int offset, int data_size);
    int parse_35_2_0_0(int offset, int data_size);
    int parse_131_1_0_0(int offset);
    int parse_195_1_0_0(int offset);
    int parse_196_1_0_0(int offset);
    int parse_207_7_0_0(int offset);
    int parse_229_1_0_0(int offset, int data_size);
    int parse_229_1_1_1(int offset);
    int parse_229_1_2_1(int offset);
    int parse_229_1_5_1(int offset);
    int parse_230_0_0_0(int offset);
    int parse_230_1_0_0(int offset, int data_size);
    int parse_230_2_0_0(int offset, int data_size);
    int add_1_7_0_0();
    int add_1_11_0_0();
    int add_3_1_0_0();
    int add_3_3_0_0();
    int add_5_1_0_0();
    int add_6_1_0_0();
    int add_7_1_0_0();
    int add_8_1_0_0();
    int add_9_2_0_0();
    int add_9_4_0_0();
    int add_9_6_0_0();
    int add_10_7_0_0();
    int add_11_7_0_0();
    int add_12_2_0_0();
    int add_12_4_0_0();
    int add_12_6_0_0();
    int add_15_1_0_0();
    int add_16_2_0_0();
    int add_19_4_0_0();
    int add_20_1_0_0();
    int add_22_1_0_0();
    int add_22_2_0_0();
    int add_23_1_0_0();
    int add_24_1_0_0();
    int add_24_2_0_0();
    int add_35_2_0_0();
    int add_131_1_0_0();
    int add_195_1_0_0();
    int add_196_1_0_0();
    int add_207_7_0_0();
    int add_229_1_0_0();
    int add_229_1_1_1();
    int add_229_1_2_1();
    int add_229_1_5_1();
    int add_230_0_0_0();
    int add_230_1_0_0();
    int add_230_2_0_0();
    int add_obj_header(unsigned short data_size, unsigned char cnum, unsigned char ctnum);

public:
    rsvp_packet();
    ~rsvp_packet();
    
    rsvp_configuration* configuration;

    rsvp_configuration* parse_data(void* buffer, int size);

	/* build rsvp objects from packet configuration
	@return 		common rsvp objects length or error code */
    int get_data(void* buffer, int size);
    int set_configuration(rsvp_configuration* conf);
    int add_object(int cnum, int ctnum, int scnum, int sctnum, char* obj_description);

	/* build rsvp message (common header)
	@param obj_buf		input/output parameter for rsvp objects/message buffer (tricky)
	@return 			common rsvp message length or error code */
	int make_rsvp_packet(void** obj_buf, int size, unsigned char msg_type);

	/* parse ip header and rsvp header(common header), checksum checking
	@param buffer		input/output parameter for packet/rsvp_message buffer (tricky)
	@return 			common rsvp message length or error code */
    int parse_headers(void** buffer, int* size, struct IPHeader* hdr = NULL);
};

#endif
