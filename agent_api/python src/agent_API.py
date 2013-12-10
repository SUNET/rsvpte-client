#!/usr/bin/env python

"""
 ***************************************************************************
 *   This is an implementation of OIF UNI-C RSVP and IETF UNI-C RSVP.      *
 *   management API.                                                *
 *   The code was created for purpose of EC IST Mupbed project.            *
 *   Copyright (C) 2007 by Poznan Supercomputing and Networking Center     *
 *   http://www.man.poznan.pl                                              *
 *   Author: Damian Parniewicz, email: damianp at man.poznan.pl            *
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
 ***************************************************************************
 """

import sys, socket, struct, traceback, binascii, copy

#====================================================================

API_MANAGEMENT_PORT = 35000          # change this value when your RSVP agent is working with with another port number.
REQUEST_TIMEOUT     = 10             # seconds; change this value according espected time of responses

#====================================================================
"""
An object list element contain an object name, an object type number and an object data type.
"""
OBJECT_LIST = """
DT_DST_ADDR         1   131     dst             # rsvp session destination address
DT_SRC_ADDR         2   131     src             # rsvp session source address   (extended tunnel id)
DT_OUT_IF_ADDR      3   131     out_if_addr     # agent's outgoing interface (HOP neighbor address)
DT_REFRESH          4   3       refresh         # path refresh period
DT_TB_RATE          5   5       tb_rate         # token bucket rate
DT_TB_SIZE          6   5       tb_size         # token bucket size
DT_PEAK_DATA_RATE   7   5       peak_rate       # peak data rate
DT_UPSTREAM_LABEL   8   3       up_label        # upstream label (for bidirectional paths)
DT_LSP_ENC          9   1       enc             # lsp encoding type
DT_LSP_SW           10  1       switch_type     # lsp switching type
DT_LSP_GPID         11  2       gpid            # lsp gpid
DT_NAME             12  6       name            # path name
DT_ERO_ADDR         13  131     ero             # ERO address
DT_ERO_PREF_LEN     14  1       ero_pref_len    # ERO prefix length
DT_ERO_SL           15  1       ero_sl          # ERO strict/loose
DT_RSVP_HOP_ADDR    16  131     hop             # RSVP Hop address (deprecated)
DT_ERR_NODE_ADDR    17  131     err_node        # error node address
DT_ERR_FLAGS        18  1       err_flag        # error flags
DT_ERR_CODE         19  1       err_code        # error code
DT_ERR_VALUE        20  2       err_val         # error value
DT_MPART_NUMBER     21  3       mpart_number    # multiple part message - packet number (0..N-1) (STATUS ACK/NACK only)
DT_MPART_COUNT      22  3       mpart_count     # multiple part message - total packet count (N) (STATUS ACK/NACK only)
DT_TUNNEL_ID        23  2       tun_id          # tunnel id
DT_TR_D_ADDR        24  131     traff_dst       # traffic destination address         (for data plane only)
DT_TR_RD_ADDR       25  131     traff_rev_dst   # traffic reverse destination address (for data plane only)
DT_OUT_IF_HANDLE    26  3       interface_id    # data forward/reverse interface id
DT_SRC_TNA          27  131     src_tna         # source TNA
DT_DST_TNA          28  131     dst_tna         # destination TNA
DT_CALL_ID_1        29  3       call_id_1       # call id - part 1
DT_CALL_ID_2        30  3       call_id_2       # call id - part 2
DT_SENDER_LSP_ID    31  2       lsp_id          # sender LSP id
DT_SETUP_PRIOR      32  1       setup_prior     # setup priority
DT_HOLD_PRIOR       33  1       hold_prior      # hold priority
DT_PATH_STATE       34  2       path_state      # path state (STATUS ACK only)
DT_SERVICE_LEVEL    35  1       service_lvl     # protection service level
DT_ETHERNET_CIR     36  5       eth_cir         # ethernet committed information rate 
DT_ETHERNET_CBS     37  5       eth_cbr         # ethernet committed burst size
DT_ETHERNET_EIR     38  5       eth_eir         # ethernet excess information rate
DT_ETHERNET_EBS     39  5       eth_ebs         # ethernet excess burst size
DT_ETHERNET_SW_GRANUALITY 40 2  eth_granuality  # type of ethernet link
DT_ETHERNET_INDEX   41  1       eth_index       # index of bandwidth allocated for given QoS
DTETHERNET_PROFILE  42  1       eth_profile     # flags of color mode and coupling
"""
"""
Additional info:            rsvp config fields in an agent rsvp
DT_DST_ADDR         <->     RC_SESSION_DEST_ADDR    (SESSION object)
DT_SRC_ADDR         <->     RC_EXTENDED_IPV4_ADDR   (SESSION object)
DT_OUT_IF_ADDR      <->     RC_CONTROL_OUT_IF_ADDR  (HOP object - OIF)
DT_REFRESH          <->     RC_REFRESH_PERIOD       (TIME object)
DT_TB_RATE          <->     RC_TOKEN_BUCKET_RATE    (TOKEN_FLOWSPEC, TOKEN_SENDER_TSPEC object)
DT_TB_SIZE          <->     RC_TOKEN_BUCKET_SIZE    (TOKEN_FLOWSPEC, TOKEN_SENDER_TSPEC object)
DT_PEAK_DATA_RATE   <->     RC_PEAK_DATA_RATE       (TOKEN_FLOWSPEC, TOKEN_SENDER_TSPEC object)
DT_UPSTREAM_LABEL   <->     RC_UPSTREAM_LABEL       (UPSTREAM_LABEL object)
DT_LSP_ENC          <->     RC_LSP_ENC_TYPE         (GENERALIZED_LABEL_REQUEST object)
DT_LSP_SW           <->     RC_LSP_SWITCHING_TYPE   (GENERALIZED_LABEL_REQUEST object)
DT_LSP_GPID         <->     RC_LSP_GPID             (GENERALIZED_LABEL_REQUEST object)
DT_NAME             <->     RC_SESSION_NAME         (SESSION_ATTRIBUTE object - IETF)
DT_ERO_ADDR         <->     struct rc_ero.address   (ERO object)
DT_ERO_PREF_LEN     <->     struct rc_ero.prefix_length (ERO object)
DT_ERO_SL           <->     struct rc_ero.loose     (ERO object)
DT_RSVP_HOP_ADDR    <->     RC_RSVP_HOP_TLVS        (HOP TLV object - not used now)
DT_ERR_NODE_ADDR    <->     RC_ERROR_NODE_ADDR      (ERROR_SPEC object)
DT_ERR_FLAGS        <->     RC_ERROR_FLAGS          (ERROR_SPEC object)
DT_ERR_CODE         <->     RC_ERROR_CODE           (ERROR_SPEC object)
DT_ERR_VALUE        <->     RC_ERROR_VALUE          (ERROR_SPEC object)
DT_MPART_NUMBER     <-
DT_MPART_COUNT      <-
DT_TUNNEL_ID        <->     RC_TUNNEL_ID            (SESSION object)
DT_TR_D_ADDR        <->     RC_TR_D_ADDR            (mpls-linux)
DT_TR_RD_ADDR       <->     RC_TR_RD_ADDR           (mpls-linux)
DT_OUT_IF_HANDLE    <->     RC_CONTROL_OUT_IF_HANDLE(HOP object)
DT_SRC_TNA          <->     RC_TNA_SRC_ADDR         (GENERALIZED_UNI, CALL_ID object - OIF)
DT_DST_TNA          <->     RC_TNA_DST_ADDR         (GENERALIZED_UNI object - OIF)
DT_CALL_ID_1        <->     RC_CALL_ID_LOCAL_ID1    (CALL_ID object - OIF)
DT_CALL_ID_2        <->     RC_CALL_ID_LOCAL_ID2    (CALL_ID object - OIF)
DT_SENDER_LSP_ID    <->     RC_SENDER_SRC_ID        (SENDER_TEMPLATE object)
DT_SETUP_PRIOR      <->     RC_SETUP_PRIORITY       (SESSION_ATTRIBUTE object)
DT_HOLD_PRIOR       <->     RC_HOLDING_PRIORITY     (SESSION_ATTRIBUTE object)	
DT_PATH_STATE       <-
DT_SERVICE_LEVEL    <->     RC_SERVICE_LEVEL        (GENERALIZED_UNI object - OIF)
DT_ETHERNET_CIR     <->     RC_ETHERNET_CIR         (ETHERNET_FLOWSPEC, ETHERNET_SENDER_TSPEC object - OIF)
DT_ETHERNET_CBS     <->     RC_ETHERNET_CBS         (ETHERNET_FLOWSPEC, ETHERNET_SENDER_TSPEC object - OIF)
DT_ETHERNET_EIR     <->     RC_ETHERNET_EIR         (ETHERNET_FLOWSPEC, ETHERNET_SENDER_TSPEC object - OIF)
DT_ETHERNET_EBS     <->     RC_ETHERNET_EBS         (ETHERNET_FLOWSPEC, ETHERNET_SENDER_TSPEC object - OIF)
DT_ETHERNET_SW_GRANUALITY <-> RC_ETHERNET_SW_GRANUALITY (ETHERNET_FLOWSPEC, ETHERNET_SENDER_TSPEC object - OIF)
DT_ETHERNET_INDEX   <->     RC_ETHERNET_INDEX       (ETHERNET_FLOWSPEC, ETHERNET_SENDER_TSPEC object - OIF)
DT_ETHERNET_PROFILE <->     RC_ETHERNET_PROFILE     (ETHERNET_FLOWSPEC, ETHERNET_SENDER_TSPEC object - OIF)
"""

