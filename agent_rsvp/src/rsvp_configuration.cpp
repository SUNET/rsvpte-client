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
 
#ifndef RSVP_CONFIGURATION_BODY_FILE
#define RSVP_CONFIGURATION_BODY_FILE

#include "rsvp_configuration.h"

rsvp_configuration::rsvp_configuration()
{
    for(int t=0;t<256;t++)
    {
        parameter_types[t]  = 0;
        data[t]             = NULL;
        void_size[t]        = 0;
    }
    parameter_types[RC_SESSION_DEST_ADDR]       = PT_UI;
    parameter_types[RC_TUNNEL_ID]               = PT_US;
    parameter_types[RC_EXTENDED_IPV4_ADDR]      = PT_UI;
    parameter_types[RC_CONTROL_OUT_IF_ADDR]     = PT_UI;
    parameter_types[RC_CONTROL_OUT_IF_HANDLE]   = PT_UI;
    parameter_types[RC_REFRESH_PERIOD]          = PT_UI;
    parameter_types[RC_SCOPE_ADDR]              = PT_UI | PT_TAB;
    parameter_types[RC_WILDCARD_FILTER]         = PT_UC;
    parameter_types[RC_FIXED_FILTER]            = PT_UC;
    parameter_types[RC_SHARED_EXPLICIT]         = PT_UC;
    parameter_types[RC_TOKEN_BUCKET_RATE]       = PT_F;
    parameter_types[RC_TOKEN_BUCKET_SIZE]       = PT_F;
    parameter_types[RC_PEAK_DATA_RATE]          = PT_F;
    parameter_types[RC_MINIMUM_POLICED_UNIT]    = PT_UI;
    parameter_types[RC_MAXIMUM_PACKET_SIZE]     = PT_UI;
    parameter_types[RC_RATE]                    = PT_F;
    parameter_types[RC_SLACK_TERM]              = PT_UI;
    parameter_types[RC_FILTER_SRC_ADDR]         = PT_UI;
    parameter_types[RC_FILTER_SRC_PORT]         = PT_US;
    parameter_types[RC_SENDER_SRC_ADDR]         = PT_UI;
    parameter_types[RC_SENDER_SRC_ID]           = PT_US;
    parameter_types[RC_RESV_CONFIRM_RECV_ADDR]  = PT_UI;
    parameter_types[RC_LABEL]                   = PT_VS;
    parameter_types[RC_LSP_ENC_TYPE]            = PT_UC;
    parameter_types[RC_LSP_SWITCHING_TYPE]      = PT_UC;
    parameter_types[RC_LSP_GPID]                = PT_US;
    parameter_types[RC_HELLO_SRC_INSTANCE]      = PT_UI;
    parameter_types[RC_HELLO_DST_INSTANCE]      = PT_UI;
    parameter_types[RC_HELLO_REQ_ACK]           = PT_UC;
    parameter_types[RC_RESTART_TIME]            = PT_UI;
    parameter_types[RC_RECOVERY_TIME]           = PT_UI;
    parameter_types[RC_SETUP_PRIORITY]          = PT_UC;
    parameter_types[RC_HOLDING_PRIORITY]        = PT_UC;
    parameter_types[RC_LOCAL_PROTECTION_FLAG]   = PT_UC;
    parameter_types[RC_LABEL_RECORDING_FLAG]    = PT_UC;
    parameter_types[RC_SE_STYLE_FLAG]           = PT_UC;
    parameter_types[RC_SESSION_NAME]            = PT_CS;
    parameter_types[RC_ERO]                     = PT_VS | PT_TAB;
    parameter_types[RC_RSVP_HOP_TLVS]           = PT_UI | PT_TAB;
    parameter_types[RC_ERROR_NODE_ADDR]         = PT_UI;
    parameter_types[RC_ERROR_FLAGS]             = PT_UC;
    parameter_types[RC_ERROR_CODE]              = PT_UC;
    parameter_types[RC_ERROR_VALUE]             = PT_US;
    parameter_types[RC_SSDH_SIGNAL_TYPE]        = PT_UC;
    parameter_types[RC_SSDH_RCC]                = PT_UC;
    parameter_types[RC_SSDH_NCC]                = PT_US;
    parameter_types[RC_SSDH_NVC]                = PT_US;
    parameter_types[RC_SSDH_MULTIPLIER]         = PT_US;
    parameter_types[RC_SSDH_TRANSPARENCY]       = PT_UI;
    parameter_types[RC_SSDH_PROFILE]            = PT_UI;
    parameter_types[RC_MSG_ID_FLAGS]            = PT_UC;
    parameter_types[RC_MSG_ID_EPOCH]            = PT_UI;
    parameter_types[RC_MSG_ID_ID]               = PT_UI;
    parameter_types[RC_MSG_ID_ACK_NACK]         = PT_UC;
    parameter_types[RC_UPSTREAM_LABEL]          = PT_UI;
    parameter_types[RC_ADMIN_STATUS_RTAD_FLAGS] = PT_UC;
    parameter_types[RC_TNA_SRC_ADDR]            = PT_UI;
    parameter_types[RC_TNA_DST_ADDR]            = PT_UI;
    parameter_types[RC_DATA_OUT_IF_ADDR]        = PT_UI;
    parameter_types[RC_DATA_OUT_IF_HANDLE]      = PT_UI;
    parameter_types[RC_DATA_IN_IF_ADDR]         = PT_UI;
    parameter_types[RC_DATA_IN_IF_HANDLE]       = PT_UI;
    parameter_types[RC_TR_D_ADDR]               = PT_UI;
    parameter_types[RC_TR_RD_ADDR]              = PT_UI;
    parameter_types[RC_CALL_ID]                 = PT_UI;
    parameter_types[RC_CALL_ID_ADDR_TYPE]       = PT_UC;
    parameter_types[RC_CALL_ID_SRC_TN_ADDR]     = PT_UI;
    parameter_types[RC_CALL_ID_LOCAL_ID1]       = PT_UI;
    parameter_types[RC_CALL_ID_LOCAL_ID2]       = PT_UI;
    parameter_types[RC_CALL_ID_IS]              = PT_UI;
    parameter_types[RC_CALL_ID_ITU_CC1]         = PT_UI;
    parameter_types[RC_CALL_ID_ITU_CC2]         = PT_US;
    parameter_types[RC_CALL_ID_UAPC1]           = PT_US;
    parameter_types[RC_CALL_ID_UAPC2]           = PT_UI;
    parameter_types[RC_PATH_STATUS]             = PT_UI;
    parameter_types[RC_SERVICE_LEVEL]           = PT_UC;
    parameter_types[RC_ETHERNET_CIR]            = PT_F;
    parameter_types[RC_ETHERNET_CBS]            = PT_F;
    parameter_types[RC_ETHERNET_EIR]            = PT_F;
    parameter_types[RC_ETHERNET_EBS]            = PT_F;
    parameter_types[RC_ETHERNET_SW_GRANUALITY]  = PT_US;
    parameter_types[RC_ETHERNET_INDEX]          = PT_UC;
    parameter_types[RC_ETHERNET_PROFILE]        = PT_UC;
    parameter_types[RC_NOTIFY_ADDRESS]          = PT_UI;
}

