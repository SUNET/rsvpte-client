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
 
#ifndef GENERIC_INTERFACE_HEADER_FILE
#define GENERIC_INTERFACE_HEADER_FILE

class generic_interface;

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "verbose.h"
#include "rsvp_error.h"
#include "rsvp_configuration.h"
#include <errno.h>

class generic_interface
{
    private:
        int _semid;
    protected:
        int wait_for_touch();
    public:
        generic_interface();
        ~generic_interface();
        void touch();
        virtual int ask_gui(int cid, rsvp_configuration conf, int mtype) = 0;
};

#endif
