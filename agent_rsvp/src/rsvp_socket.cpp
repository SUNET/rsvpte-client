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
 
#ifndef RSVP_SOCKET_BODY_FILE
#define RSVP_SOCKET_BODY_FILE

#include "rsvp_socket.h"
#include <errno.h>

extern rsvp_globals globals;

//-------------------------------------------------------------------

rsvp_socket::rsvp_socket()
{
    local_address = 0;
    init();
}

rsvp_socket::rsvp_socket(unsigned int local_addr)
{
    local_address = local_addr;
    init();
}

rsvp_socket::rsvp_socket(const char* local_addr)
{
    local_address = inet_addr(local_addr);
    init();
}

rsvp_socket::rsvp_socket(ip_mask ipm)
{
    local_address = 0;
    ipm_allowed_addrs = ipm;
    init();
}

rsvp_socket::rsvp_socket(unsigned int local_addr, ip_mask ipm)
{
    local_address = local_addr;
    ipm_allowed_addrs = ipm;
    init();
}

rsvp_socket::rsvp_socket(const char* local_addr, ip_mask ipm)
{
    local_address = inet_addr(local_addr);
    ipm_allowed_addrs = ipm;
    init();
}

//-------------------------------------------------------------------

rsvp_socket::~rsvp_socket()
{
    int t;
    close_connection();
    recv_buffer_count = 0;
    for(t=0;t<256;t++)
        free(recv_buffer[t]);
}

//-------------------------------------------------------------------

void rsvp_socket::init()
{
    int t;

    for(t=0;t<256;t++)
    {
        recv_buffer[t] = NULL;
        recv_buffer_size[t] = 0;
    }

    recv_buffer_count = 0;
    socket_desc = -1;
    run_socket();
}

//-------------------------------------------------------------------

int rsvp_socket::run_socket()
{
    int t,a;
    unsigned char z;
    struct sockaddr_in sin_local_addr;

    sin_local_addr.sin_family = AF_INET;
    sin_local_addr.sin_port = 0;
    sin_local_addr.sin_addr.s_addr = local_address;
    socket_desc = socket(PF_INET, SOCK_RAW, IPPROTO_RSVP);
    VERBOSE(5, "socket_desc = %d, local_address is %s\n", socket_desc, int_2_addr(local_address));
    if(socket_desc == -1)
        return ERROR_SOCK_CREAT;
    if(local_address != 0)
        bind(socket_desc, (struct sockaddr*)&sin_local_addr, sizeof(sin_local_addr));

    if (globals.protocol_vers == IETF_RSVP)
    {
       // IP Router Alert - write to packets
       t = htonl(0x94040000);
       VERBOSE(6, "IP Router Alert - write to packets\n");
       setsockopt(socket_desc, SOL_IP, IP_OPTIONS, (const void*)&t, 4);
    }

    // TOS
    z = 0xc0;
    VERBOSE(6, "TOS option\n");
    setsockopt(socket_desc, SOL_IP, IP_TOS, (const void*)&z, 1);

    // TTL
    z = 0xff;
    VERBOSE(6, "TTL option\n");
    setsockopt(socket_desc, SOL_IP, IP_TTL, (const void*)&z, 1);

    if (globals.protocol_vers == IETF_RSVP)
    {
        // IP Router Alert - receive packets
        t = 1;
        VERBOSE(6, "IP Router Alert - receive packets\n");
        setsockopt(socket_desc, SOL_IP, IP_ROUTER_ALERT, (const void*)&t, 4);
    }

    return 0;
}

//-------------------------------------------------------------------

int rsvp_socket::send(unsigned int remote_addr, void* buffer, int size)
{
    struct sockaddr_in rem_addr;
    int t;

    rem_addr.sin_family = AF_INET;
    rem_addr.sin_port = 0;
    rem_addr.sin_addr.s_addr = remote_addr;
    VERBOSE(5, "rsvp_socket::send - address is %s, size is %d\n", int_2_addr(remote_addr), size);
    if(buffer == NULL)
        {VERBOSE(5, "buffer is NULL!!!!!!!!!!!!!!!\n");}
    if(socket_desc == -1)
        return ERROR_SOCK_NOT_FUNC;

    VERBOSE(7, "sendto: socket_desc %d, size %d, sizeof(rem_addr) %d\n", socket_desc, size, sizeof(rem_addr));
    if((t = sendto(socket_desc, buffer, size, 0, (struct sockaddr*)&rem_addr, sizeof(rem_addr))) != size)
    {
            VERBOSE(7, "errno catching\n");
            switch(errno)
            {
            case EBADF:
                VERBOSE(3, "An invalid file descriptor was given in one of the sets\n");
                break;
            case EINTR:
                VERBOSE(3, "A non blocked signal was caught\n");
                break;
            case EINVAL:
                VERBOSE(3, "An socket_desc is negative or the value contained within timeout is invalid\n");
                break;
            case ENOMEM:
                VERBOSE(3, "select was unable to allocate memory for internal tables\n");
                break;
            default:
                VERBOSE(3, "value of errno is %i\n", errno);
	    } 
        return ERROR_PACKET_SEND;
    }
    VERBOSE(7, "sendto said %d\n", t);

    if(strlen(globals.debug_send_addr) != 0)
    {
        VERBOSE(5, "rsvp_socket::debug send - address is %s\n", globals.debug_send_addr);
        rem_addr.sin_addr.s_addr = inet_addr(globals.debug_send_addr);
        if(sendto(socket_desc, buffer, size, 0, (struct sockaddr*)&rem_addr, sizeof(rem_addr)) != size)
            {VERBOSE(5, "cannot send copy of the packet!!!!!!!!!!!!!\n");}
    }
    return 0;
}