rsvp_configuration::rsvp_configuration(const rsvp_configuration& old)
{
    for(int t=0;t<256;t++)
    {
        parameter_types[t] = old.parameter_types[t];
        void_size[t] = old.void_size[t];
        if(old.data[t] == NULL)
        {
            data[t] = NULL;
            continue;
        }
        if((parameter_types[t] & PT_TAB) == 0)
        {
            if((parameter_types[t] & PT_UC) != 0)
            {
                data[t] = malloc(sizeof(unsigned char));
                memcpy(data[t], old.data[t], sizeof(unsigned char));
            }
            if((parameter_types[t] & PT_US) != 0)
            {
                data[t] = malloc(sizeof(unsigned short));
                memcpy(data[t], old.data[t], sizeof(unsigned short));
            }
            if((parameter_types[t] & PT_UI) != 0)
            {
                data[t] = malloc(sizeof(unsigned int));
                memcpy(data[t], old.data[t], sizeof(unsigned int));
            }
            if((parameter_types[t] & PT_F) != 0)
            {
                data[t] = malloc(sizeof(float));
                memcpy(data[t], old.data[t], sizeof(float));
            }
            if((parameter_types[t] & PT_CS) != 0)
            {
                data[t] = malloc(strlen((char*)old.data[t])+1);
                memcpy(data[t], old.data[t], strlen((char*)old.data[t])+1);
            }
            if((parameter_types[t] & PT_VS) != 0)
            {
                data[t] = malloc(void_size[t]);
                memcpy(data[t], old.data[t], void_size[t]);
            }
        }
        else
        {
            int x, y;
            void** oldvals = (void**)old.data[t];
            if(oldvals == NULL)
            {
                data[t] = NULL;
                continue;
            }
            x = 0;
            while(oldvals[x] != NULL)
                x++;
            data[t] = malloc(sizeof(void*) * (x+1));
            void** newvals = (void**)data[t];
            newvals[x] = NULL;
            for(y=0;y<x;y++)
            {
	    	if(oldvals[y] == NULL)
		    continue;
                if((parameter_types[t] & PT_UC) != 0)
                {
                    newvals[y] = malloc(sizeof(unsigned char));
                    memcpy(newvals[y], oldvals[y], sizeof(unsigned char));
                }
                if((parameter_types[t] & PT_US) != 0)
                {
                    newvals[y] = malloc(sizeof(unsigned short));
                    memcpy(newvals[y], oldvals[y], sizeof(unsigned short));
                }
                if((parameter_types[t] & PT_UI) != 0)
                {
                    newvals[y] = malloc(sizeof(unsigned int));
                    memcpy(newvals[y], oldvals[y], sizeof(unsigned int));
                }
                if((parameter_types[t] & PT_F) != 0)
                {
                    newvals[y] = malloc(sizeof(float));
                    memcpy(newvals[y], oldvals[y], sizeof(float));
                }
                if((parameter_types[t] & PT_CS) != 0)
                {
                    newvals[y] = malloc(strlen((char*)oldvals[y])+1);
                    memcpy(newvals[y], oldvals[y], strlen((char*)oldvals[y])+1);
                }
                if((parameter_types[t] & PT_VS) != 0)
                {
                    newvals[y] = malloc(void_size[t]);
                    memcpy(newvals[y], oldvals[y], void_size[t]);
                }
            }
        }
    }
}

