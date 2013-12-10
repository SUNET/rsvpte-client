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
 
#ifndef IP_MASK_BODY_FILE
#define IP_MASK_BODY_FILE

#include "ip_mask.h"

ip_mask::ip_mask()
{
    VERBOSE(9, "ip_mask standard constructor\n");
    counter = 0;
    addr = NULL;
    mask = NULL;
}

ip_mask::ip_mask(const ip_mask& old)
{
    VERBOSE(9, "ip_mask copy constructor\n");
    counter = old.counter;
    addr = (unsigned int*)malloc(sizeof(unsigned int) * counter);
    mask = (unsigned char*)malloc(sizeof(unsigned char) * counter);
    memcpy(addr, old.addr, sizeof(unsigned int) * counter);
    memcpy(mask, old.mask, sizeof(unsigned char) * counter);
}

ip_mask::~ip_mask()
{
    VERBOSE(9, "ip_mask standard destructor\n");
    free(addr);
    free(mask);
    addr = NULL;
    mask = NULL;
    counter = 0;
}

ip_mask& ip_mask::operator=(const ip_mask& old)
{
    VERBOSE(9, "ip_mask operator=\n");
    counter = old.counter;
    addr = (unsigned int*)malloc(sizeof(unsigned int) * counter);
    mask = (unsigned char*)malloc(sizeof(unsigned char) * counter);
    memcpy(addr, old.addr, sizeof(unsigned int) * counter);
    memcpy(mask, old.mask, sizeof(unsigned char) * counter);
    return *this;
}

void ip_mask::add(unsigned int ip_addr, unsigned char ip_mask)
{
    VERBOSE(9, "ip_mask add method\n");
    counter++;
    addr = (unsigned int*)realloc(addr, sizeof(unsigned int) * counter);
    mask = (unsigned char*)realloc(mask, sizeof(unsigned char) * counter);
    addr[counter - 1] = ip_addr;
    mask[counter - 1] = ip_mask;
}

unsigned int ip_mask::get_counter()
{
    VERBOSE(9, "ip_mask get_counter\n");
    return counter;
}

unsigned int ip_mask::get_addr(unsigned int number)
{
    VERBOSE(9, "ip_mask get_addr\n");
    if(number >= counter || number < 0)
        return 0xffffffff;
    return addr[number];
}

unsigned char ip_mask::get_mask(unsigned int number)
{
    VERBOSE(9, "ip_mask get_mask\n");
    if(number >= counter || number < 0)
        return 0xff;
    return mask[number];
}


#endif