#====================================================================

DATA_TYPE_LIST = """
1   B       int     # unsigned char       (1 byte)
2   H       int     # unsigned short      (2 bytes)
3   I       int     # unsigned int        (4 bytes)
4   Q       int     # unsigned long int   (8 bytes)
5   f       int     # float               (4 bytes)
6   size    string  # char*               ('rozmiar' bytes)
7   size    int     # void                ('rozmiar' bytes)
131 ip_str  ip_addr # ip address in a dotted string format (internal use only - in packet replaced by 'obj_type'=3)
"""

#====================================================================

REQUIRED = """
CREATE  REQ     1 23
CREATE  ACK     1 23
CREATE  NACK    1 23
DELETE  REQ     23
DELETE  ACK     23
DELETE  NACK    23
STATUS  REQ
STATUS  ACK     23
STATUS  NACK    23
TRANSIT REQ     23
"""

#====================================================================

path_states = {0:'null', 1:'initiated', 2:'path sent', 6:'up', 11:'down'}

#====================================================================

# MESSAGE TYPES:
REQ     = 1
ACK     = 2
NACK    = 3
mes_types = {REQ:'REQ', ACK:'ACK', NACK:'NACK'}

#====================================================================

# COMMAND TYPES:
CREATE  = 1
DELETE  = 2
STATUS  = 3
TRANSIT = 4
command_types = {CREATE:'CREATE', DELETE:'DELETE', STATUS:'STATUS', TRANSIT:'TRANSIT'}