rsvp_configuration& rsvp_configuration::operator=(const rsvp_configuration& old)
{
    int t;
    for(t=0;t<256;t++)
    {
        parameter_types[t] = old.parameter_types[t];
        void_size[t] = old.void_size[t];
        if(old.data[t] == NULL)
        {
            data[t] = NULL;
            continue;
        }
        if((parameter_types[t] & PT_TAB) == 0)
        {
            if((parameter_types[t] & PT_UC) != 0)
            {
                data[t] = malloc(sizeof(unsigned char));
                memcpy(data[t], old.data[t], sizeof(unsigned char));
            }
            if((parameter_types[t] & PT_US) != 0)
            {
                data[t] = malloc(sizeof(unsigned short));
                memcpy(data[t], old.data[t], sizeof(unsigned short));
            }
            if((parameter_types[t] & PT_UI) != 0)
            {
                data[t] = malloc(sizeof(unsigned int));
                memcpy(data[t], old.data[t], sizeof(unsigned int));
            }
            if((parameter_types[t] & PT_F) != 0)
            {
                data[t] = malloc(sizeof(float));
                memcpy(data[t], old.data[t], sizeof(float));
            }
            if((parameter_types[t] & PT_CS) != 0)
            {
                data[t] = malloc(strlen((char*)old.data[t])+1);
                memcpy(data[t], old.data[t], strlen((char*)old.data[t])+1);
            }
            if((parameter_types[t] & PT_VS) != 0)
            {
                data[t] = malloc(void_size[t]);
                memcpy(data[t], old.data[t], void_size[t]);
            }
        }
        else
        {
            int x, y;
            void** oldvals;
            void** newvals;
            oldvals = (void**)old.data[t];
            if(oldvals == NULL)
            {
                data[t] = NULL;
                continue;
            }
            x = 0;
            while(oldvals[x] != NULL)
                x++;
            data[t] = malloc(sizeof(void*) * x+1);
            newvals = (void**)data[t];
            newvals[x] = NULL;
            for(y=0;y<x;y++)
            {
	        if(oldvals[y] == NULL)
		    continue;
                if((parameter_types[t] & PT_UC) != 0)
                {
                    newvals[y] = malloc(sizeof(unsigned char));
                    memcpy(newvals[y], oldvals[y], sizeof(unsigned char));
                }
                if((parameter_types[t] & PT_US) != 0)
                {
                    newvals[y] = malloc(sizeof(unsigned short));
                    memcpy(newvals[y], oldvals[y], sizeof(unsigned short));
                }
                if((parameter_types[t] & PT_UI) != 0)
                {
                    newvals[y] = malloc(sizeof(unsigned int));
                    memcpy(newvals[y], oldvals[y], sizeof(unsigned int));
                }
                if((parameter_types[t] & PT_F) != 0)
                {
                    newvals[y] = malloc(sizeof(float));
                    memcpy(newvals[y], oldvals[y], sizeof(float));
                }
                if((parameter_types[t] & PT_CS) != 0)
                {
                    newvals[y] = malloc(strlen((char*)oldvals[y])+1);
                    memcpy(newvals[y], oldvals[y], strlen((char*)oldvals[y])+1);
                }
                if((parameter_types[t] & PT_VS) != 0)
                {
                    newvals[y] = malloc(void_size[t]);
                    memcpy(newvals[y], oldvals[y], void_size[t]);
                }
            }
        }
    }
    return *this;
}

