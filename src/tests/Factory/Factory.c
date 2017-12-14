
#include <msg_t/msg_t.h>
#include <stdlib.h>
#include <buffer_t/buffer_t.h>
#include <list.h>
#include "../../Provider/Provider.h"
#include "../mocks/DispatcherMock/DispatcherMock.h"
#define EXPECTED_MSG 1010

/**
 * Crea e restituisce un messaggio
 *
 * @return
 */
msg_t *factory_createExpectedMessage() {
  int *content = malloc(sizeof(int));
  *content = EXPECTED_MSG;
  msg_t *msg = msg_init(content);
  return msg;
}

/**
 * Crea un buffer di dimensione sizeBuffer vuoto
 *
 * @param sizeBuffer
 * @return
 */
buffer_t *factory_createEmptyBuffer(unsigned int sizeBuffer) {
  buffer_t *b = buffer_init(sizeBuffer);
  return b;
}

/**
 * Crea un buffer di dimensione sizeBuffer pieno
 *
 * @param sizeBuffer
 * @return
 */
buffer_t *factory_createFullBuffer(unsigned int sizeBuffer) {
  buffer_t *b = buffer_init(sizeBuffer);
  int i = 0;
  while (i < sizeBuffer) {
    msg_t *msg = factory_createExpectedMessage();
    put_bloccante(b, msg);
    i++;
  }
  return b;
}

/**
 * Crea un buffer di dimensione sizeBuffer mezzo vuoto
 *
 * @param sizeBuffer
 * @return
 */
buffer_t *factory_createHalfFullBuffer(unsigned int sizeBuffer) {
  buffer_t *b = buffer_init(sizeBuffer);
  int i = 0;
  while (i < (sizeBuffer / 2)) {
    msg_t *msg = factory_createExpectedMessage();
    put_bloccante(b, msg);
    i++;
  }
  return b;
}

list_t *factory_createListOfMessages() {
  list_t *list = list_init();
  addElement(list, factory_createExpectedMessage());
  addElement(list, factory_createExpectedMessage());
  addElement(list, factory_createExpectedMessage());
  addElement(list, factory_createExpectedMessage());
  addElement(list, factory_createExpectedMessage());
  return list;
}

Provider *factory_createProviderWithDispatcherMock() {
  DispatcherMock *dispatcherMock = _new_DispatcherMock(1, 100, factory_createEmptyBuffer(10), 0);
  return _new_Provider(factory_createListOfMessages(), (Dispatcher*)dispatcherMock);
}
