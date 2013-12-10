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
 
#ifndef MPLS_IOCTL_HEADER_INCLUDED
#define MPLS_IOCTL_HEADER_INCLUDED

class mpls_ioctl;

/* if you have no mpls-linux installed and you wish to complile this code
   then uncomment macro or add gcc compiler flag: -D__LINUX_MPLS_ABSENCE__ */
#define __LINUX_MPLS_ABSENCE__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "verbose.h"

#ifndef __LINUX_MPLS_ABSENCE__
#include <linux/mpls.h>
#endif /* __LINUX_MPLS_ABSENCE__ */

class mpls_ioctl
{
private:
    int make_ioctl(int req, void* ptr);
    unsigned int get_if_index(const char* name);
    struct sockaddr get_sockaddr_addr(unsigned int addr);
public:
    int set_in_label(unsigned int label);
    int set_out_label(unsigned int label, const char* if_name, unsigned int addr);
    int delete_in_label(unsigned int label);
    int delete_out_label(unsigned int key);
    int set_labelspace(int labelspace, const char* if_name);
};

#endif
