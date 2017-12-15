#include <stdlib.h>
#include "Accepter.h"
#include "../poison_pill/poison_pill.h"

#define BUFFER_SIZE_ACCEPTER 10
#define ACCEPTER_SEND_REQUEST_ERROR NULL;

static msg_t *sendReader(Accepter *this, Reader *reader) {
  msg_t *msg_reader = msg_init((void *) reader);
  return this->sendRequest(this, msg_reader);
}

static msg_t *sendRequest(Accepter *this, msg_t *msg) {
  if (put_bloccante(this->_bufferAccepter, msg) == NULL) {
    return ACCEPTER_SEND_REQUEST_ERROR;
  }
  return msg;
}

static msg_t *sendPoisonPill(struct Accepter* this) {
  return this->sendRequest(this, POISON_PILL_MSG);
}

static list_t *getSubscribedReaders(Accepter *this) {
  return this->_listOfSubscribedReaders;
}

static void* _startTask(void* args) {
  Accepter *this = (Accepter *)args;
  buffer_t *buffer = this->_bufferAccepter;
  msg_t *msg = get_bloccante(buffer);
  while(msg != POISON_PILL_MSG) {
    this->_dispatcher->addMessageOnProviderBuffer(this->_dispatcher, msg);
    this->_requestSentToDispatcher++;
    msg = get_bloccante(buffer);
  }
  this->_isRunning = 0;
  pthread_exit(0);
}

static int start(Accepter* this) {
  this->_isRunning = 1;
  pthread_t thread;
  pthread_create(&thread, NULL, this->_startTask, this);
  pthread_detach(thread);
  return 1;
}

unsigned short getIsRunning(Accepter* this) {
  return this->_isRunning;
}

Accepter *_new_Accepter(Dispatcher *dispatcher) {
  Accepter *this = malloc(sizeof(Accepter));

  // Private properties
  this->_bufferAccepter = buffer_init(BUFFER_SIZE_ACCEPTER);
  this->_listOfSubscribedReaders = list_init();
  this->_dispatcher = dispatcher;
  this->_requestSentToDispatcher = 0;
  this->_isRunning = 0;

  // Private methods
  this->_startTask = _startTask;

  // Public methods
  this->getSubscribedReaders = getSubscribedReaders;
  this->sendRequest = sendRequest;
  this->sendReader = sendReader;
  this->sendPoisonPill = sendPoisonPill;
  this->start = start;
  this->getIsRunning = getIsRunning;

  return this;
}
