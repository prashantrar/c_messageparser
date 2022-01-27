#include "c_messageparser.h"
#include "c_messageparser_tests.h"

/*!
  Helper function to spawn Receive threads.

  This function is used as a start routine for threads that
  receive data from the message heap. The semaphore mechanism
  ensures that send is completed successfully before receive
  starts.
*/
void* recvfn(void* args) {
    sem_wait(&test_start);
    message_t* recv_msg = C_MSGPARSER_INVALID_MSG;
    recv(pthread_self(),&recv_msg);
    if(recv_msg!=C_MSGPARSER_INVALID_MSG) {
        if(recv_msg->data[0]==254) {
            delete_message(recv_msg);
            sem_post(&test_start);
            return (void*)C_MSG_PARSER_TEST_SUCCESS;
        }
    }
    sem_post(&test_start);
    return (void*)C_MSG_PARSER_TEST_FAIL;
}

/*!
  Helper function to spawn send threads.

  This function is used as a start routine for threads that
  send data to the message heap. The arguments passed in
  should contain the ID of the destination thread.
*/
void* sendfn(void* args) {
    thread_args* send_args = (thread_args*) args;
    message_t* send_msg = new_message();
    if(send_msg!=C_MSGPARSER_INVALID_MSG) {
        send_msg->data[0] = 254;
        ASSERT_EQ(C_MSGPARSER_SUCCESS, send(send_args->destination_id,send_msg));
    } else {
        return (void*)C_MSG_PARSER_TEST_FAIL;
    }
    return (void*)C_MSG_PARSER_TEST_SUCCESS;
}

/*!
  Test to check normal case of multiple sends and receives.
  
  This test sends 5 messages to the library and creates the
  necessary receive threads. Receive threads are created first
  anf wait on a semaphore that is posted after all send threads 
  are completed.
*/
int testRepeatTxRx() {
    sem_wait(&test_start);
    pthread_t send_t[5];
    pthread_t recv_t[5];
    uint8_t itr = 0;
    //Create receive threads
    for(itr=0;itr<5;++itr) {
        pthread_create(&recv_t[itr],NULL,recvfn,NULL);
    }
    //Create and join send threads
    for(itr=0;itr<5;++itr) {
        thread_args* send_args = (thread_args*)malloc(sizeof(thread_args));
        send_args->destination_id = recv_t[itr];
        pthread_create(&send_t[itr],NULL,sendfn,send_args);
        int send_status;
        pthread_join(send_t[itr],(void**)&send_status);
        if(send_status != C_MSG_PARSER_TEST_SUCCESS) {
            return C_MSG_PARSER_TEST_FAIL;
        }
        free(send_args);
    }
    //Post semaphore to trigger receive.
    sem_post(&test_start);
    //Wait for and join all receive threads
    for(itr=0;itr<5;++itr) {
        int recv_status;
        pthread_join(recv_t[itr],(void**)&recv_status);
        if(recv_status != C_MSG_PARSER_TEST_SUCCESS) {
            return C_MSG_PARSER_TEST_FAIL;
        }
    }
    return C_MSG_PARSER_TEST_SUCCESS;
}

/*!
  Test to check if recv is called without any send
  
  This test case is meant to fail.
*/
int testRecvNone() {
    pthread_t recv_t;
    pthread_create(&recv_t,NULL,recvfn,NULL);
    int recv_status;
    pthread_join(recv_t,(void**)&recv_status);
    if(recv_status != C_MSG_PARSER_TEST_SUCCESS) {
        return C_MSG_PARSER_TEST_FAIL;
    }
    return C_MSG_PARSER_TEST_SUCCESS;
}

/*!
  Test to check if send is called beyond message heap capacity.
  
  This test case is meant to fail.
*/
int testSendMaxLimit() {
    pthread_t send_t;
    thread_args* send_args = (thread_args*)malloc(sizeof(thread_args));
    send_args->destination_id = -1;
    uint8_t itr = 0;
    for(itr=0;itr<=C_MSGPARSER_MSG_HEAP_SIZE+1;++itr) {
        pthread_create(&send_t,NULL,sendfn,send_args);
        int send_status;
        pthread_join(send_t,(void**)&send_status);
        if(send_status != C_MSG_PARSER_TEST_SUCCESS) {
            clear_messageparser();
            return C_MSG_PARSER_TEST_FAIL;
        }
    }
    return C_MSG_PARSER_TEST_SUCCESS;
}

/*!
  Test to check sending and receiving from multiple threads
  
  This test sends single messages to the library and creates a
  single receive thread. Receive thread is created first
  anf wait on a semaphore that is posted after all send threads 
  are completed.
*/
int testMultipleThreadTxRx() {
    sem_wait(&test_start);
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
    sem_post(&test_start);
    int recv_status;
    pthread_join(recv_t,(void**)&recv_status);
    if(recv_status != C_MSG_PARSER_TEST_SUCCESS) {
        return C_MSG_PARSER_TEST_FAIL;
    }
    return C_MSG_PARSER_TEST_SUCCESS;
}

/*!
  Test send and receive from same thread.

*/
int testSingleThreadTxRx() {
    sem_wait(&test_start);
    message_t* send_msg = new_message();
    if(send_msg!=C_MSGPARSER_INVALID_MSG) {
        send_msg->data[0] = 254;
        ASSERT_EQ(C_MSGPARSER_SUCCESS, send(pthread_self(),send_msg));
    } else {
        return C_MSG_PARSER_TEST_FAIL;
    }
    sem_post(&test_start);
    message_t* recv_msg = C_MSGPARSER_INVALID_MSG;
    ASSERT_EQ(C_MSGPARSER_SUCCESS, recv(pthread_self(),&recv_msg));
    if(recv_msg!=C_MSGPARSER_INVALID_MSG) {
        if(recv_msg->data[0]==254) {
            delete_message(recv_msg);
            return C_MSG_PARSER_TEST_SUCCESS;
        }
    }
    return C_MSG_PARSER_TEST_FAIL;

}

void test_main() {
    init_messageparser();
    sem_init(&test_start, 0, 1);
    TEST_EQ(C_MSG_PARSER_TEST_SUCCESS,testSingleThreadTxRx());
    TEST_EQ(C_MSG_PARSER_TEST_SUCCESS,testMultipleThreadTxRx());
    TEST_EQ(C_MSG_PARSER_TEST_FAIL,testSendMaxLimit());
    TEST_EQ(C_MSG_PARSER_TEST_FAIL,testRecvNone());
    TEST_EQ(C_MSG_PARSER_TEST_SUCCESS,testRepeatTxRx());
    sem_destroy(&test_start);
}

int main() {
    test_main();
    return 0;
}