rsvp_configuration::~rsvp_configuration()
{
    int t, x;
    void** vals;

    for(x=0;x<256;x++)
    {
        if((parameter_types[x] & PT_TAB) != 0)
        {
            vals = (void**)data[x];
            t = 0;
            if(vals != NULL)
                while(vals[t] != NULL)
                    free(vals[t++]);
        }
    }
    for(t=0;t<256;t++)
        free(data[t]);
}

int rsvp_configuration::add(unsigned char parameter, unsigned char value)
{
    if((parameter_types[parameter] & PT_UC) == 0)
        return ERROR_INC_PAR;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if(data[parameter] != NULL)
            return ERROR_VAL_PRES;
        data[parameter] = malloc(sizeof(unsigned char));
        if(data[parameter] == NULL)
            return ERROR_MALLOC;
        *(unsigned char*)data[parameter] = value;
    }
    else
    {
        int t;
        unsigned char** vals;

        vals = (unsigned char**)data[parameter];
        if(vals == NULL)
        {
            vals = (unsigned char**)malloc(sizeof(unsigned char*));
            vals[0] = NULL;
        }
        t = 0;
        while(vals[t] != NULL)
            t++;
        vals[t] = (unsigned char*)malloc(sizeof(unsigned char));
        vals[t][0] = value;
        vals = (unsigned char**)realloc(vals, sizeof(unsigned char*) * (t+2));
        vals[t+1] = NULL;
        data[parameter] = (void*)vals;
    }

    return 0;
}

int rsvp_configuration::add(unsigned char parameter, unsigned short value)
{
    if((parameter_types[parameter] & PT_US) == 0)
        return ERROR_INC_PAR;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if(data[parameter] != NULL)
            return ERROR_VAL_PRES;
        data[parameter] = malloc(sizeof(unsigned short));
        if(data[parameter] == NULL)
            return ERROR_MALLOC;
        *(unsigned short*)data[parameter] = value;
    }
    else
    {
        int t;
        unsigned short** vals;

        vals = (unsigned short**)data[parameter];
        if(vals == NULL)
        {
            vals = (unsigned short**)malloc(sizeof(unsigned short*));
            vals[0] = NULL;
        }
        t = 0;
        while(vals[t] != NULL)
            t++;
        vals[t] = (unsigned short*)malloc(sizeof(unsigned short));
        vals[t][0] = value;
        vals = (unsigned short**)realloc(vals, sizeof(unsigned short*) * (t+2));
        vals[t+1] = NULL;
        data[parameter] = (void*)vals;
    }

    return 0;
}

