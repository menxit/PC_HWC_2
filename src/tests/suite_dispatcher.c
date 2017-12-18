#include <CUnit/CUnit.h>
#include "../Reader/Reader.h"
#include "../Factory/Factory.h"

void aSubscribedReaderShouldReceiveMessages(void) {
  // Numero di messaggi che il Provider invia al Dispatcher
  int N = 100;

  // Creo un dispatcher che riceverà 10 messaggi dal Provider
  Dispatcher *dispatcher = factory_createDispatcherThatWillReceiveNMessagesFromProvider(N);

  // Creo un reader che consuma i messaggi
  Reader *reader = factory_createReader();
  reader->subscribe(reader, dispatcher);

  // Avvio l'invio dei messaggi da parte del dispatcher ai reader registrati
  dispatcher->start(dispatcher);

  // Attendo che tutte le poison pill siano state inviate sul buffer dei reader
  dispatcher->wait(dispatcher);

  // Faccio una sleep per essere certo che ciascun reader consumato
  // tutti i messaggi presenti nel proprio buffer
  usleep(100);

  // Verifico che il reader abbia effettivamente ricevuto e consumato 10 messaggi
  unsigned long numberOfMessagesReceived = reader->getNumberOfMessagesConsumed(reader);

  CU_ASSERT_EQUAL(numberOfMessagesReceived, N);
}

void twoSubscribedReaderShouldReceiveMessages(void) {
  // Numero di messaggi che il Provider invia al Dispatcher
  int N = 100;

  // Creo un dispatcher che riceverà 10 messaggi dal Provider
  Dispatcher *dispatcher = factory_createDispatcherThatWillReceiveNMessagesFromProvider(N);

  // Creo i due reader che consumano i messaggi
  Reader *reader1 = factory_createReader();
  reader1->subscribe(reader1, dispatcher);
  Reader *reader2 = factory_createReader();
  reader2->subscribe(reader2, dispatcher);

  // Avvio l'invio dei messaggi da parte del dispatcher ai reader registrati
  dispatcher->start(dispatcher);

  // Attendo che tutte le poison pill siano state inviate sul buffer dei reader
  dispatcher->wait(dispatcher);

  // Faccio una sleep per essere certo che ciascun reader abbia consumato
  // tutti i messaggi presenti nel proprio buffer
  usleep(100);

  // Verifico che il reader abbia effettivamente ricevuto e consumato 10 messaggi
  unsigned long numberOfMessagesReceived1 = reader1->getNumberOfMessagesConsumed(reader1);
  unsigned long numberOfMessagesReceived2 = reader2->getNumberOfMessagesConsumed(reader2);

  CU_ASSERT_EQUAL(numberOfMessagesReceived1, N);
  CU_ASSERT_EQUAL(numberOfMessagesReceived2, N);
}

void aSlowReaderShouldBeDeleted(void) {

  int N = 100;

  // Creo un dispatcher che riceverà 100 messaggi dal Provider
  Dispatcher *dispatcher = factory_createDispatcherThatWillReceiveNMessagesFromProvider(N);

  // Creo un reader lento
  Reader *slowReader = factory_createSlowReader();
  slowReader->subscribe(slowReader, dispatcher);

  // Creo un reader normale
  Reader *reader = factory_createReader();
  reader->subscribe(reader, dispatcher);

  // Avvio l'invio dei messaggi da parte del dispatcher ai reader registrati
  dispatcher->start(dispatcher);

  // Attendo che tutte le poison pill siano state inviate sul buffer dei reader
  dispatcher->wait(dispatcher);

  // Faccio una sleep per essere certo che ciascun reader abbia consumato
  // tutti i messaggi presenti nel proprio buffer
  usleep(100);

  // Mi aspetto che il reader lento non abbia ricevuto tutti i messaggi ma che sia
  // stato ucciso prima dal dispatcher.
  CU_ASSERT_NOT_EQUAL(slowReader->getNumberOfMessagesConsumed(slowReader), N);
  CU_ASSERT_EQUAL(reader->getNumberOfMessagesConsumed(reader), N);
}

void onDispatcherWaitTheListOfBufferProviderShouldBeEmpty(void) {

  // Creo un dispatcher che riceverà 100 messaggi dal Provider
  Dispatcher *dispatcher = factory_createDispatcherThatWillReceiveNMessagesFromProvider(100);

  // Creo qualche reader
  int i;
  for(i = 0; i < 100; i++) {
    Reader *reader = factory_createReader();
    reader->subscribe(reader, dispatcher);
  }

  // Avvio l'invio dei messaggi da parte del dispatcher ai reader registrati
  dispatcher->start(dispatcher);

  // Attendo che tutte le poison pill siano state inviate sul buffer dei reader
  dispatcher->wait(dispatcher);

  // Mi aspetto che la lista dei buffer reader sia stata svuotata
  CU_ASSERT_EQUAL(size(dispatcher->_listOfBufferReader), 0);

}

void onDispatcherWaitTheListOfReaderShouldReceiveAPoisonPill(void) {

  // Creo un dispatcher che riceverà 100 messaggi dal Provider
  Dispatcher *dispatcher = factory_createDispatcherThatWillReceiveNMessagesFromProvider(100);

  // Creo qualche reader
  int i;
  Reader *readers[50];
  for(i = 0; i < 50; i++) {
    readers[i] = factory_createSlowReader();
    readers[i]->subscribe(readers[i], dispatcher);
  }

  // Avvio l'invio dei messaggi da parte del dispatcher ai reader registrati
  dispatcher->start(dispatcher);

  // Attendo che tutte le poison pill siano state inviate sul buffer dei reader
  dispatcher->wait(dispatcher);

  // Faccio questa attesa per essere certo che tutti i readers abbiano estratto dal buffer
  // la poison pill e che si siano uccisi
  usleep(100);

  for(i = 0; i < 50; i++)
    CU_ASSERT_EQUAL(readers[i]->_isSubscribed, 0);

}
