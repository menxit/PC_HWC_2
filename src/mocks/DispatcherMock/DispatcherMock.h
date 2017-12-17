#ifndef HWC2_DISPATCHERMOCK_H
#define HWC2_DISPATCHERMOCK_H

#include <buffer_t/buffer_t.h>
#include "../../Dispatcher/Dispatcher.h"
typedef struct DispatcherMock {

  buffer_t *_bufferProvider;
  list_t *_listOfBufferReader;
  pc_sem_t *_useListOfBufferReader;
  pthread_t *_startTaskID;
  int (*_broadcastPoisonPill)(struct Dispatcher *);
  int (*_broadcastMessage)(struct Dispatcher* this, msg_t* message);
  int (*addMessageOnProviderBuffer)(struct Dispatcher*, msg_t *message);
  buffer_t *(*subscribe)(struct Dispatcher*);
  void (*start)(struct Dispatcher* this);

  /**
   * @Private
   * Il valore che il Dispatcher deve restituire quando viene richiamato
   * il metodo addMessageOnProviderBuffer.
   */
  int _addMessageOnProviderBufferReturnValue;

  /**
   * @Private
   * Tempo necessario per eseguire il metodo pubblico addMessageOnProviderBuffer in usecond.
   */
  int _addMessageOnProviderBufferSleep;

  /**
   * @Private
   * Il valore che il Dispatcher deve restituire quando viene richiamato
   * il metodo subscribe.
   */
  buffer_t *_subscribeReturnValue;

  /**
   * @Private
   * Tempo necessario per eseguire il metodo pubblico subscribe.
   */
  int _subscribeSleep;

} DispatcherMock;

DispatcherMock *_new_DispatcherMock(
  int addMessageOnProviderBufferReturnValue,
  int addMessageOnProviderBufferSleep,
  buffer_t *subscribeReturnValue,
  int subscribeSleep
);

#endif
