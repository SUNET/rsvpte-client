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
 
#ifndef RSVP_DISPATCHER_HEADER_FILE
#define RSVP_DISPATCHER_HEADER_FILE

class rsvp_dispatcher;

#include <pthread.h>
#include "rsvp_socket.h"
#include "rsvp_connection.h"
#include "rsvp_packet.h"
#include "rsvp_configuration.h"
#include "rsvp_error.h"
#include "generic_interface.h"
#include "tools.h"
#include "ip_mask.h"
#include "semaphore.h"
#include "configuration.h"

#define MAX_ROUTER_ADDRS            10
#define MAX_REGISTERED_CONNS        10
#define MAX_REGISTERED_INTERFACES   10

struct reg_conn
{
    rsvp_connection* conn;
    unsigned short id;
    int wake_interval;
    int wake_counter_sec;
    int wake_counter_usec;
};

struct reg_int
{
    generic_interface* interface;
};

class rsvp_dispatcher
{
private:
    ip_mask ipm_allowed_addrs;  // allowed dest ip addresses for incoming packets
    pthread_t tid;
    rsvp_socket* socket;
    int hello_interval;
    unsigned int router_addrs[MAX_ROUTER_ADDRS];
    int router_addrs_count;
    
    semaphore registered_conns_access;
    struct reg_int registered_interfaces[MAX_REGISTERED_INTERFACES];
    int registered_interfaces_count;

    static void* receiver_thread(void* arg);
    void init(unsigned int local_addr);
    int send_hellos();

public:
    struct reg_conn registered_conns[MAX_REGISTERED_CONNS];
    int registered_conns_count;

    rsvp_dispatcher(const char* local_addr);
    rsvp_dispatcher(unsigned int local_addr);
    rsvp_dispatcher(const char* local_addr, ip_mask ipm);
    rsvp_dispatcher(unsigned int local_addr, ip_mask ipm);
    ~rsvp_dispatcher();
    int add_router(unsigned int addr); // for hello packets
    int send(void* buf, int buf_size, unsigned int addr);
    int register_connection(rsvp_connection* conn, unsigned short conn_id, int wk_int = 30);
    int unregister_connection(int pos);
    int register_interface(generic_interface* interf);
    int unregister_interface(generic_interface* interf);
    void report_paths();
    int ask_interface(unsigned short cid, rsvp_configuration conf, int mtype);
	// deliver message configuration to connection object as like as that message was receiving by rsvp_listener
    int deliver(unsigned short cid, rsvp_configuration conf, int mtype, int nack = 0);
    rsvp_connection* get_connection(unsigned short cid);
    int get_connection_state(unsigned short cid);
    rsvp_configuration* get_connection_configuration(unsigned short cid);
};

#endif