#====================================================================

object_list = {}
prefix_map = {}
desc_map = {}
for line in OBJECT_LIST.splitlines():
    line = line.split()
    if line == [] or line[0] == '#':
        continue
    object_list[line[0]] = {'obj_type':int(line[1]), 'data_type':int(line[2])}
    prefix_map['-' + line[3]] = line[0]
    try:    stop_index = line.index('(in')
    except: stop_index = len(line)
    desc_map['-' + line[3]] = line[5:stop_index]

#--------------------------------------------------------------------

data_list = {}
data_desc = {}
for line in DATA_TYPE_LIST.splitlines():
    line = line.split()
    if line == [] or line[0] == '#':
        continue
    data_type = int(line[0])
    data_list[data_type] = line[1]
    data_desc[data_type] = line[2]

#--------------------------------------------------------------------

required_list = {}
for line in REQUIRED.splitlines():
    line = line.split()
    if line == [] or line[0] == '#':
        continue
    cmd, msg_type = line[:2]
    required_list[(eval(cmd), eval(msg_type))] = [int(obj_type) for obj_type in line[2:]]

#--------------------------------------------------------------------

cache = {}
def find_obj_name(obj_type):
    if obj_type in cache:
        return cache[obj_type]
    for object_name, obj_dict in object_list.items():
        if obj_dict['obj_type'] == obj_type:
            cache[obj_type] = object_name
            return object_name

