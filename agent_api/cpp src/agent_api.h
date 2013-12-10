#ifndef AGENT_API_HEADER_FILE
#define AGENT_API_HEADER_FILE 

#include <pthread.h>
#include "mgmt_packet.h"

//--------------------------------------------------------------------------------------------------
// command types
#define COMMAND_CREATE  1
#define COMMAND_DELETE  2
#define COMMAND_STATUS  3

//--------------------------------------------------------------------------------------------------
// message types
#define TYPE_REQ        1
#define TYPE_ACK        2
#define TYPE_NACK       3

//--------------------------------------------------------------------------------------------------

class agent_api
{
private:
    unsigned int        agent_addr;          // ip address of the agent
    unsigned short      next_message_id;     // value of message id to use in next sent request
    unsigned short      agent_port;          // port number of the agent
    int                 socket_desc;         // descriptor of opened socket to agent 
    pthread_t           socket_listener_tid; // identificator of listener thread
    bool                running;             // flag of running of listener thread

    /* send packet to agent, success of operation returned */
    bool send(mgmt_packet* packet);
    /* initialization of agent_api with address and port of the agent */
    void init(unsigned int addr, unsigned short port);
    /* loop for incoming packet read */ 
    int receive_messages();
    /* listener thread function */
    static void socket_listener(agent_api* api);
public:
    /* constructor for a comunication with the agent on loopback address and port=35000 */
    agent_api();
    
    /* constructor for a comunication with the agent on declared address and port */
    agent_api(unsigned int addr, unsigned short port=35000);
    
    /* constructor for a comunication with the agent on declared address (in string format) and port */
    agent_api(const char* addr, unsigned short port=35000);
    
    /* destructor */
    ~agent_api();

    /* setup connection to the agent */
    void api_connect();

    /* close connection to the agent */
    void api_disconnect();

    /* send message to the agent with particular command, type and configuration
     * if message_id is omitted then next incremented value is used 
     * when you send request you shouldn't set message_id
     * when you send ack or nack for received request please set value of message_id of that request
     * returned value:
     *        >= 0:   message_id of the message is returned 
     *        < 0:    message sending is unsuccessful
     */
    int send_message(unsigned char command, unsigned char message_type, configuration* conf, int message_id=-1);

    /* this metod is invoke when message is received, arguments of this metod are read from received message */
    virtual void receive_message(unsigned char command, unsigned char message_type, configuration* conf, unsigned short message_id) = 0;
    
};



#endif /* AGENT_API_HEADER_FILE */
