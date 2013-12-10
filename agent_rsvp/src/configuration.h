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
 
#ifndef CONFIGURATION_HEADER_FILE
#define CONFIGURATION_HEADER_FILE

#include "semaphore.h"

//==============================================================================

#define IETF_RSVP   0
#define OIFUNI10    1
#define OIFUNI20    2

//==============================================================================

class rsvp_globals
{
public:
    /* pseudo constant atrributes */
    int protocol_vers;                  /* a protocol implementaction version:
                                           __IETF_RSVP__, __OIFUNI10__, __OIFUNI20__ */
    const char* rsvp_local_if_addr;     /* an eth interface used by the agent_rsvp */
    const char* rsvp_local_if_name;     /* an interface name used by the agent_rsvp */
    const char* rsvp_router_addr;       /* a cooperative gmpls router (ASON: UNI-N) */
    int         report_paths_interval;  /* a reporting interval of rsvp agent state to screen */
    const char* debug_send_addr;	      /* an ip address of node to which RSVP message copies will be sended */
    const char* default_path_dest;	      /* a destination address of outgoing path - used in the GMPLS/ASON translator */
    int         path_setup_timeout;     /* how long agent is waiting for RSVP RESV (seconds) */ 

    /* pseudo variable atrributes */
    unsigned int label_id;              /* TODO: check for what is it ???? */
    unsigned int epoch;                 /* a constant between restarts */
    unsigned int hello_instance;        /* a constant between restarts */
    unsigned int message_id;            /* incrementing with sended message */
    semaphore    message_id_sem;
    char         program_running;       /* till true the rsvp agent is working */
    unsigned int uni_proxy_server;      /* is this instance of agent a server of mngt comunication */
    unsigned int translator;            /* is this IETF/OIF RSVP translator */
    unsigned short management_port;     /* TCP port for API on which the agent is listining incoming client's connection */ 

    rsvp_globals();

    void         init();
    bool         load_configuration();  /* load configuration from file */
    unsigned int get_message_id();      /* thread save getter */
    void         inc_message_id();
};

//==============================================================================

#endif /* CONFIGURATION_HEADER_FILE */

