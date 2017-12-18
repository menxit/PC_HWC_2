#include <stdlib.h>
#include <pthread.h>
#include <list.h>
#include <printf.h>
#include "Provider.h"
#include "../poison_pill/poison_pill.h"

#define PROVIDER_ERROR 0

static void *_startTask(void *args) {
  Provider *this = (Provider *) args;
  iterator_t *iterator = iterator_init(this->_messagesToSend);
  while(hasNext(iterator)) {
    usleep(this->_sleep);
    msg_t *msg = next(iterator);
    this->_dispatcher->addMessageOnProviderBuffer(this->_dispatcher, msg);
  }
  this->_dispatcher->addMessageOnProviderBuffer(this->_dispatcher, POISON_PILL_MSG);
  pc_sem_post(this->_providerIsNotRunning);
  pthread_exit(0);
}

static int _start(Provider *this) {

  // Se il Provider è già stato avviato restituire un errore
  if (pc_sem_trywait(this->_providerIsNotRunning) != 0) {
    return PROVIDER_ERROR;
  }

  // Altrimenti crea un flusso di esecuzione e restituisci 1
  pthread_t startTaskThread;
  pthread_create(&startTaskThread, NULL, this->_startTask, this);
  this->_startTaskID = startTaskThread;
  return 1;
}

static void _wait(Provider *this) {
  pthread_join(this->_startTaskID, NULL);
}

Provider *_new_Provider(list_t *messagesToSend, Dispatcher *dispatcher, unsigned int sleep) {
  Provider *this = malloc(sizeof(Provider));

  // Private properties
  this->_sleep = sleep;
  this->_messagesToSend = messagesToSend;
  this->_dispatcher = dispatcher;
  this->_providerIsNotRunning = malloc(sizeof(pc_sem_t));
  pc_sem_init(this->_providerIsNotRunning, 1);

  // Private methods
  this->_startTask = _startTask;

  // Public methods
  this->start = _start;
  this->wait = _wait;

  return this;
}
