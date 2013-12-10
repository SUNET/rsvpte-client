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
 
#ifndef GENERIC_INTERFACE_BODY_FILE
#define GENERIC_INTERFACE_BODY_FILE

#include "generic_interface.h"

int generic_interface::wait_for_touch()
{
    if(_semid < 0)
        return ERROR_SEM_INT;
        
    int t, x;
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    VERBOSE(7, "waiting for a gentle touch\n");
    t = semop(_semid, &sb, 1);
    x = errno;
    if(t < 0 && x != 22)
    {
        VERBOSE(0, "error: errno is %d\n", x);
    }
    VERBOSE(7, "touched... awakening!\n");
    if(t == 0)
        return 0;
    else
        return ERROR_SEM_INT;
}

generic_interface::generic_interface()
{
    _semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    VERBOSE(4, "_semid id %d\n", _semid);
    if(_semid < 0)
    {
        VERBOSE(0, "!!!! Semaphore not allocated !!!!\n");
        VERBOSE(0, "!!!! PLEASE FREE semaphores MANUALLY !!!!\n");
        return;
    }
    semctl(_semid, 0, SETVAL, 0);
}

generic_interface::~generic_interface()
{
    int ret = semctl(_semid, 0, IPC_RMID);
    VERBOSE(0, "Removing semaphore: %i\n", ret);
    _semid = -1;
}

void generic_interface::touch()
{
    if(_semid < 0)
        return;
        
    struct sembuf sb;
    /* incrementing value */
    sb.sem_num = 0;
    sb.sem_op = +1;
    sb.sem_flg = 0;
    semop(_semid, &sb, 1);

// setting value
//     semctl(_semid, 0, SETVAL, 1);

    VERBOSE(7, "i have touched and I'm free to go on now.\n");
}

#endif
