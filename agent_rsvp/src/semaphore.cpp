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
 
#ifndef SEMAPHORE_BODY_FILE
#define SEMAPHORE_BODY_FILE

#include "semaphore.h"

semaphore::semaphore()
{
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT);
    semctl(semid, 0, SETVAL, 0);
}

semaphore::~semaphore()
{
    semctl(semid, 0, IPC_RMID);
}

int semaphore::wait()
{
    int t;
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    t = semop(semid, &sb, 1);
    if(t == 0)
        return 0;
    else
        return ERROR_SEM_INT;
}

void semaphore::set_value(int value)
{
    semctl(semid, 0, SETVAL, value);
}

void semaphore::signal_plus()
{
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = +1;
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

void semaphore::signal_zero()
{
     semctl(semid, 0, SETVAL, 1);
}

#endif