#====================================================================

def data_type_transforming(data_type, value):
    if data_type in (1,2,3,4):
        return int('+' + value, 0)
    elif data_type == 5:
        return float(value)
    elif data_type in  (6,7,131):
        return value
    else:
        print 'Missing data_type:', data_type, 'for value:', value

#--------------------------------------------------------------------

def find_arguments(argv, conf):
    for index in range(0, len(argv), 2):
        try:
            obj_name = prefix_map.get(argv[index])
            if obj_name == None: continue
            obj_value = argv[index+1]
            data_type = object_list[obj_name]['data_type']
            if conf.__dict__.get(obj_name, None) == None:
            	conf.__dict__[obj_name] = data_type_transforming(data_type, obj_value)
            elif type(conf.__dict__[obj_name]) == type([]):
                conf.__dict__[obj_name] += [data_type_transforming(data_type, obj_value)]
            else:
                conf.__dict__[obj_name] = [conf.__dict__[obj_name], data_type_transforming(data_type, obj_value)]
                
        except:
            traceback.print_exc()
    return conf
       
#--------------------------------------------------------------------

def present_usage():
    print '-----------------------------------------------------------------------'
    print 'The agent client usage:\n'
    print '1) Path creation:    ./agent_client.py create -tun_id <tunel_id> -dst <dest_addr> [other parameters,...]'
    print '2) Path deletion:    ./agent_client.py delete -tun_id <tunel_id>'
    print '3) Status query:     ./agent_client.py status [-tun_id <tunel_id>]'
    print '\nAvailable parameters:'
    prefixes = prefix_map.keys()
    prefixes.sort()
    for prefix in prefixes:
        obj_name = prefix_map[prefix]
        data_type = object_list[obj_name]['data_type']
        type_desc = data_desc[data_type]
        print '  %-15s %-13s %s' % (prefix, '<' + type_desc + '>', ' '.join(desc_map[prefix]))
    print '-----------------------------------------------------------------------'

#====================================================================

