#include "c_messageparser_defines.h"

/*!
  \brief Structure holding message data.
*/
typedef struct {
    uint8_t len;
    uint8_t data[255];
} message_t;

/*!
  \brief Initializes the message parser ensures the message heap is empty and values are zeroes
  Must be called before using the library.

  \param[in] none.

  \retval void.
*/
void init_messageparser();

/*!
  \brief Clears existing message heap irrespective
  Invalidates all messages on heap and threads that call recv will fail after this call.
  Built in for exceptional cases and debugging

  \param[in] none.

  \retval void.
*/
void clear_messageparser();

/*!
  \brief User needs to call this function to get a new message from the message heap
  memory allocation is dealt with by the library
  delete_message() needs to be called to free the memory from message heap.

  \param[in] none.

  \retval message_t* if space is available on message heap.
  \retval NULL no space is available on message heap.
*/
message_t* new_message();

/*!
  \brief User needs to call this function to return message back to message heap

  \param[in] message_t* pointer to valid message structure

  \retval void.
*/
void delete_message(message_t* msg);

/*!
  \brief Asynchronously sends message to destination ID provided(pthread id)
  Single thread can send multiple messages to same receiver thread.

  \param[in] message_t* pointer to valid message structure.
  \param[in] destination_id valid thread ID of destination thread.

  \retval C_MSGPARSER_SUCCESS Successfully sent message.
  \retval C_MSGPARSER_ERROR_MSG_Q_FULL Message Queue full.
*/
int send(unsigned long int destination_id, message_t* msg);

/*!
  \brief Asynchronously receives messages intended for receiving thread(pthread id)
  Order of messages is not guaranteed.

  \param[in] message_t** pointer to pointer of local structure variable to hold return data.
  \param[in] destination_id valid thread ID of receiving thread.

  \retval C_MSGPARSER_SUCCESS Successfully received message.
  \retval C_MSGPARSER_ERROR_MSG_NO_MSG No message exists for thread.
*/
int recv(unsigned long int receiver_id, message_t** msg);