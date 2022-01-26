#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define C_MSGPARSER_MSG_Q_SIZE 128
#define C_MSGPARSER_MSQ_Q_EMPTY 0

#define C_MSGPARSER_INVALID_MSG NULL
#define C_MSGPARSER_INVALID_MSG_LEN -0Xffff
#define C_MSG_PARSER_INVALID_DESTINATION_ID -0xfffe
#define C_MSG_PARSER_INVALID_SOURCE_ID -0xfffd

#define C_MSGPARSER_SUCCESS 0
#define C_MSGPARSER_ERROR_MSG_Q_FULL -1
#define C_MSGPARSER_ERROR_MSG_NO_MSG -2


#define C_MSG_PARSER_TEST_SUCCESS 0
#define C_MSG_PARSER_TEST_FAIL -1


#define ASSERT_EQ(e1,e2) assert(e1==e2)
#define TEST_EQ(e1,e2) {ASSERT_EQ(e1,e2); fprintf(stdout, "SUCCESS: %s\n", #e2);}