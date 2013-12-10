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
 
#ifndef MPLS_IOCTL_BODY_INCLUDED
#define MPLS_IOCTL_BODY_INCLUDED

#include "mpls_ioctl.h"



int mpls_ioctl::make_ioctl(int req, void* ptr)
{
    int result;
    int errs;
    int fd;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
        return -2;

    result = ioctl(fd, req, ptr);
    errs = errno;
    close(fd);
    errno = errs;
    return result;
}

unsigned int mpls_ioctl::get_if_index(const char* name)
{
    unsigned int if_index;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    make_ioctl(SIOCGIFINDEX, &ifr);
    return ifr.ifr_ifindex;
}

struct sockaddr mpls_ioctl::get_sockaddr_addr(unsigned int addr)
{
    struct sockaddr s;
    struct sockaddr_in sin;

    memset(&s, 0, sizeof(struct sockaddr));
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_addr.s_addr = addr;
    sin.sin_family = AF_INET;
    memcpy(&s, &sin, sizeof(struct sockaddr));
    return s;
}

int mpls_ioctl::set_in_label(unsigned int label)
{
#ifndef __LINUX_MPLS_ABSENCE__
    struct mpls_in_label_req mil_r;

    memset(&mil_r, 0, sizeof(struct mpls_in_label_req));
    mil_r.mil_label.ml_type = MPLS_LABEL_GEN;
    mil_r.mil_label.u.ml_gen = label;
    mil_r.mil_label.ml_index = 0;
    return make_ioctl(SIOCMPLSILMADD, &mil_r);
#else /* __LINUX_MPLS_ABSENCE__ */
	return 0;
#endif /* __LINUX_MPLS_ABSENCE__ */
}

int mpls_ioctl::set_out_label(unsigned int label, const char* if_name, unsigned int addr)
{
#ifndef __LINUX_MPLS_ABSENCE__
    // plain and empty label add
    struct mpls_out_label_req mol_r;
    unsigned int new_key;

    memset(&mol_r, 0, sizeof(struct mpls_out_label_req));
    mol_r.mol_label.ml_type = MPLS_LABEL_KEY;
    mol_r.mol_label.u.ml_key = 0;
    make_ioctl(SIOCMPLSNHLFEADD, &mol_r);
    new_key = mol_r.mol_label.u.ml_key;

    // let's add instructions! :)
    struct mpls_instr_req mir_r;

    memset(&mir_r, 0, sizeof(struct mpls_instr_req));
    mir_r.mir_direction = MPLS_OUT;
    mir_r.mir_instr[0].mir_opcode = MPLS_OP_PUSH;
    mir_r.mir_instr[0].mir_data.push.ml_type = MPLS_LABEL_GEN;
    mir_r.mir_instr[0].mir_data.push.u.ml_gen = label;
    mir_r.mir_instr[1].mir_opcode = MPLS_OP_SET;
    mir_r.mir_instr[1].mir_data.set.mni_if = get_if_index(if_name);
    mir_r.mir_instr[1].mir_data.set.mni_addr = get_sockaddr_addr(addr);
    mir_r.mir_instr_length = 2;

    memcpy(&mir_r.mir_label, &mol_r.mol_label, sizeof(struct mpls_label));
    make_ioctl(SIOCSMPLSOUTINSTR, &mir_r);

    return new_key;
#else /* __LINUX_MPLS_ABSENCE__ */
	return 0;
#endif /* __LINUX_MPLS_ABSENCE__ */
}

int mpls_ioctl::delete_in_label(unsigned int label)
{
#ifndef __LINUX_MPLS_ABSENCE__
    struct mpls_in_label_req mil_r;

    memset(&mil_r, 0, sizeof(struct mpls_in_label_req));
    mil_r.mil_label.ml_type = MPLS_LABEL_GEN;
    mil_r.mil_label.u.ml_gen = label;
    mil_r.mil_label.ml_index = 0;
    return make_ioctl(SIOCMPLSILMDEL, &mil_r);
#else /* __LINUX_MPLS_ABSENCE__ */
	return 0;
#endif /* __LINUX_MPLS_ABSENCE__ */
}

int mpls_ioctl::delete_out_label(unsigned int key)
{
#ifndef __LINUX_MPLS_ABSENCE__
    struct mpls_out_label_req mol_r;

    memset(&mol_r, 0, sizeof(struct mpls_out_label_req));
    mol_r.mol_label.ml_type = MPLS_LABEL_KEY;
    mol_r.mol_label.u.ml_key = key;
    return make_ioctl(SIOCMPLSNHLFEDEL, &mol_r);
#else /* __LINUX_MPLS_ABSENCE__ */
	return 0;
#endif /* __LINUX_MPLS_ABSENCE__ */
}

int mpls_ioctl::set_labelspace(int labelspace, const char* if_name)
{
#ifndef __LINUX_MPLS_ABSENCE__
    struct mpls_labelspace_req mls_r;

    memset(&mls_r, 0, sizeof(struct mpls_labelspace_req));
    mls_r.mls_ifindex = get_if_index(if_name);
    mls_r.mls_labelspace = labelspace;
    return make_ioctl(SIOCSLABELSPACEMPLS, &mls_r);
#else /* __LINUX_MPLS_ABSENCE__ */
	return 0;
#endif /* __LINUX_MPLS_ABSENCE__ */
}


#endif