//-------------------------------------------------------------------

int rsvp_socket::send(const char* remote_addr, void* buffer, int size)
{
    return send(inet_addr(remote_addr), buffer, size);
}

//-------------------------------------------------------------------

int rsvp_socket::wait_for_packet(struct timeval* tv)
{
    fd_set fds;
    int something_received, t;
    struct timeval short_tv;
    void* buffer;

    something_received = 0;
    short_tv.tv_sec = 0;
    short_tv.tv_usec = 1;
    while(1)
    {
        if(recv_buffer_count == 256)
            return ERROR_RECV_BUF_FULL;
        FD_ZERO(&fds);
        FD_SET(socket_desc, &fds);
        VERBOSE(7, "socket thread waiting for packet: %i\n", socket_desc);
        if(something_received == 0)
            t = select(socket_desc+1, &fds, NULL, NULL, tv);
        else
            t = select(socket_desc+1, &fds, NULL, NULL, &short_tv);
        VERBOSE(7, "socket thread woke up (by select): %i\n", t);
        if(t == 0 && something_received == 0)
        {
            VERBOSE(7, "socket timeout\n");
            return ERROR_TIMEOUT;
        }
        if(t == 0 && something_received != 0)
        {
            VERBOSE(7, "someting received and saved in buffor\n");
            return something_received;
        }
        if(t < 0)
        {
            VERBOSE(7, "errno catching\n");
            switch(errno)
            {
            case EBADF:
                VERBOSE(3, "An invalid file descriptor was given in one of the sets\n");
                break;
            case EINTR:
                VERBOSE(3, "A non blocked signal was caught\n");
                break;
            case EINVAL:
                VERBOSE(3, "An socket_desc is negative or the value contained within timeout is invalid\n");
                break;
            case ENOMEM:
                VERBOSE(3, "select was unable to allocate memory for internal tables\n");
                break;
            default:
                VERBOSE(3, "value of errno is %i\n", errno);

            }
            return ERROR_SELECT;
        }
        buffer = malloc(MAX_RECV_BUFFER_SIZE);
        t = read(socket_desc, buffer, MAX_RECV_BUFFER_SIZE);
        VERBOSE(7, "read byte from socket: %i\n", t);
        if(check_packet_valid(buffer) == VAL_TRUE)
        {
            VERBOSE(7, "writing incoming bytes to buffor", t);
            something_received++;
            recv_buffer[recv_buffer_count] = buffer;
            recv_buffer_size[recv_buffer_count] = t;
            recv_buffer_count++;
        }
        else
        {
            free(buffer);
        }
    }
}

//-------------------------------------------------------------------

int rsvp_socket::check_packet_valid(void* buffer)
{
    unsigned int* ibuf;
    int t, x;
    unsigned int res;
    unsigned int mask;

    ibuf = (unsigned int*)(buffer);
    // ibuf[4] -> destination address of this IP packet
    if(ipm_allowed_addrs.get_counter() == 0)
        return VAL_TRUE;  
                             // we consider IP checking OFF in this case
    for(t=0;t<ipm_allowed_addrs.get_counter();t++)
    {
        res = ipm_allowed_addrs.get_addr(t) ^ ibuf[4];      // calculate differences between these two addresses
        mask = 0;
        for(x=0;x<ipm_allowed_addrs.get_mask(t);x++)
            mask = mask + (1<<x);                       // calculate the mask
        res = res & mask;                               // check if differences are in the range of the mask

        VERBOSE(7, "res is 0x%x, mask is 0x%x\n", res, mask);
        VERBOSE(7, "ip_mask IP is 0x%x, packet IP is 0x%x\n", ipm_allowed_addrs.get_addr(t), ibuf[4]);

        if(res == 0)                                    // IP addresses are identical in the mask range
            return VAL_TRUE;
    }
    return VAL_FALSE;
}

//-------------------------------------------------------------------

int rsvp_socket::get_packet(void* buffer, unsigned int size)
{
    int t, s;
    VERBOSE(7, "socket's get_packet invoked\n");
    if(recv_buffer_count == 0)
        return ERROR_RECV_BUF_EMPTY;
    if(size < recv_buffer_size[0])
        return ERROR_BUF_2_SMALL;
    memcpy(buffer, recv_buffer[0], recv_buffer_size[0]);
    s = recv_buffer_size[0];
    for(t=0;t<255;t++)
    {
        recv_buffer[t] = recv_buffer[t+1]; 
        recv_buffer_size[t] = recv_buffer_size[t+1];
    }
    recv_buffer_count--;
    VERBOSE(8, "packet successfuly copied\n");
    return s;
}

//-------------------------------------------------------------------

int rsvp_socket::close_connection()
{
    if(close(socket_desc) == -1)
        return ERROR_SOCK_CLOSE;
    return 0;
}

//-------------------------------------------------------------------

unsigned int addr_to_int(const char* txt)
{
    return ntohs(inet_addr(txt));
}

//-------------------------------------------------------------------

#endif

