#include <msg_t/msg_t.h>
#include <stdlib.h>
#include <buffer_t/buffer_t.h>
#include <list.h>
#include <printf.h>
#include "../../Provider/Provider.h"
#include "../mocks/DispatcherMock/DispatcherMock.h"
#include "../../Reader/Reader.h"
#include "../../poison_pill/poison_pill.h"
#include "../../Accepter/Accepter.h"
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

DispatcherMock *factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill() {
  buffer_t *buffer = factory_createHalfFullBuffer(20);
  put_bloccante(buffer, POISON_PILL_MSG);
  return _new_DispatcherMock(1, 100, buffer, 100);
}

DispatcherMock *factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesWithoutPoisonPill() {
  buffer_t *buffer = factory_createHalfFullBuffer(20);
  return _new_DispatcherMock(1, 100, buffer, 100);
}

Provider *factory_createProviderWithDispatcherMock() {
  return _new_Provider(factory_createListOfMessages(),
                       (Dispatcher *) factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill());
}

void *task(msg_t *msg, void *args) {
  //printf("%d\n", *(int*)msg->content);
  return NULL;
}

Reader *factory_createReader() {
  return _new_Reader(task);
}

Accepter *factory_createAccepterWithEmptyAccepterBuffer() {
  DispatcherMock *dispatcherMock = factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill();
  return _new_Accepter(dispatcherMock);
}

Accepter *factory_createAccepterWithAFullAccepterBuffer() {
  DispatcherMock *dispatcherMock = factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill();
  Accepter *result = _new_Accepter(dispatcherMock);
  buffer_t *fullBuffer = factory_createFullBuffer(10);
  result->_bufferAccepter = fullBuffer;
  return result;
}
