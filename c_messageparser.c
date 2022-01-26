#include "c_messageparser.h"

static volatile message_q msg_queue[C_MSGPARSER_MSG_Q_SIZE] = {};

static volatile int msg_queue_len = C_MSGPARSER_MSG_Q_SIZE;

void init_messageparser() {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        msg_queue[itr].destination_id = C_MSG_PARSER_INVALID_DESTINATION_ID;
    }
}

message_t* new_message() {
    message_t* ret = (message_t*)malloc(sizeof(message_t));
    return ret;
}

void delete_message(message_t* msg) {
    free(msg);
}

int send(int destination_id, message_t* msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        if(msg_queue[itr].msg==C_MSGPARSER_INVALID_MSG) {
            msg_queue[itr].msg = msg;
            msg_queue[itr].destination_id = destination_id;
            return C_MSGPARSER_SUCCESS;
        }
    }
    return C_MSGPARSER_ERROR_MSG_Q_FULL;
}

int recv(int receiver_id, message_t** msg) {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        if(msg_queue[itr].destination_id==receiver_id) {
            *msg = msg_queue[itr].msg;

            msg_queue[itr].msg = C_MSGPARSER_INVALID_MSG;
            msg_queue[itr].destination_id = -1;
            return C_MSGPARSER_SUCCESS;
        }
    }
    return C_MSGPARSER_ERROR_MSG_NO_MSG;
}