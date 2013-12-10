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
 
#ifndef TOOLS_BODY_FILE
#define TOOLS_BODY_FILE

#include "tools.h"

extern rsvp_globals globals;

char* int_2_addr(unsigned int addr)
{
    struct in_addr in;
    in.s_addr = addr;
    return strdup(inet_ntoa(in));
}

unsigned int resolve_router_addr(rsvp_configuration conf)
{
    // for now - ultimately, this function should give proper router address based on info contained in configuration
    // we assume, that the configuration belongs to a newly created path - otherwise, it doesn't make sense, does it? :)
    return inet_addr(globals.rsvp_router_addr);
}

void write_ieee_float(char* data, float value)
{
    int exp, s;
    double mant;
    unsigned int ibuf, ibuf2;

    memset(data, 0, 4);
    if(value == 0)
        return;
    if(value < 0)
    {
        value = -value;
        s = 1;
    }
    // do the thing for the weird numbers = NaNs, infinities, etc.
    if(isinf(value) != 0)
    {
        if(isinf(value) < 0)
            s = 1;
        exp = 255;
        ibuf = 0;
        ibuf |= exp << 23;
        ibuf |= s << 31;
        ibuf2 = htonl(ibuf);
        memcpy(data, &ibuf2, 4);
        return;
    }
    if(isnan(value))
    {
        exp = 255;
        ibuf = 1;
        ibuf |= exp << 23;
        ibuf2 = htonl(ibuf);
        memcpy(data, &ibuf2, 4);
        return;
    }
    frexpf(value, &exp);
    mant = value;
    while(mant >= 0x7fffffff)
        mant /= 2;
    while((mant != (int)mant) && (mant <= 0x00ffffff))
        mant *= 2;
    ibuf = (unsigned int)mant;
    while((ibuf & 0xff000000) != 0)
        ibuf = ibuf >> 1;
    while((ibuf & 0xff800000) == 0)
        ibuf = ibuf << 1;
    ibuf &= 0x007fffff;
    exp--;

    exp += 127;
    ibuf |= exp << 23;
    ibuf |= s << 31;
    ibuf2 = htonl(ibuf);
    memcpy(data, &ibuf2, 4);
}

float read_ieee_float(char* data)
{
    int s, e;
    unsigned long src;
    long f;
    float value;

    src = ((unsigned long)(data[0] & 0x000000ff) << 24) +
          ((unsigned long)(data[1] & 0x000000ff) << 16) +
          ((unsigned long)(data[2] & 0x000000ff) << 8) +
          ((unsigned long)(data[3] & 0x000000ff));

    s = (src & 0x80000000UL) >> 31;
    e = (src & 0x7F800000UL) >> 23;
    f = (src & 0x007FFFFFUL);

    if (e == 255 && f != 0)
    {
        /* NaN - Not a number */
        value = DBL_MAX;
    }
    else if (e == 255 && f == 0 && s == 1)
    {
        /* Negative infinity */
        value = -DBL_MAX;
    }
    else if (e == 255 && f == 0 && s == 0)
    {
        /* Positive infinity */
        value = DBL_MAX;
    }
    else if (e > 0 && e < 255)
    {
        /* Normal number */
        f += 0x00800000UL;
        if (s)
            f = -f;
        value = ldexp(f, e - 127 - 23);
    }
    else if (e == 0 && f != 0)
    {
        /* Denormal number */
        if (s)
            f = -f;
        value = ldexp(f, -126 - 23);
    }
    else if (e == 0 && f == 0 && s == 1)
    {
        /* Negative zero */
        value = 0;
    }
    else if (e == 0 && f == 0 && s == 0)
    {
        /* Positive zero */
        value = 0;
    }
    else
    {
        /* Never happens */
    }

    return value;
}

#endif
