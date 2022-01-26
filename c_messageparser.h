#include "c_messageparser_defines.h"

typedef struct {
    uint8_t len;
    uint8_t data[255];
} message_t;

typedef struct {
    message_t* msg;
    int destination_id;
} message_q;

void init_messageparser();

message_t* new_message();

void delete_message(message_t* msg);

int send(int destination_id, message_t* msg);

int recv(int receiver_id, message_t** msg);