#include "c_messageparser.h"
#include <pthread.h>

/* Structure wrapping message_t and also holding src and dest thread ID's */
typedef struct {
    message_t msg;
    unsigned long int source_id;
    unsigned long int destination_id;
} message_pkt;

/* Static message heap as array to make library thread safe and prevent non file access*/
static message_pkt msg_heap[C_MSGPARSER_MSG_HEAP_SIZE] = {};

/* Variable to hold maximum heap size and tracking memory */
static int msg_heap_len = C_MSGPARSER_MSG_HEAP_SIZE;

/* initializes default values and zeros out structures */
void init_messageparser() {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_HEAP_SIZE;++itr) {
        msg_heap[itr].destination_id = C_MSG_PARSER_INVALID_DESTINATION_ID;
        msg_heap[itr].source_id = C_MSG_PARSER_INVALID_SOURCE_ID;
        memset(&msg_heap[itr].msg,0,sizeof(message_t));
    }
}

/* Clears message heap irrespective of receive status */
void clear_messageparser() {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_HEAP_SIZE;++itr) {
        delete_message(&msg_heap[itr].msg);
    }
}

/* Checks if message struct is available in heap and returns memory if available */
message_t* new_message() {
    if(msg_heap_len == C_MSGPARSER_MSQ_HEAP_EMPTY) return C_MSGPARSER_INVALID_MSG;
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_HEAP_SIZE;++itr) {
        if(msg_heap[itr].source_id == C_MSG_PARSER_INVALID_SOURCE_ID) {
            msg_heap[itr].source_id = pthread_self();
            --msg_heap_len;
            return &msg_heap[itr].msg;
        }
    }
    return C_MSGPARSER_INVALID_MSG;
}

/*Deletes message ID's and zeros out structures */
void delete_message(message_t* msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_HEAP_SIZE;++itr) {
        if(&msg_heap[itr].msg==msg) {
            msg_heap[itr].destination_id = C_MSG_PARSER_INVALID_DESTINATION_ID;
            msg_heap[itr].source_id = C_MSG_PARSER_INVALID_SOURCE_ID;
            memset(&msg_heap[itr].msg,0,sizeof(message_t));
            ++msg_heap_len;
        }
    }
}

/* Adds message tagged with destination ID on message heap */
int send(unsigned long int destination_id, message_t* msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_HEAP_SIZE;++itr) {
        if(&msg_heap[itr].msg==msg && msg_heap[itr].source_id==pthread_self()) {
            msg_heap[itr].destination_id = destination_id;
            return C_MSGPARSER_SUCCESS;
        }
    }
    return C_MSGPARSER_ERROR_MSG_Q_FULL;
}

/* Receives message tagged with destination ID on message heap */
int recv(unsigned long int receiver_id, message_t** msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_HEAP_SIZE;++itr) {
        if(msg_heap[itr].destination_id==receiver_id) {
            *msg = &msg_heap[itr].msg;
            return C_MSGPARSER_SUCCESS;
        }
    }
    return C_MSGPARSER_ERROR_MSG_NO_MSG;
}