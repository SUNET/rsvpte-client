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
 
#ifndef RSVP_SOCKET_HEADER_FILE
#define RSVP_SOCKET_HEADER_FILE

class rsvp_socket;

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "verbose.h"
#include "rsvp_error.h"
#include "configuration.h"
#include "tools.h"
#include "ip_mask.h"

#define MAX_RECV_BUFFER_SIZE 2500

unsigned int addr_to_int(const char* txt);

class rsvp_socket
{
private:
    unsigned int local_address;
    void* recv_buffer[256];		// receive buffers
    int recv_buffer_size[256];		// receive buffers size
    unsigned int recv_buffer_count; 
    int socket_desc;			// socket descriptor
    ip_mask ipm_allowed_addrs;		// allowed dest ip addresses for incoming packets

    int run_socket();
    void init();

public:
    rsvp_socket();
    rsvp_socket(unsigned int local_addr);
    rsvp_socket(const char* local_addr);
    rsvp_socket(ip_mask ipm);
    rsvp_socket(unsigned int local_addr, ip_mask ipm);
    rsvp_socket(const char* local_addr, ip_mask ipm);
    ~rsvp_socket();

    int send(unsigned int remote_addr, void* buffer, int size);
    int send(const char* remote_addr, void* buffer, int size);

	/* bufforing of incoming packets
	@param tv timeout period
	@return   buffored packet count(>0) or error code(<0) */
    int wait_for_packet(struct timeval* tv);

	/* get one buffored, incoming packet
	@param buffer 	buffor for packet
	@param size 	buffor size
	@return 		packet size */
    int get_packet(void* buffer, unsigned int size);

    int close_connection();

	/* packet destination ip address validation
	@param buffer  	buffor with packet
	@return 		TRUE or FALSE */
    int check_packet_valid(void* buffer);
};

#endif

