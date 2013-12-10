#ifndef MGMT_PACKET_HEADER_FILE
#define MGMT_PACKET_HEADER_FILE

#include "configuration.h"


//--------------------------------------------------------------------------------------------------
// data types

#define DATA_TYPE_UC    1        // unsigned char   , 1 byte
#define DATA_TYPE_US    2        // unsigned short  , 2 bytes
#define DATA_TYPE_UI    3        // unsigned int    , 4 bytes
#define DATA_TYPE_UL    4        // unsigned long   , 8 bytes   
#define DATA_TYPE_F     5        // float           , 4 bytes
#define DATA_TYPE_CS    6        // string          , specified length
#define DATA_TYPE_V     7        // unspecified data, specified length


//===============================================================================================

class mgmt_packet
{
private:
    void* contents;               // binary representation of message
    int contents_length;          // length of binary data
    int objects_count;            // number of objects in packet

    configuration conf;           // configuration with objects
    unsigned char packet_type;    // message type for packet
    unsigned char packet_comm;    // message command for packet
    unsigned short packet_id;     // identification number of packet

    /* add specific object types to packet */
    int add_unsigned_int(unsigned short id, unsigned int value);
    int add_ipaddr(unsigned short id, unsigned int addr);
    int add_unsigned_short(unsigned short id, unsigned short value);
    int add_unsigned_char(unsigned short id, unsigned char value);
    int add_float(unsigned short id, float value);
    int add_string(unsigned short id, const char* name, int size);

    /* init packet */
    void init();

public:
    /* constructor for incoming binary data */
    mgmt_packet(char* data, int length);
        
    /* constructor for created message */
    mgmt_packet(configuration* conf, unsigned short id, unsigned char type, unsigned char comm);

    /* return binary representation of message */
    void* get_data(int* size);

    /* getters for parsed message information */
    unsigned char get_packet_type()  {return packet_type;};
    unsigned char get_packet_comm()  {return packet_comm;};
    unsigned short get_packet_id()   {return packet_id;};
    configuration get_conf()         {return conf;};

    /* destructor */
    ~mgmt_packet();
};

//===============================================================================================

#endif