class rsvp_conf:
    """
    getting value of a noexist attribute throw the exception
    """
    def __init__(self, **kwargs):
        self.__dict__.update(kwargs)

    def __str__(self):
        keys = self.__dict__.keys()
        keys.sort()
        string = '\n'
        for name in keys:
            value = self.__dict__[name]
            if 'DT_' not in name: continue
            name = name[3:]
            if isinstance(value, long) or isinstance(value, int):
                string += '\t%25s: %s (%s)' % (name,value,hex(value))
            else:
                string += '\t%25s: %s' % (name,value)
            if name == 'DT_PATH_STATE':
                string += ' (path state %s)' % path_states.get(value, "unknown")
            string += '\n'
        return string

    def dict(self):
        return copy.deepcopy(self.__dict__)

    #-------------------------------------------------------------------------

    def check_requirements(self):
        """
        check for necessary attributes
        """
        req_list = ['msg_id', 'msg_type', 'msg_cmd']
        for obj_type in required_list.get((self.msg_cmd, self.msg_type), []):
            req_list += [find_obj_name(obj_type)]
         
        for obj_name in req_list:
            if obj_name not in self.__dict__:
                print "Object has no attribute:", obj_name
                return False
        return True

    #-------------------------------------------------------------------------
    
    def build(self):
        """
        build binary data with rsvp packet from rsvp configuration
        """
        def pack(fmt, *args):
            pack.bindata += [struct.pack('!'+fmt, *args)]
        pack.bindata = []

        def build_header(obj_name, obj_value):                  
            if obj_name[:3] != 'DT_':
                return
            try:
                obj_type  = object_list[obj_name]['obj_type']
                data_type = object_list[obj_name]['data_type']
                data_size = data_list[data_type]
            except:
                print "Unknown configuration field"
                traceback.print_exc()
                return
            pack('H', obj_type)
            if data_size in ('B', 'H', 'I', 'Q'):
                pack('B', data_type)
                pack('B', struct.calcsize(data_size))
                pack(data_size, obj_value)
            elif data_size == 'f':
                pack('B', data_type)
                pack('B', struct.calcsize(data_size))
                pack.bindata += [struct.pack('f', obj_value)]
            elif data_type == 6:
                pack('B', data_type)
                string_len = len(obj_value)
                pack('B', string_len)
                pack('%is'%string_len, obj_value)
            elif data_type == 7:
                print data_type, data_size, obj_value, type(obj_value)
                pack('B', data_type)
                if type(obj_value) == type(0):
                    obj_len = 4
                else:
                    obj_len = len(obj_value)
                print obj_len
                pack('B', obj_len)
                pack('B'*obj_len, obj_value)
            elif data_type == 131:
                pack('B', 3)
                pack('B', 4)
                pack.bindata += socket.inet_aton(obj_value)
            else:
                print 'Object building intenal error'
                return ""    
            self.msg_cnt += 1

        if(self.check_requirements() == False):
            print "Message requirements are not satisfied, a lack of the required fields..."
            return ""

        # build header
        self.msg_cnt = 0
        pack('H', self.msg_id)
        pack('B', self.msg_type)
        pack('B', self.msg_cmd)
        pack('B', self.msg_cnt) # temporary
        
        # build objects
        for obj_name,obj_values in self.__dict__.items():
            if type(obj_values) == type([]):
                for obj_value in obj_values:
                    build_header(obj_name,obj_value)
            else:
                build_header(obj_name,obj_values)
        
        pack.bindata[3] = struct.pack('!B', self.msg_cnt)
        return ''.join(pack.bindata)    # list to string
            
    #-------------------------------------------------------------------------
    
    def parse(self, bindata):
        """
        parse binary packet data to rsvp configuration
        """
        def unpack(fmt):
            calcsize = struct.calcsize(fmt)
            data = struct.unpack('!'+fmt, bindata[unpack.pos:unpack.pos+calcsize])[0]
            unpack.pos += calcsize
            return data
        unpack.pos = 0

        if len(bindata) < 5:
            print "Parsed bindata too short ! ", binascii.hexlify(bindata)
            return []
        
        # print binascii.hexlify(bindata)
        # parse header
        self.msg_id     = unpack('H')
        self.msg_type   = unpack('B')
        self.msg_cmd    = unpack('B')
        self.msg_cnd    = unpack('B')

        # parse objects
        cnd = 0
        while unpack.pos + 3 < len(bindata) and cnd < self.msg_cnd:
            obj_type  = unpack('H')
            obj_name  = 'DT_Unknown'
            try:
                obj_name = find_obj_name(obj_type)
                if obj_name == None:
                    continue
                data_type = object_list[obj_name]['data_type'] # data type getted from obj_list, not from packet!
                _data_type = unpack('B')
                if data_type != 131 and data_type != _data_type:
                    print "Data type field mismatch" 
            except:
                print 'Unknown object type'
                traceback.print_exc()
                data_type = unpack('B')
            
            if len(data_list.get(data_type, [])) == 1:
                unpack('B')
                data_size = data_list[data_type]
                if data_type == 5:
                    calcsize = struct.calcsize(data_size)
                    obj_value = struct.unpack(data_size, bindata[unpack.pos:unpack.pos+calcsize])[0]
                    unpack.pos += calcsize
                else:
                    obj_value = unpack(data_size)
            elif data_type == 6:
                data_size = unpack('B')
                obj_value = unpack('%is'%data_size)
            elif data_type == 7:
                data_size = unpack('B')
                obj_value = unpack('B'*data_size)
            elif data_type == 131:
                data_size = unpack('B')
                obj_value = socket.inet_ntoa(bindata[unpack.pos:unpack.pos+4])
                unpack.pos += 4
            else:
                print 'Object parsing intenal error'
                return []
            
            if self.__dict__.get(obj_name, None) == None:
            	self.__dict__[obj_name] = obj_value
            elif type(self.__dict__[obj_name]) == type([]):
                self.__dict__[obj_name] += [obj_value]
            else:
                self.__dict__[obj_name] = [self.__dict__[obj_name], obj_value]

            cnd += 1

        return bindata[unpack.pos:]

