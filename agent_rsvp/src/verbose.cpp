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
 
#ifndef VERBOSE_BODY_FILE
#define VERBOSE_BODY_FILE

#include "verbose.h"

struct
{
    pthread_t tid;
    char name[MAX_THREAD_NAME_LENGTH];
} _thread_names[MAX_THREAD_NAMES_COUNT];

int _thread_count = 0;
int _thread_name_max_length = UNKNOWN_THREAD_STRING_LENGTH;

char* thread_name()
{
    int t;
    pthread_t my_tid;
    char th_name[MAX_THREAD_NAME_LENGTH];

    for(t=0;t<MAX_THREAD_NAME_LENGTH-1;t++)
        th_name[t] = ' ';
    th_name[_thread_name_max_length] = 0;
    my_tid = pthread_self();
    for(t=0;t<_thread_count;t++)
        if(_thread_names[t].tid == my_tid)
        {
            strncpy(th_name, _thread_names[t].name, strlen(_thread_names[t].name));
            return strdup(th_name);
        }
    strncpy(th_name, UNKNOWN_THREAD_STRING, UNKNOWN_THREAD_STRING_LENGTH);
    return strdup(th_name);
}

void name_thread(const char* new_name, pthread_t new_tid)
{
    if(_thread_count == MAX_THREAD_NAMES_COUNT)
        return;
    _thread_names[_thread_count].tid = new_tid;
    strncpy(_thread_names[_thread_count].name, new_name, MAX_THREAD_NAME_LENGTH);
    _thread_names[_thread_count].name[MAX_THREAD_NAME_LENGTH-1] = 0;
    if(strlen(_thread_names[_thread_count].name) > _thread_name_max_length)
        _thread_name_max_length = strlen(_thread_names[_thread_count].name);
    _thread_count++;
}

#endif
