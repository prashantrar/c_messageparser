#include "c_messageparser.h"

static message_q msg_queue[C_MSGPARSER_MSG_Q_SIZE] = {};

void init_messageparser() {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_Q_SIZE;++itr) {
        msg_queue[itr].msg = C_MSGPARSER_INVALID_MSG;
        msg_queue[itr].destination_id = -1;
    }
}

message_t* new_message() {
    message_t* ret = (message_t*)malloc(sizeof(message_t));
    return ret;
}

void delete_message(message_t* msg) {
    free(msg);
}

int send(uint8_t destination_id, message_t* msg) {
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

int recv(uint8_t receiver_id, message_t** msg) {
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