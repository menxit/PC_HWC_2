#include <msg_t/msg_t.h>
#include <stdlib.h>
#include <buffer_t/buffer_t.h>
#include <list.h>
#include "../Provider/Provider.h"
#include "../Reader/Reader.h"
#include "../poison_pill/poison_pill.h"
#include "../Accepter/Accepter.h"
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

list_t *factory_createListOfMessages(int N) {
  list_t *list = list_init();
  int i;
  for(i = 0; i < N; i++) {
    addElement(list, factory_createExpectedMessage());
  }
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
  return _new_Provider(factory_createListOfMessages(10),
                       (Dispatcher *) factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill());
}

void *task(msg_t *msg, void *args) {
  return NULL;
}

void *taskSleep1(msg_t *msg, void *args) {
  sleep(1);
  return NULL;
}

void *taskSleep5(msg_t *msg, void *args) {
  sleep(5);
  return NULL;
}

Reader *factory_createReader() {
  return _new_Reader(task);
}

Reader *factory_createReaderSleep1() {
  return _new_Reader(taskSleep1);
}

Reader *factory_createReaderSleep5() {
  return _new_Reader(taskSleep5);
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

/******/

void *taskSlowReader(msg_t *msg, void *args) {
  usleep(500);
  return NULL;
}

Reader *factory_createSlowReader() {
  return _new_Reader(taskSlowReader);
}

Dispatcher *factory_createDispatcherThatWillReceiveNMessagesFromProvider(int N) {

  // Creo un Dispatcher
  Dispatcher * dispatcher = _new_Dispatcher();

  // Creo un Provider con 100 messaggi da inviare al Dispatcher
  Provider* provider = _new_Provider(factory_createListOfMessages(N), dispatcher);

  // Provider inizia a inviare i messaggi al dispatcher
  provider->start(provider);

  return dispatcher;
}
