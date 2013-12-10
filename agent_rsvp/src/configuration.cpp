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

#ifndef CONFIGURATION_BODY_FILE
#define CONFIGURATION_BODY_FILE

#include "GetPot"
#include "configuration.h"
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

//============================================================================

rsvp_globals::rsvp_globals()
{
    init();
}

//----------------------------------------------------------------------------

void rsvp_globals::init()
{
    protocol_vers           = -1;
    rsvp_local_if_addr      = "";
    rsvp_local_if_name      = "";
    rsvp_router_addr        = "";
    report_paths_interval   = -1;
    debug_send_addr	        = "";
    default_path_dest       = "";
    path_setup_timeout      = -1;
    management_port         = 0;
    
    srand(time(NULL));
    hello_instance  = rand();
    epoch           = rand();
    message_id      = 1;
    label_id        = 1;
    program_running = 1;
    translator	    = false;
    uni_proxy_server = true;
    message_id_sem.set_value(1);
}

//----------------------------------------------------------------------------

bool rsvp_globals::load_configuration()
{
    string  filename = "conf.txt";
    GetPot   c(filename.c_str());

    string active = string(c("active_section", "")) + '/';

    std::cout<<"active section: "<<active<<"\n";
    /* setting access prefix to an active section */
    c.set_prefix(active.c_str());

    /* reading configuration parameters to program variables */
    string _protocol_vers   = string(c("protocol_vers", ""));
    rsvp_local_if_addr      = c("rsvp_local_if_addr", "");
    rsvp_local_if_name      = c("rsvp_local_if_name", "");
    rsvp_router_addr        = c("rsvp_router_addr", "");
    report_paths_interval   = c("report_paths_interval", -1);
    debug_send_addr         = c("debug_send_addr", "");
    default_path_dest       = c("default_path_dest", "");
    path_setup_timeout      = c("path_setup_timeout", -1);
    management_port         = c("management_port", 0);

    /* if cannot read some configuration in the section then search
    that configuration outside the section */
    c.set_prefix("");

    if(_protocol_vers == "")
        _protocol_vers          = string(c("protocol_vers", ""));
        
    if(strlen(rsvp_local_if_addr) == 0)
        rsvp_local_if_addr      = c("rsvp_local_if_addr", "");
        
    if(strlen(rsvp_local_if_name) == 0)
        rsvp_local_if_name      = c("rsvp_local_if_name", "");
        
    if(strlen(rsvp_router_addr) == 0)
        rsvp_router_addr        = c("rsvp_router_addr", "");
	
    if(strlen(debug_send_addr) == 0)
        debug_send_addr         = c("debug_send_addr", "");
    
    if(strlen(default_path_dest) == 0)
        default_path_dest       = c("default_path_dest", "");
        
    if(report_paths_interval == -1)
        report_paths_interval   = c("report_paths_interval", -1);	
    
    if(path_setup_timeout == -1)
        path_setup_timeout      = c("path_setup_timeout", -1);
        
    if(management_port == 0)
        management_port      = c("management_port", 35000); /* default value */

    /* checking is configuration loaded */
    if(_protocol_vers == "")
    {
        VERBOSE(2, "protocol_vers not configured\n");
        return false;
    }
    if(strlen(rsvp_local_if_addr) == 0)
    {
        VERBOSE(2, "rsvp_local_if_addr not configured\n");
        return false;
    }
    if(strlen(rsvp_local_if_name) == 0)
    {
        VERBOSE(2, "rsvp_local_if_name not configured\n");
        return false;
    }
    if(strlen(rsvp_router_addr) == 0)
    {
        VERBOSE(2, "rsvp_router_addr not configured\n");
        return false;
    }
    if(translator == true && strlen(default_path_dest) == 0)
    {
        VERBOSE(2, "default_path_dest not configured\n");
        return false;
    }
    if(report_paths_interval == -1)
    {
        VERBOSE(2, "report_paths_interval not configured\n");
        return false;
    }
    if(path_setup_timeout == -1)
    {
        VERBOSE(2, "path_setup_timeout not configured\n");
        return false;
    }

    /* changing protocol_vers string to integer */
    if (_protocol_vers == "IETF_RSVP")
        protocol_vers = IETF_RSVP;
    else if (_protocol_vers == "OIFUNI10")
        protocol_vers = OIFUNI10;
    else if (_protocol_vers == "OIFUNI20")
        protocol_vers = OIFUNI20;
    else
    {
        VERBOSE(2, "protocol_vers invalid\n");
        return false;
    }

    /* validing ip addresses */
    struct in_addr addr;
    if(inet_aton(rsvp_local_if_addr, &addr) == 0)
    {
        VERBOSE(2, "rsvp_local_if_addr invalid\n");
        return false;
    }
    if(inet_aton(rsvp_router_addr, &addr) == 0)
    {
        VERBOSE(2, "rsvp_router_addr invalid\n");
        return false;
    }
    if(translator == true && inet_aton(default_path_dest, &addr) == 0)
    {
        VERBOSE(2, "default_path_dest invalid\n");
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------

unsigned int rsvp_globals::get_message_id()
{
    message_id_sem.wait();
    unsigned int m_id = message_id;
    message_id_sem.signal_plus();
    return m_id;
}

//----------------------------------------------------------------------------

void rsvp_globals::inc_message_id()
{
    message_id_sem.wait();
    message_id++;
    message_id_sem.signal_plus();
}

//----------------------------------------------------------------------------

//============================================================================


#endif /* CONFIGURATION_BODY_FILE */