int rsvp_configuration::add(unsigned char parameter, unsigned int value)
{
    if((parameter_types[parameter] & PT_UI) == 0)
        return ERROR_INC_PAR;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if(data[parameter] != NULL)
            return ERROR_VAL_PRES;
        data[parameter] = malloc(sizeof(unsigned int));
        if(data[parameter] == NULL)
            return ERROR_MALLOC;
        *(unsigned int*)data[parameter] = value;
    }
    else
    {
        int t;
        unsigned int** vals;

        vals = (unsigned int**)data[parameter];
        if(vals == NULL)
        {
            vals = (unsigned int**)malloc(sizeof(unsigned int*));
            vals[0] = NULL;
        }
        t = 0;
        while(vals[t] != NULL)
            t++;
        vals[t] = (unsigned int*)malloc(sizeof(unsigned int));
        vals[t][0] = value;
        vals = (unsigned int**)realloc(vals, sizeof(unsigned int*) * (t+2));
        vals[t+1] = NULL;
        data[parameter] = (void*)vals;
    }

    return 0;
}

int rsvp_configuration::add(unsigned char parameter, const char* value)
{
    int l;
    l = strlen(value);
    if(l<0)
        return ERROR_STRLEN;
    if((parameter_types[parameter] & PT_CS) == 0)
        return ERROR_INC_PAR;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if(data[parameter] != NULL)
            return ERROR_VAL_PRES;
        data[parameter] = malloc((l+1) * sizeof(char));
        if(data[parameter] == NULL)
            return ERROR_MALLOC;
        strncpy((char*)data[parameter], value, l);
        ((char*)data[parameter])[l] = 0;
    }
    else
    {
        int t;
        char** vals;

        vals = (char**)data[parameter];
        if(vals == NULL)
        {
            vals = (char**)malloc(sizeof(char*));
            vals[0] = NULL;
        }
        t = 0;
        while(vals[t] != NULL)
            t++;
        vals[t] = (char*)malloc((l+1) * sizeof(char));
        strncpy(vals[t], value, l);
        vals[t][l] = 0;
        vals = (char**)realloc(vals, sizeof(char*) * (t+2));
        vals[t+1] = NULL;
        data[parameter] = (void*)vals;
    }

    return 0;
}

int rsvp_configuration::add(unsigned char parameter, void* value, int size)
{
    if((parameter_types[parameter] & PT_VS) == 0)
    {
        return ERROR_INC_PAR;
    }
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if(data[parameter] != NULL)
            return ERROR_VAL_PRES;
           data[parameter] = malloc(size);
        if(data[parameter] == NULL)
            return ERROR_MALLOC;
        memcpy(data[parameter], value, size);
        void_size[parameter] = size;
    }
    else
    {
        int t;
        void** vals;

        if(void_size[parameter] != 0 && void_size[parameter] != size)
            return ERROR_DIFF_VOID_SIZE;
        void_size[parameter] = size;
        vals = (void**)data[parameter];
        if(vals == NULL)
        {
            vals = (void**)malloc(sizeof(void*));
            vals[0] = NULL;
        }
        t = 0;
        while(vals[t] != NULL)
            t++;
        vals[t] = (void*)malloc(size);
        memcpy(vals[t], value, size);
        vals = (void**)realloc(vals, sizeof(void*) * (t+2));
        vals[t+1] = NULL;
        data[parameter] = (void*)vals;
    }

    return 0;
}

int rsvp_configuration::add(unsigned char parameter, float value)
{
    if((parameter_types[parameter] & PT_F) == 0)
        return ERROR_INC_PAR;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if(data[parameter] != NULL)
            return ERROR_VAL_PRES;
        data[parameter] = malloc(sizeof(float));
        if(data[parameter] == NULL)
            return ERROR_MALLOC;
        *(float*)data[parameter] = value;
    }
    else
    {
        int t;
        float** vals;

        vals = (float**)data[parameter];
        if(vals == NULL)
        {
            vals = (float**)malloc(sizeof(float*));
            vals[0] = NULL;
        }
        t = 0;
        while(vals[t] != NULL)
            t++;
        vals[t] = (float*)malloc(sizeof(float));
        vals[t][0] = value;
        vals = (float**)realloc(vals, sizeof(float*) * (t+2));
        vals[t+1] = NULL;
        data[parameter] = (void*)vals;
    }

    return 0;
}


