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
 
#ifndef RSVP_ERROR_HEADER_FILE
#define RSVP_ERROR_HEADER_FILE

#include <string.h>

#define ERROR_NO_ERROR          0
#define ERROR_INC_PAR           -1
#define ERROR_MALLOC            -2
#define ERROR_STRLEN            -3
#define ERROR_VAL_NOT_PRES      -4
#define ERROR_VAL_PRES          -5
#define ERROR_NOT_IMPL          -6
#define ERROR_SOCK_CREAT        -7
#define ERROR_SOCK_NOT_FUNC     -8
#define ERROR_PACKET_SEND       -9
#define ERROR_TIMEOUT           -10
#define ERROR_SELECT            -11
#define ERROR_RECV_BUF_FULL     -12
#define ERROR_RECV_BUF_EMPTY    -13
#define ERROR_BUF_2_SMALL       -14
#define ERROR_SOCK_CLOSE        -15
#define ERROR_CONF_INSUFF       -16
#define ERROR_2_MANY_OBJECTS    -17
#define ERROR_DIFF_VOID_SIZE    -18
#define ERROR_SIZE_NOT_4        -19
#define ERROR_MAX_ELEMENTS      -20
#define ERROR_CHECKSUM          -21
#define ERROR_CANNOT_DESTROY    -22
#define ERROR_SEM_INT           -23
#define ERROR_NO_INT            -24
#define ERROR_NO_CONN           -25
#define ERROR_INT_NOT_US        -26
#define ERROR_UNDEFINED         -255

#define VAL_TRUE    1
#define VAL_FALSE   0

char* decode_error(int errnum);

#endif

