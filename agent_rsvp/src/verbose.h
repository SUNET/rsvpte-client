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
 
#ifndef VERBOSE_HEADER_FILE
#define VERBOSE_HEADER_FILE

#include <stdio.h>
#include <pthread.h>
#include <string.h>

// verbosity level (only messages with level set to value lower or equal to VERBOSE_LEVEL will be printed)
//#define VERBOSE_LEVEL                   5
#define VERBOSE_LEVEL                   15

// non-zero if we want to see info about the file, line and function, in which the call to VERBOSE took place
#define VERBOSE_LINES                   0

// non-zero if we want to see the name of a thread in mulit-threaded application
#define VERBOSE_THREAD_NAME             1

// non-zero if we want to see the level in a message (level number will be printed)
#define VERBOSE_LEVEL_PRINT             1

// maximum number of thread names, that can be remembered
#define MAX_THREAD_NAMES_COUNT          20

// maximum length of thread name - longer names will be truncated to this length
#define MAX_THREAD_NAME_LENGTH          50

// string for an unknown thread (if tid wasn't found in thread names table)
#define UNKNOWN_THREAD_STRING           "unknown thread"

// length of the unknown thread string - must be accurate number of characters in UNKNOWN_THREAD_STRING string
#define UNKNOWN_THREAD_STRING_LENGTH    14

// function used to name a thread
void name_thread(const char* new_name, pthread_t new_tid);

// function that resolves name of the thread - used by macro, no need to invoke directly
// returned string is padded with spaces to the length of the longest name known
char* thread_name();

// macro used for verbosity level filtering of printed message
#define VERBOSE(lev, f, a...) \
{ \
    if(lev <= VERBOSE_LEVEL) \
    { \
        if(VERBOSE_LINES) \
            printf("%s:%s - %d:\n\t", __FILE__, __FUNCTION__, __LINE__); \
        if(VERBOSE_THREAD_NAME) \
        { \
            char* buf; \
            buf = thread_name(); \
            printf("<%s> ", buf); \
            free(buf); \
        } \
        if(VERBOSE_LEVEL_PRINT) \
            printf("%d: ", lev); \
        printf(f, ##a); \
    } \
}

#endif
