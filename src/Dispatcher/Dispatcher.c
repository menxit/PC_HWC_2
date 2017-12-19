#include <stdlib.h>
#include <printf.h>
#include "Dispatcher.h"
#include "../poison_pill/poison_pill.h"

#define SIZE_BUFFER_PROVIDER 3
#define SIZE_BUFFER_READER 5
#define DISPATCHER_ERROR 0;

static int addMessageOnProviderBuffer(Dispatcher* this, msg_t *message) {
  return put_bloccante(this->_bufferProvider, message) != NULL;
}

static buffer_t *subscribe(Dispatcher* this) {
  if (pc_sem_wait(this->_useListOfBufferReader) != 0) {
    return DISPATCHER_ERROR;
  }
  buffer_t *buffer = buffer_init(SIZE_BUFFER_READER);
  addElement(this->_listOfBufferReader, buffer);
  pc_sem_post(this->_useListOfBufferReader);
  return buffer;
}

static void *_sendPoisonPillTask(void *args) {
  buffer_t *buffer = (buffer_t *) args;
  put_bloccante(buffer, POISON_PILL_MSG);
  pthread_exit(0);
}

static pthread_t* _sendPoisonPill(buffer_t* bufferReader, int detached) {
  pthread_t *sendPoisonPillThread = malloc(sizeof(pthread_t));
  pthread_create(sendPoisonPillThread, NULL, _sendPoisonPillTask, bufferReader);
  if(detached == 1) {
    pthread_detach(*sendPoisonPillThread);
  }
  return sendPoisonPillThread;
}

static int _broadcastPoisonPill(Dispatcher *this) {
  if (pc_sem_wait(this->_useListOfBufferReader) != 0) {
    return DISPATCHER_ERROR;
  }
  iterator_t *iterator = iterator_init(this->_listOfBufferReader);
  list_t *listOfPThread = list_init();
  while(hasNext(iterator)) {
    buffer_t *bufferReader = next(iterator);
    addElement(listOfPThread, _sendPoisonPill(bufferReader, 0));
    removeElement(this->_listOfBufferReader, bufferReader);
  }
  iterator_destroy(iterator);
  iterator = iterator_init(listOfPThread);
  while(hasNext(iterator)) {
    pthread_join(*(pthread_t *)next(iterator), NULL);
  }
  pc_sem_post(this->_useListOfBufferReader);
  return 1;
}

static int _broadcastMessage(Dispatcher* this, msg_t* message) {

  if (pc_sem_wait(this->_useListOfBufferReader) != 0) {
    return DISPATCHER_ERROR;
  }

  iterator_t *iterator = iterator_init(this->_listOfBufferReader);

  while(hasNext(iterator)) {

    buffer_t *bufferReader = next(iterator);

    // reader troppo lento, richiesta rimozione
    if(put_non_bloccante(bufferReader, message) == BUFFER_ERROR) {
      _sendPoisonPill(bufferReader, 1);
      removeElement(this->_listOfBufferReader, bufferReader);
    }

  }

  iterator_destroy(iterator);

  pc_sem_post(this->_useListOfBufferReader);

  return 1;
}

static void *_startTask(void *args) {
  Dispatcher *this = (Dispatcher *) args;

  for(;;) {

    msg_t *message = get_bloccante(this->_bufferProvider);

    if(message == POISON_PILL_MSG) {
      this->_broadcastPoisonPill(this);
      break;
    }

    this->_broadcastMessage(this, message);

  }

  pthread_exit(0);
}

static void _start(struct Dispatcher* this) {
  pthread_create(this->_startTaskID, NULL, _startTask, this);
}

static void _wait(Dispatcher *this) {
  pthread_join(*this->_startTaskID, NULL);
}


Dispatcher *_new_Dispatcher() {
  Dispatcher *this = malloc(sizeof(Dispatcher));

  // Private properties
  this->_bufferProvider = buffer_init(SIZE_BUFFER_PROVIDER);
  this->_listOfBufferReader = list_init();
  this->_useListOfBufferReader = malloc(sizeof(pc_sem_t));
  this->_startTaskID = malloc(sizeof(pthread_t));
  pc_sem_init(this->_useListOfBufferReader, 1);

  // Private methods
  this->_broadcastPoisonPill = _broadcastPoisonPill;
  this->_broadcastMessage = _broadcastMessage;

  // Public methods
  this->addMessageOnProviderBuffer = addMessageOnProviderBuffer;
  this->subscribe = subscribe;
  this->start = _start;
  this->wait = _wait;

  return this;
}
