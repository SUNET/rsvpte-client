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
 
#ifndef RSVP_CONNECTION_HEADER_FILE
#define RSVP_CONNECTION_HEADER_FILE

class rsvp_connection;

#include <pthread.h>
#include "rsvp_packet.h"
#include "rsvp_error.h"
#include "rsvp_dispatcher.h"
#include "rsvp_configuration.h"
#include "mpls_ioctl.h"
#include "verbose.h"
#include "configuration.h"
#include "tools.h"
#include "semaphore.h"
#include "ip_route.h"

#define PATH_STATE_NULL             0
#define PATH_STATE_INITIATED        1
#define PATH_STATE_PATH_SENT        2
#define PATH_STATE_PATH_RECV        3
#define PATH_STATE_RESV_RECV        4
#define PATH_STATE_RESV_SENT        5
#define PATH_STATE_UP               6
#define PATH_STATE_RESVTEAR_RECV    7
#define PATH_STATE_RESVTEAR_SENT    8
#define PATH_STATE_PATHTEAR_SENT    9
#define PATH_STATE_PATHTEAR_RECV    10
#define PATH_STATE_DOWN             11

#define DIRECTION_UNDEFINED         0
#define DIRECTION_INCOMING          1
#define DIRECTION_OUTGOING          2

#define MTYPE_PATH                  1
#define MTYPE_RESV                  2
#define MTYPE_PATHERR               3
#define MTYPE_RESVERR               4
#define MTYPE_PATHTEAR              5
#define MTYPE_RESVTEAR              6
#define MTYPE_RESVCONF              7
#define MTYPE_BUNDLE                12
#define MTYPE_ACK                   13
#define MTYPE_SREFRESH              15
#define MTYPE_HELLO                 20

class rsvp_connection
{
private:
    /* data for path-refreshing thread only */
    rsvp_packet*        path;
    char**              path_data;
    int                 path_data_size;
    int                 refresh_interval;
    pthread_t           tid;
    // ------------------------------------

    unsigned int        remote_addr;
    unsigned int        local_addr;
    unsigned int        router_address;  // address of immediate router which accepts RSVP messages
    unsigned int        dest_address;    // address of destination node of the traffic
    
    semaphore           access;

    rsvp_configuration  configuration;
    rsvp_configuration  conf_defaults;

    unsigned int        label;
    int                 key;

    void set_conf_defaults      ();
    int  check_if_packet_valid  (int msg_type);
    void respond_path           (rsvp_configuration* packet_conf);
    void respond_path_nack      (rsvp_configuration* packet_conf);
    void respond_resv           (rsvp_configuration* packet_conf);
    void respond_patherr        (rsvp_configuration* packet_conf);
    void respond_resverr        (rsvp_configuration* packet_conf);
    void respond_pathtear       (rsvp_configuration* packet_conf);
    void respond_resvtear       (rsvp_configuration* packet_conf);
    void respond_resvconf       (rsvp_configuration* packet_conf);
    static void* refresh_path   (rsvp_connection* o); // in order to send path message every x seconds - function called as a thread
    void respond_ackreq         (rsvp_configuration* packet_conf);
    void init                   (rsvp_dispatcher* disp, rsvp_configuration conf);
    void create_refresh_thread  ();
    void adding_resv_objects(rsvp_packet* resv);

    void PathErr_RD_flags_response      (rsvp_configuration* packet_conf);
    void PathTear_RD_flags_response      (rsvp_configuration* packet_conf);

    void send_rsvp_message      (rsvp_packet* packet, int mtype, int dest_addr);
    void adding_create_path_objects(rsvp_packet* mes, rsvp_configuration* conf);

public:
    int                         path_state;
    int                         refresh_tread_state;
    unsigned short              connection_id;
    rsvp_dispatcher*            dispatcher;
    rsvp_configuration          path_configuration;   /* the configuration of the path */
    int                         path_direction;
    
    rsvp_connection             (rsvp_dispatcher* disp, unsigned short conn_id, rsvp_configuration conf);
    rsvp_connection             (rsvp_dispatcher* disp, rsvp_configuration conf);
    ~rsvp_connection            ();

    void  get_packet            (int msg_type, void* buf, int buf_len, rsvp_configuration* conf = NULL, int nack = 0);
    int   create_path           (unsigned int router_addr);
    int   create_path           (const char* router_addr);
    int   destroy_path          ();
    char* get_path_state        (char* ptr);
    int   get_path_state        ();
    int   get_label             ();
};

#endif
