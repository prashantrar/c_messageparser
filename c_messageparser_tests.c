#include "c_messageparser.h"
int testMultipleThreadTxRx() {
    
}
int testSingleThreadTxRx() {
    message_t* send_msg = new_message();
    send_msg->data[0] = 254;
    if(send_msg!=C_MSGPARSER_INVALID_MSG) {
        ASSERT_EQ(C_MSGPARSER_SUCCESS, send(123,send_msg));
    } else {
        return C_MSG_PARSER_TEST_FAIL;
    }
    message_t* recv_msg = C_MSGPARSER_INVALID_MSG;
    ASSERT_EQ(C_MSGPARSER_SUCCESS, recv(123,&recv_msg));
    if(recv_msg!=C_MSGPARSER_INVALID_MSG) {
        if(recv_msg->data[0]==254) return C_MSG_PARSER_TEST_SUCCESS;
    }
    return C_MSG_PARSER_TEST_FAIL;

}
void test_main() {
    init_messageparser();
    TEST_EQ(C_MSG_PARSER_TEST_SUCCESS,testSingleThreadTxRx());
}


int main() {
    test_main();
    return 0;
}