int rsvp_configuration::set(unsigned char parameter, unsigned char value, int index)
{
    if((parameter_types[parameter] & PT_UC) == 0)
        return ERROR_INC_PAR;
    if(data[parameter] == NULL)
        return ERROR_VAL_NOT_PRES;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        *(unsigned char*)data[parameter] = value;
    }
    else
    {
        int t;
        unsigned char** vals;

        vals = (unsigned char**)data[parameter];
        t = 0;
        while(vals[t] != NULL && t < index)
            t++;
        if(vals[t] != NULL && t == index)
            vals[t][0] = value;
        else
            return ERROR_VAL_NOT_PRES;
    }

    return 0;
}

int rsvp_configuration::set(unsigned char parameter, unsigned short value, int index)
{
    if((parameter_types[parameter] & PT_US) == 0)
        return ERROR_INC_PAR;
    if(data[parameter] == NULL)
        return ERROR_VAL_NOT_PRES;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        *(unsigned short*)data[parameter] = value;
    }
    else
    {
        int t;
        unsigned short** vals;

        vals = (unsigned short**)data[parameter];
        t = 0;
        while(vals[t] != NULL && t < index)
            t++;
        if(vals[t] != NULL && t == index)
            vals[t][0] = value;
        else
            return ERROR_VAL_NOT_PRES;
    }

    return 0;
}

int rsvp_configuration::set(unsigned char parameter, unsigned int value, int index)
{
    if((parameter_types[parameter] & PT_UI) == 0)
        return ERROR_INC_PAR;
    if(data[parameter] == NULL)
        return ERROR_VAL_NOT_PRES;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        *(unsigned int*)data[parameter] = value;
    }
    else
    {
        int t;
        unsigned int** vals;

        vals = (unsigned int**)data[parameter];
        t = 0;
        while(vals[t] != NULL && t < index)
            t++;
        if(vals[t] != NULL && t == index)
            vals[t][0] = value;
        else
            return ERROR_VAL_NOT_PRES;
    }

    return 0;
}

int rsvp_configuration::set(unsigned char parameter, char* value, int index)
{
    int l;

    l = strlen(value);
    if(l<0)
        return ERROR_STRLEN;
    if((parameter_types[parameter] & PT_CS) == 0)
        return ERROR_INC_PAR;
    if(data[parameter] == NULL)
        return ERROR_VAL_NOT_PRES;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        data[parameter] = realloc(data[parameter], (l+1) * sizeof(char));
        strncpy((char*)data[parameter], value, l);
        ((char*)data[parameter])[l] = 0;
    }
    else
    {
        int t;
        char** vals;

        vals = (char**)data[parameter];
        t = 0;
        while(vals[t] != NULL && t < index)
            t++;
        if(vals[t] != NULL && t == index)
        {
            vals[t] = (char*)realloc(vals[t], (l+1) * sizeof(char));
            strncpy(vals[t], value, l);
            ((char*)vals[t])[l] = 0;
        }
        else
            return ERROR_VAL_NOT_PRES;
    }

    return 0;
}

int rsvp_configuration::set(unsigned char parameter, void* value, int size, int index)
{
    if((parameter_types[parameter] & PT_VS) == 0)
        return ERROR_INC_PAR;
    if(data[parameter] == NULL)
        return ERROR_VAL_NOT_PRES;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        data[parameter] = realloc(data[parameter], size);
        memcpy(data[parameter], value, size);
        void_size[parameter] = size;
    }
    else
    {
        int t;
        void** vals;

        if(void_size[parameter] != size)
            return ERROR_DIFF_VOID_SIZE;
        void_size[parameter] = size;
        vals = (void**)data[parameter];
        t = 0;
        while(vals[t] != NULL && t < index)
            t++;
        if(vals[t] != NULL && t == index)
        {
            vals[t] = realloc(vals[t], size);
            memcpy(vals[t], value, size);
        }
        else
            return ERROR_VAL_NOT_PRES;
    }

    return 0;
}

int rsvp_configuration::set(unsigned char parameter, float value, int index)
{
    if((parameter_types[parameter] & PT_F) == 0)
        return ERROR_INC_PAR;
    if(data[parameter] == NULL)
        return ERROR_VAL_NOT_PRES;
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        *(float*)data[parameter] = value;
    }
    else
    {
        int t;
        float** vals;

        vals = (float**)data[parameter];
        t = 0;
        while(vals[t] != NULL && t < index)
            t++;
        if(vals[t] != NULL && t == index)
            vals[t][0] = value;
        else
            return ERROR_VAL_NOT_PRES;
    }

    return 0;
}

