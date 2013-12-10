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
 
#ifndef MGMT_INTERFACE_HEADER_FILE
#define MGMT_INTERFACE_HEADER_FILE

class mgmt_interface;

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include "generic_interface.h"
#include "rsvp_configuration.h"
#include "rsvp_dispatcher.h"
#include "rsvp_connection.h"
#include "mgmt_packet.h"
#include "tools.h"
#include "configuration.h"

#define MAX_MGMT_PATHS  100
#define MAX_PENDING_RSVP_PACKETS 100
#define MAX_PENDING_MGMT_PACKETS 100
#define READ_BUF_SIZE 8192
#define MGMT_LOCAL_ADDRESS   "0.0.0.0"
//#define MGMT_LISTEN_PORT    35000

#define STATE_LISTEN    1
#define STATE_CONNECT   2
#define STATE_CONNECTED 3

/* for comming rsvp packet */
struct pending_rsvp_packet
{
    int                 cid;    /* connection id = tunnel_id */
    int                 mtype;  /* message type     */
    int                 mmid;   /* mgmt message id  */
    bool                waiting_confirmation; /* PATH message waiting for client action */
    rsvp_configuration  conf;
};

/* for comming mgnt packet */
struct pending_mgnt_packet
{
    void*       packet;
    int         length;
};

/* particular path desctription */
struct path_elem
{
    unsigned int        tunnel_id;
    unsigned int        src_addr;
    unsigned int        dest_addr;
    rsvp_configuration  conf;
    int                 conn_id;
};

class mgmt_interface:public generic_interface
{
    private:
        unsigned short      local_next_m_id;
	unsigned short      remote_next_m_id;
        unsigned int        m_console_addr;
        rsvp_dispatcher*    dispatcher;
        int                 socket_descriptor_listen;
        int                 socket_descriptor;
        int                 state;
        pthread_t           listener_tid;
        pthread_t           socket_listener_tid;
        struct path_elem   paths[MAX_MGMT_PATHS];
        int                 paths_count;
        struct pending_rsvp_packet pending_rsvp_packets[MAX_PENDING_RSVP_PACKETS];
        struct pending_mgnt_packet pending_mgmt_packets[MAX_PENDING_MGMT_PACKETS];
	unsigned int mngt_server_address; /* mngt server address if agent is a nmgt client*/

        static void listener(mgmt_interface* i);        // main thread
        static void socket_listener(mgmt_interface* i); // listening to socket and awakening listener(...)
                                                        // through touch()
        int send_create_req(rsvp_configuration conf, int mid);
        int send_create_ack(rsvp_configuration conf, int nack=0);
        int send_delete_req(rsvp_configuration conf);
        int send_delete_ack(rsvp_configuration conf, int nack=0);
        int send_status_ack(rsvp_configuration conf, int num, int total);
	int send_status_nack();
        int send_transit_req();
        void init();
        void check_pending_rsvp_packet(int packet_index);
        void check_pending_mgnt_packet(int packet_index);
        void read_mgnt_packets();
        void accepting_connection();
	void connect_to_mngt_server();
        pending_rsvp_packet* find_pending_rsvp_packet(rsvp_configuration*  pack_conf, int m_id);
        int send_mgnt_packet(rsvp_configuration* conf, int mid, int type, int cmd,
                            int multi_num = -1, int multi_count = -1);
        bool add_path(pending_rsvp_packet* pend_pack);
        void delete_path(int number);
        int find_path(int tunnel_id, int src_addr, int dest_addr); /* return -1 if no found */

    public:
        mgmt_interface(rsvp_dispatcher* disp);                       // for listening
        mgmt_interface(rsvp_dispatcher* disp, unsigned int address); // for establishing connection with
                                                                     // particular management console
        ~mgmt_interface();
        int ask_gui(int conn_id, rsvp_configuration config, int msg_type);
};

#endif
