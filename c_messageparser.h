#include "c_messageparser_defines.h"

typedef struct {
    uint8_t len;
    uint8_t data[255];
} message_t;

void init_messageparser();

void clear_messageparser();

message_t* new_message();

void delete_message(message_t* msg);

int send(unsigned long int destination_id, message_t* msg);

int recv(unsigned long int receiver_id, message_t** msg);