int rsvp_configuration::setadd(unsigned char parameter, unsigned char value, int index)
{
    if(set(parameter, value, index) != 0)
        return add(parameter, value);
}

int rsvp_configuration::setadd(unsigned char parameter, unsigned short value, int index)
{
    if(set(parameter, value, index) != 0)
        return add(parameter, value);
}

int rsvp_configuration::setadd(unsigned char parameter, unsigned int value, int index)
{
    if(set(parameter, value, index) != 0)
        return add(parameter, value);
}

int rsvp_configuration::setadd(unsigned char parameter, char* value, int index)
{
    if(set(parameter, value, index) != 0)
        return add(parameter, value);
}

int rsvp_configuration::setadd(unsigned char parameter, void* value, int size, int index)
{
    if(set(parameter, value, size, index) != 0)
        return add(parameter, value, size);
}

int rsvp_configuration::setadd(unsigned char parameter, float value, int index)
{
    if(set(parameter, value, index) != 0)
        return add(parameter, value);
}


int rsvp_configuration::remove(unsigned char parameter, int index)
{
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        free(data[parameter]);
        data[parameter] = NULL;
        void_size[parameter] = 0;
    }
    else
    {
        void** vals;
        int t;
        vals = (void**)data[parameter];
        if(vals != NULL)
        {
            t = 0;
            while(t < index && vals[t] != NULL)
                t++;
            if(t < index)
                return ERROR_VAL_NOT_PRES;
            if(t == index)
            {
                if(vals[t] == NULL)
                    return ERROR_VAL_NOT_PRES;
                free(vals[t]);
                while(vals[t] != NULL)
                {
                    vals[t] = vals[t+1];
                    t++;
                }
                vals = (void**)realloc(vals, sizeof(void*) * t+1);
                if(ispresent(parameter) == VAL_FALSE)
                    void_size[parameter] = 0;
            }
        }
        else
            return ERROR_VAL_NOT_PRES;
    }
    return 0;
}


int rsvp_configuration::ispresent(unsigned char parameter, int index)
{
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if(data[parameter] == NULL)
            return VAL_FALSE;
        else
            return VAL_TRUE;
    }
    else
    {
        void** vals;
        int t;

        if(data[parameter] == NULL)
            return VAL_FALSE;
        vals = (void**)data[parameter];
        t = 0;
        while(t<index && vals[t] != NULL)
            t++;
        if(t == index && vals[t] != NULL)
            return VAL_TRUE;
        else
            return VAL_FALSE;
    }
}


void* rsvp_configuration::read(unsigned char parameter, int index, int *size)
{
    void* buf;

    if(ispresent(parameter, index) == VAL_FALSE)
        return NULL;

    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if((parameter_types[parameter] & PT_UC) != 0)
        {
            buf = malloc(sizeof(unsigned char));
            memcpy(buf, data[parameter], sizeof(unsigned char));
            return buf;
        }
        if((parameter_types[parameter] & PT_US) != 0)
        {
            buf = malloc(sizeof(unsigned short));
            memcpy(buf, data[parameter], sizeof(unsigned short));
            return buf;
        }
        if((parameter_types[parameter] & PT_UI) != 0)
        {
            buf = malloc(sizeof(unsigned int));
            memcpy(buf, data[parameter], sizeof(unsigned int));
            return buf;
        }
        if((parameter_types[parameter] & PT_F) != 0)
        {
            buf = malloc(sizeof(float));
            memcpy(buf, data[parameter], sizeof(float));
            return buf;
        }
        if((parameter_types[parameter] & PT_CS) != 0)
        {
            buf = strdup((char*)data[parameter]);
            return buf;
        }
        if((parameter_types[parameter] & PT_VS) != 0)
        {
            buf = malloc(void_size[parameter]);
            memcpy(buf, data[parameter], void_size[parameter]);
            if(size != NULL)
                *size = void_size[parameter];
            return buf;
        }
    }
    else
    {
        void** vals;
        vals = (void**)data[parameter];

        if((parameter_types[parameter] & PT_UC) != 0)
        {
            buf = malloc(sizeof(unsigned char));
            memcpy(buf, vals[index], sizeof(unsigned char));
            return buf;
        }
        if((parameter_types[parameter] & PT_US) != 0)
        {
            buf = malloc(sizeof(unsigned short));
            memcpy(buf, vals[index], sizeof(unsigned short));
            return buf;
        }
        if((parameter_types[parameter] & PT_UI) != 0)
        {
            buf = malloc(sizeof(unsigned int));
            memcpy(buf, vals[index], sizeof(unsigned int));
            return buf;
        }
        if((parameter_types[parameter] & PT_F) != 0)
        {
            buf = malloc(sizeof(float));
            memcpy(buf, vals[index], sizeof(float));
            return buf;
        }
        if((parameter_types[parameter] & PT_CS) != 0)
        {
            buf = strdup((char*)vals[index]);
            return buf;
        }
        if((parameter_types[parameter] & PT_VS) != 0)
        {
            buf = malloc(void_size[parameter]);
            memcpy(buf, vals[index], void_size[parameter]);
            if(size != NULL)
                *size = void_size[parameter];
            return buf;
        }
    }
    return NULL;
}

