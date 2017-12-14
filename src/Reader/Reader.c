#include <stdlib.h>
#include "Reader.h"
#include "../poison_pill/poison_pill.h"

static void *_subscribeTask(void *args) {
  Reader *this = (Reader *) args;
  msg_t *msg;
  do {
    msg = get_bloccante(this->_buffer);
    this->_task(msg, NULL);
  } while(msg != POISON_PILL_MSG);
  free(this);
  pthread_exit(0);
}

static int subscribe(struct Reader *this, Dispatcher *dispatcher) {
  this->_buffer = dispatcher->subscribe(dispatcher);
  pthread_t thread;
  pthread_create(&thread, NULL, this->_subscribeTask, this);
  pthread_detach(thread);
  return 1;
}

Reader *_new_Reader(void *task(msg_t *msg, void* args)) {
  Reader *this = malloc(sizeof(Reader));
  this->_task = task;
  this->_subscribeTask = _subscribeTask;
  this->subscribe = subscribe;
  return this;
}
