#include <stdlib.h>
#include "Accepter.h"

#define BUFFER_SIZE_ACCEPTER 10
#define SEND_REQUEST_ERROR NULL;

static msg_t *sendRequest(Accepter *this, Reader *reader) {
  msg_t *msg_reader = msg_init((void *) reader);
  if (put_bloccante(this->_bufferAccepter, msg_reader) == NULL) {
    return SEND_REQUEST_ERROR;
  }
  return msg_reader;
}

list_t *getSubscribedReaders(Accepter *this) {
  return this->_listOfSubscribedReaders;
}

Accepter *_new_Accepter(Dispatcher *dispatcher) {
  Accepter *this = malloc(sizeof(Accepter));

  // Private properties
  this->_bufferAccepter = buffer_init(BUFFER_SIZE_ACCEPTER);
  this->_listOfSubscribedReaders = list_init();

  // Public methods
  this->getSubscribedReaders = getSubscribedReaders;
  this->sendRequest = sendRequest;

  return this;
}