int rsvp_configuration::copy_from(unsigned char parameter, rsvp_configuration conf)
{
    int size;
    void* buf;

    if(conf.ispresent(parameter) == VAL_FALSE)
        return ERROR_VAL_NOT_PRES;
    while(ispresent(parameter) == VAL_TRUE)
        remove(parameter);
    if((parameter_types[parameter] & PT_TAB) == 0)
    {
        if((parameter_types[parameter] & PT_UC) != 0)
            add(parameter, *((unsigned char*)conf.read(parameter)));
        if((parameter_types[parameter] & PT_US) != 0)
            add(parameter, *((unsigned short*)conf.read(parameter)));
        if((parameter_types[parameter] & PT_UI) != 0)
            add(parameter, *((unsigned int*)conf.read(parameter)));
        if((parameter_types[parameter] & PT_F) != 0)
            add(parameter, *((float*)conf.read(parameter)));
        if((parameter_types[parameter] & PT_CS) != 0)
            add(parameter, (char*)conf.read(parameter));
        if((parameter_types[parameter] & PT_VS) != 0)
        {
            buf = conf.read(parameter, 0, &size);
            add(parameter, buf, size);
            free(buf);
        }
    }
    else
    {
        int t;

        t = 0;
        if((parameter_types[parameter] & PT_UC) != 0)
            while(conf.ispresent(parameter, t) == VAL_TRUE)
                add(parameter, *((unsigned char*)conf.read(parameter, t++)));
        if((parameter_types[parameter] & PT_US) != 0)
            while(conf.ispresent(parameter, t) == VAL_TRUE)
                add(parameter, *((unsigned short*)conf.read(parameter, t++)));
        if((parameter_types[parameter] & PT_UI) != 0)
            while(conf.ispresent(parameter, t) == VAL_TRUE)
                add(parameter, *((unsigned int*)conf.read(parameter, t++)));
        if((parameter_types[parameter] & PT_F) != 0)
            while(conf.ispresent(parameter, t) == VAL_TRUE)
                add(parameter, *((float*)conf.read(parameter, t++)));
        if((parameter_types[parameter] & PT_CS) != 0)
            while(conf.ispresent(parameter, t) == VAL_TRUE)
                add(parameter, (char*)conf.read(parameter, t++));
        if((parameter_types[parameter] & PT_VS) != 0)
            while(conf.ispresent(parameter, t) == VAL_TRUE)
            {
                buf = conf.read(parameter, t++, &size);
                add(parameter, buf, size);
                free(buf);
            }
    }
    return 0;
}

int rsvp_configuration::add_missing(rsvp_configuration add_from)
{
    int t;
    for(t=0;t<256;t++)
        if((this->ispresent(t) == VAL_FALSE) && (add_from.ispresent(t) == VAL_TRUE))
            this->copy_from(t, add_from);
    return 0;
}

int rsvp_configuration::update(rsvp_configuration add_from)
{
    int t;
    for(t=0;t<256;t++)
        if(add_from.ispresent(t) == VAL_TRUE)
            this->copy_from(t, add_from);
    return 0;
}

#endif