#====================================================================

class rsvp_client_api:
    msg_id = 0
    def __init__(self, ip_addr='127.0.0.1', port=API_MANAGEMENT_PORT):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((ip_addr, port))
        self.sock.settimeout(REQUEST_TIMEOUT) # ten second timeout

    #-------------------------------------------------------------------------
    
    def create_path(self, rsvp_conf):
        self.send_req_and_wait_resp(CREATE, rsvp_conf)

    #-------------------------------------------------------------------------
    
    def destroy_path(self, rsvp_conf):
        self.send_req_and_wait_resp(DELETE, rsvp_conf)

    #-------------------------------------------------------------------------
    
    def status_query(self, rsvp_conf):
        self.send_req_and_wait_resp(STATUS, rsvp_conf)

    #-------------------------------------------------------------------------

    def send_req_and_wait_resp(self, msg_cmd,rsvp_conf):
        if self.send_req(msg_cmd, rsvp_conf):
            print 'Request has been sent\nWaiting for response...'
            self.wait_for_message(msg_cmd)
            sys.stdout.flush()
            print "client stops"

    #-------------------------------------------------------------------------
    
    def send_req(self, msg_cmd,rsvp_conf):
        rsvp_client_api.msg_id += 1
        rsvp_conf.msg_id    = rsvp_client_api.msg_id
        rsvp_conf.msg_type  = REQ
        rsvp_conf.msg_cmd   = msg_cmd
        try:
            bin_msg = rsvp_conf.build()
            if bin_msg == '':
                print 'Message have not been built...'
                return False
            msg_len = self.sock.send(bin_msg)
            print 'Msg length is %d bytes - sent are %d bytes' % (msg_len, len(bin_msg))
            return msg_len == len(bin_msg)
        except:
            print "Request build or send unsuccessful"
            traceback.print_exc()
            return False

    #-------------------------------------------------------------------------
    
    def wait_for_message(self, msg_cmd):
        try:
            bin_msg = self.sock.recv(1500)
        except:
            return False
        if bin_msg == '':
            return False
        sys.stdout.flush()
        conf = rsvp_conf()
        try:
            while True:
                bin_msg = conf.parse(bin_msg)
                if conf.msg_type in (ACK, NACK) and conf.msg_cmd == msg_cmd:
                    print 'Received %s %s message:' % (command_types[conf.msg_cmd],mes_types[conf.msg_type]) , conf
                if len(bin_msg) <= 5:
                    return False
            return True
        except:
            print "Unsuccessful response parsing"
            traceback.print_exc()
        return False

#====================================================================
    
def test1():
    conf = rsvp_conf()
    conf.DT_TB_RATE = conf.DT_TB_SIZE = conf.DT_PEAK_DATA_RATE = 31250000

    ## build and parse test !!!!
    conf.DT_SRC_ADDR    = '127.0.0.1'
    conf.DT_TUNNEL_ID   = 10
    conf.DT_DST_ADDR    = '127.0.0.2'
    conf.DT_TR_D_ADDR   = '127.0.0.3'
    conf.msg_id    = 0
    conf.msg_type  = REQ
    conf.msg_cmd   = CREATE
    print 'Sent config:', conf
    bin = conf.build()
    print 'Sent binary:', binascii.hexlify(bin)
    conf2 = rsvp_conf()
    conf2.parse(bin)
    print 'Receiced config:', conf2
    ## end of test !!!! 
    
def test2():
    conf = rsvp_conf()
    
    conf.DT_TB_RATE = conf.DT_TB_SIZE = conf.DT_PEAK_DATA_RATE = 31250000
    conf.DT_SRC_ADDR    = '127.0.0.1'
    conf.DT_TUNNEL_ID   = 10
    conf.DT_DST_ADDR    = '127.0.0.2'
    conf.DT_TR_D_ADDR   = '127.0.0.3'
   
    try:
        client = rsvp_client_api()
    except:
        print "Client cannot connect to rsvp agent!"
        print "Connection refused"
        # traceback.print_exc()
        return
    client.create_path(conf)
    client.sock.close()

#====================================================================
if __name__ == "__main__":
    #test1()
    #test2()
    
#====================================================================
