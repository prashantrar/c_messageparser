#include "c_messageparser.h"
#include <pthread.h>

typedef struct {
    message_t msg;
    unsigned long int source_id;
    unsigned long int destination_id;
} message_q;

static message_q msg_queue[C_MSGPARSER_MSG_Q_SIZE] = {};

static int msg_queue_len = C_MSGPARSER_MSG_Q_SIZE;

void init_messageparser() {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        msg_queue[itr].destination_id = C_MSG_PARSER_INVALID_DESTINATION_ID;
        msg_queue[itr].source_id = C_MSG_PARSER_INVALID_SOURCE_ID;
        memset(&msg_queue[itr].msg,0,sizeof(message_t));
    }
}

void clear_messageparser() {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        delete_message(&msg_queue[itr].msg);
    }
}

message_t* new_message() {
    if(msg_queue_len == C_MSGPARSER_MSQ_Q_EMPTY) return C_MSGPARSER_INVALID_MSG;
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        if(msg_queue[itr].source_id == C_MSG_PARSER_INVALID_SOURCE_ID) {
            msg_queue[itr].source_id = pthread_self();
            --msg_queue_len;
            return &msg_queue[itr].msg;
        }
    }
    return C_MSGPARSER_INVALID_MSG;
}

void delete_message(message_t* msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        if(&msg_queue[itr].msg==msg) {
            msg_queue[itr].destination_id = C_MSG_PARSER_INVALID_DESTINATION_ID;
            msg_queue[itr].source_id = C_MSG_PARSER_INVALID_SOURCE_ID;
            memset(&msg_queue[itr].msg,0,sizeof(message_t));
            ++msg_queue_len;
        }
    }
}

int send(unsigned long int destination_id, message_t* msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        if(&msg_queue[itr].msg==msg && msg_queue[itr].source_id==pthread_self()) {
            msg_queue[itr].destination_id = destination_id;
            return C_MSGPARSER_SUCCESS;
        }
    }
    return C_MSGPARSER_ERROR_MSG_Q_FULL;
}

int recv(unsigned long int receiver_id, message_t** msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        if(msg_queue[itr].destination_id==receiver_id) {
            *msg = &msg_queue[itr].msg;
            return C_MSGPARSER_SUCCESS;
        }
    }
    return C_MSGPARSER_ERROR_MSG_NO_MSG;
}