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
 
 
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include "configuration.h"
#include "rsvp_configuration.h"
#include "rsvp_dispatcher.h"
#include "rsvp_connection.h"
#include "mpls_ioctl.h"
#include "verbose.h"
#include "mgmt_interface.h"
#include "semaphore.h"
#include "ip_mask.h"

//===============================================================================
/* globals for all files of the rsvp agent */
rsvp_globals        globals;
rsvp_configuration  global_newconn_configuration;

//===============================================================================

rsvp_connection* path1          (rsvp_dispatcher* disp, unsigned short cid);
rsvp_connection* path2          (rsvp_dispatcher* disp, unsigned short cid);
rsvp_connection* path_oif       (rsvp_dispatcher* disp, unsigned short cid);
void             signal_handler (int sig);

//===============================================================================
 
int main(int argc, char *argv[])
{
    rsvp_dispatcher *dispatcher;
    mgmt_interface  *mgmt;
    rsvp_connection *conn1;
    rsvp_connection *conn2;
    rsvp_connection *conn_oif;
    unsigned short  cid1, cid2, cid_oif;
    unsigned short  global_connection_id;
    ip_mask         ipm;
    mpls_ioctl      mpls;
    int             t;

    if(globals.load_configuration() == false)
    {
        VERBOSE(1, "configuration loading failure\n");
        return 0;
    }

    name_thread("main thread", pthread_self());
    VERBOSE(1, "VERBOSE_LEVEL is %d\n", VERBOSE_LEVEL);
    
    /* so we can stop the program */
    signal(SIGINT, signal_handler);
    /* so we can make agent display it's paths using Ctrl-\ */
    signal(SIGQUIT, signal_handler);
    /* so we can make agent reload configuration */
    signal(SIGUSR1, signal_handler);
 
    /* checking of dest. address of RSVP packets.
    Only packets that fit into mask are accepted.
    If ipm is not given to dispatcher at all, all packets are accepted. */
    
    
    // TODO: move parameter to configuration
    // ipm.add(inet_addr("10.0.0.0"), 8);
    
    if(globals.protocol_vers == IETF_RSVP) {
        VERBOSE(1, "IETF implementation\n");
    }
    else if(globals.protocol_vers == OIFUNI10) {
       VERBOSE(1, "OIF implementation, UNI1.0R2\n");
    }
    else if(globals.protocol_vers == OIFUNI20) {
       VERBOSE(1, "OIF implementation, UNI2.0\n");
    }

    if(ipm.get_counter() == 0) {
        VERBOSE(2, "Accepting all addresses\n");
    }
    else
    {
        //VERBOSE(2, "Accepting:\n");
        //for(t=0;t<ipm.get_counter();t++)
        //    VERBOSE(2, " %s/%d\n", int_2_addr(ipm.get_addr(t)), ipm.get_mask(t));
    }
    VERBOSE(1, "\n");
    
    dispatcher = new rsvp_dispatcher(globals.rsvp_local_if_addr, ipm);

    /* label space for eth interface */
    mpls.set_labelspace(0, globals.rsvp_local_if_name);
    
    if(globals.uni_proxy_server == true || globals.translator == false)
    	mgmt = new mgmt_interface(dispatcher);
    else
    	mgmt = new mgmt_interface(dispatcher, inet_addr("127.0.0.1"));  // used for UNI proxy - second side
   
    while(1)
    {
        sleep(globals.report_paths_interval);
        dispatcher->report_paths();     /* info to screen */
        if(globals.program_running != 1)
            break;
    }
    delete(dispatcher);
    delete(mgmt);
    VERBOSE(1, "Finish\n");
    return 0;
}

void signal_handler(int sig)
{
    if(sig == SIGINT)
    {
        globals.program_running = 0;
    }
    if(sig == SIGUSR1)
    {
        VERBOSE(0, "configuration loaded\n");
        if(globals.load_configuration() == false)
        {
            VERBOSE(1, "configuration loading failure\n");
        }
    }
}

