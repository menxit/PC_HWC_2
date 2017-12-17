#include <stdlib.h>
#include <printf.h>
#include "Reader.h"
#include "../poison_pill/poison_pill.h"
#define READER_ERROR NULL;

static void *_subscribeTask(void *args) {
  Reader *this = (Reader *) args;
  buffer_t *buffer = this->_buffer;
  msg_t *msg = get_bloccante(buffer);
  while(msg != POISON_PILL_MSG) {
    //printf("Reader ha ricevuto un messaggio.\n");
    this->_numberOfConsumedMessages++;
    this->_task(msg, NULL);
    msg = get_bloccante(buffer);
  }
  this->_isSubscribed = 0;
  //free(this);
  pthread_exit(0);
}

static int subscribe(struct Reader *this, Dispatcher *dispatcher) {
  this->_buffer = dispatcher->subscribe(dispatcher);
  this->_isSubscribed = 1;
  //printf("Reader si è registrato al dispatcher.\n");
  pthread_t thread;
  pthread_create(&thread, NULL, this->_subscribeTask, this);
  pthread_detach(thread);
  return 1;
}

static unsigned long getNumberOfMessagesConsumed(Reader* this) {
  return this->_numberOfConsumedMessages;
}

static unsigned short getIsSubscribed(Reader* this) {
  return this->_isSubscribed;
}

static void sendPoisonPill(Reader* this) {
  put_bloccante(this->_buffer, POISON_PILL_MSG);
}

Reader *_new_Reader(void *task(msg_t *msg, void* args)) {
  Reader *this = malloc(sizeof(Reader));
  this->_isSubscribed = 0;
  this->_numberOfConsumedMessages = 0;
  this->_task = task;
  this->_subscribeTask = _subscribeTask;
  this->subscribe = subscribe;
  this->getNumberOfMessagesConsumed = getNumberOfMessagesConsumed;
  this->getIsSubscribed = getIsSubscribed;
  this->sendPoisonPill = sendPoisonPill;
  return this;
}
