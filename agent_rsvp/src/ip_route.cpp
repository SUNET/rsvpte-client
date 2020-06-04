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
 
#ifndef IP_ROUTE_BODY_FILE
#define IP_ROUTE_BODY_FILE

#include "ip_route.h"

void ip_route::add_mpls_route(unsigned int target, unsigned int via, unsigned int key)
{
#ifndef __LINUX_MPLS_ROUTING_ABSENCE__
    char key_buf[20];
    char* target_buf;
    char* via_buf;

    sprintf(key_buf, "%#x\0", key);
    target_buf  = int_2_addr(target);
    via_buf     = int_2_addr(via);
    if(fork() == 0)
    {
        VERBOSE(4, "ip_route: adding mpls route\n");
        //VERBOSE(5, "ip route add %s via %s spec_nh 0x8847 %s\n", target_buf, via_buf, key_buf);
        //execlp("ip", "ip", "route", "add", target_buf, "via", via_buf, "spec_nh", "0x8847", key_buf, NULL);
        VERBOSE(5, "ip route %s encap mpls %s via inet %s\n", target_buf, key_buf, via_buf);
        execlp("ip", "ip", "route", "add", target_buf, "encap mpls", key_buf, "via inet", via_buf, NULL);
        VERBOSE(0, "ip_route: error: %s\n", strerror(errno));
        exit(0);
    }
    wait(NULL);
    free(target_buf);
    free(via_buf);
#endif /* __LINUX_MPLS_ROUTING_ABSENCE__ */
}

void ip_route::add_ip_route(unsigned int target, const char* interface_name)
{
#ifndef __LINUX_MPLS_ROUTING_ABSENCE__
    char* target_buf;

    target_buf = int_2_addr(target);
    if(fork() == 0)
    {
        VERBOSE(4, "ip_route: adding ip route\n");
        execlp("ip", "ip", "route", "add", target_buf, "dev", interface_name, NULL);
        VERBOSE(0, "ip_route: error: %s\n", strerror(errno));
        exit(0);
    }
    wait(NULL);
    free(target_buf);
#endif /* __LINUX_MPLS_ROUTING_ABSENCE__ */
}

void ip_route::delete_route(unsigned int target)
{
#ifndef __LINUX_MPLS_ROUTING_ABSENCE__
    char* target_buf;

    target_buf = int_2_addr(target);
    if(fork() == 0)
    {
        VERBOSE(4, "ip_route: deleting route\n");
        execlp("ip", "ip", "route", "del", target_buf, NULL);
        VERBOSE(0, "ip_route: error: %s\n", strerror(errno));
        exit(0);
    }
    wait(NULL);
    free(target_buf);
#endif /* __LINUX_MPLS_ROUTING_ABSENCE__ */
}

#endif
