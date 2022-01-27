#include "c_messageparser.h"
#include <pthread.h>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

/* Static message heap as array to make library thread safe and prevent non file access*/
static message_t msg_heap[C_MSGPARSER_MSG_HEAP_SIZE] = {};

static queue<pair<message_t*, int> > msg_heap_free;

static unordered_map<message_t*, int> msg_heap_used;

static unordered_map<unsigned long int, queue<message_t*> > msg_send_q;

/* initializes default values and zeros out structures */
void init_messageparser() {
    uint8_t itr = 0;
    for(itr=0;itr<C_MSGPARSER_MSG_HEAP_SIZE;++itr) {
        memset(&msg_heap[itr],0,sizeof(message_t));
        msg_heap_free.push( {&msg_heap[itr],itr} );
    }
}

/* Clears message heap irrespective of receive status */
void clear_messageparser() {
    for(auto i: msg_heap_used) {
        msg_heap_free.push({i.first,i.second});
    }
    msg_heap_used.clear();
    msg_send_q.clear();
}

/* Checks if message struct is available in heap and returns memory if available */
message_t* new_message() {
    if(msg_heap_free.empty()) return C_MSGPARSER_INVALID_MSG;
    auto it = msg_heap_free.front();
    msg_heap_free.pop();
    msg_heap_used[it.first] = it.second;
    return it.first;
}

/*Deletes message ID's and zeros out structures */
void delete_message(message_t* msg) {
    auto it = msg_heap_used.find(msg);
    if(it!=msg_heap_used.end()) {
        memset(&msg_heap[msg_heap_used[msg]],0,sizeof(message_t));
        msg_heap_free.push({msg, msg_heap_used[msg]});
        msg_heap_used.erase(it);
    }
}

/* Adds message tagged with destination ID on message heap */
int send(unsigned long int destination_id, message_t* msg) {
    auto it = msg_heap_used.find(msg);
    if(it!=msg_heap_used.end()) {
        msg_send_q[destination_id].push(msg);
        return C_MSGPARSER_SUCCESS;
    }
    return C_MSGPARSER_ERROR_MSG_NULL;
}

/* Receives message tagged with destination ID on message heap */
int recv(unsigned long int receiver_id, message_t** msg) {
    auto it = msg_send_q.find(receiver_id);
    if(it!=msg_send_q.end() && !it->second.empty()) {
        *msg = it->second.front();
        it->second.pop();
        if(it->second.empty()) msg_send_q.erase(receiver_id);
        return C_MSGPARSER_SUCCESS;
    }
    return C_MSGPARSER_ERROR_MSG_NO_MSG;
}