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
 
#ifndef IP_MASK_HEADER_FILE
#define IP_MASK_HEADER_FILE

class ip_mask;

#include "verbose.h"
#include "stdlib.h"

class ip_mask
{
private:
    unsigned int* addr;			/* ip address table */
    unsigned char* mask;		/* ip mask table for addr table */
    unsigned int counter;		/* addr and mask pairs count */
public:
    ip_mask();
    ip_mask(const ip_mask& old);
    ~ip_mask();
    ip_mask& operator=(const ip_mask& old);

    void add(unsigned int ip_addr, unsigned char ip_mask);
    unsigned int get_counter();
    unsigned int get_addr(unsigned int number);
    unsigned char get_mask(unsigned int number);
};

#endif
