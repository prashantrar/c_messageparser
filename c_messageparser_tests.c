#include "c_messageparser.h"
#include "c_messageparser_tests.h"

void* recvfn(void* args) {
    sem_wait(&test_start);
    message_t* recv_msg = C_MSGPARSER_INVALID_MSG;
    ASSERT_EQ(C_MSGPARSER_SUCCESS, recv(pthread_self(),&recv_msg));
    if(recv_msg!=C_MSGPARSER_INVALID_MSG) {
        if(recv_msg->data[0]==254) {
            free(recv_msg);
            return (void*)C_MSG_PARSER_TEST_SUCCESS;
        }
    }
    return (void*)C_MSG_PARSER_TEST_FAIL;
}

void* sendfn(void* args) {
    thread_args* send_args = (thread_args*) args;
    message_t* send_msg = new_message();
    if(send_msg!=C_MSGPARSER_INVALID_MSG) {
        send_msg->data[0] = 254;
        ASSERT_EQ(C_MSGPARSER_SUCCESS, send(send_args->destination_id,send_msg));
    } else {
        return (void*)C_MSG_PARSER_TEST_FAIL;
    }
    sem_post(&test_start);
    return (void*)C_MSG_PARSER_TEST_SUCCESS;
}

int testMultipleThreadTxRx() {
    pthread_t send_t;
    pthread_t recv_t;
    pthread_create(&recv_t,NULL,recvfn,NULL);
    thread_args* send_args = (thread_args*)malloc(sizeof(thread_args));
    send_args->destination_id = recv_t;
    pthread_create(&send_t,NULL,sendfn,send_args);

    int send_status;
    pthread_join(send_t,(void**)&send_status);
    if(send_status != C_MSG_PARSER_TEST_SUCCESS) {
        return C_MSG_PARSER_TEST_FAIL;
    }
    int recv_status;
    pthread_join(recv_t,(void**)&recv_status);
    if(recv_status != C_MSG_PARSER_TEST_SUCCESS) {
        return C_MSG_PARSER_TEST_FAIL;
    }
    return C_MSG_PARSER_TEST_SUCCESS;
}

int testSingleThreadTxRx() {
    message_t* send_msg = new_message();
    if(send_msg!=C_MSGPARSER_INVALID_MSG) {
        send_msg->data[0] = 254;
        ASSERT_EQ(C_MSGPARSER_SUCCESS, send(pthread_self(),send_msg));
    } else {
        return C_MSG_PARSER_TEST_FAIL;
    }
    message_t* recv_msg = C_MSGPARSER_INVALID_MSG;
    ASSERT_EQ(C_MSGPARSER_SUCCESS, recv(pthread_self(),&recv_msg));
    if(recv_msg!=C_MSGPARSER_INVALID_MSG) {
        if(recv_msg->data[0]==254) {
            free(recv_msg);
            return C_MSG_PARSER_TEST_SUCCESS;
        }
    }
    return C_MSG_PARSER_TEST_FAIL;

}

void test_main() {
    init_messageparser();
    TEST_EQ(C_MSG_PARSER_TEST_SUCCESS,testSingleThreadTxRx());
    TEST_EQ(C_MSG_PARSER_TEST_SUCCESS,testMultipleThreadTxRx());
}

int main() {
    test_main();
    return 0;
}