#include <CUnit/CUnit.h>
#include "../Reader/Reader.h"
#include "../Factory/Factory.h"
#include "../poison_pill/poison_pill.h"

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

  int i;

  // Creo un dispatcher che riceverà 100 messaggi dal Provider
  Dispatcher *dispatcher = factory_createDispatcherThatWillReceiveNMessagesFromProvider(4);

  // Creo qualche reader
  for(i = 0; i < 50; i++) {
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
    readers[i] = factory_createReader();
    readers[i]->subscribe(readers[i], dispatcher);
  }

  // Avvio l'invio dei messaggi da parte del dispatcher ai reader registrati
  dispatcher->start(dispatcher);

  // Attendo che tutte le poison pill siano state inviate sul buffer dei reader
  dispatcher->wait(dispatcher);

  // Faccio questa attesa per essere certo che tutti i readers abbiano estratto dal buffer
  // la poison pill e che si siano uccisi
  usleep(100);

  int numberOfReadersSubscribed = 0;
  for(i = 0; i < 10; i++)
    numberOfReadersSubscribed += readers[i]->_isSubscribed;

  CU_ASSERT_EQUAL(numberOfReadersSubscribed, 0);

}

void dispatcherShouldSendOnlyNewMessagesToReaders(void) {

  int i;

  // Creo un dispatcher
  Dispatcher *dispatcher = _new_Dispatcher();

  // Creo reader1 e reader2
  Reader *reader1 = factory_createReader(), *reader2 = factory_createReader();

  // Registro reader1
  reader1->subscribe(reader1, dispatcher);

  // Avvio l'invio dei messaggi da parte del dispatcher ai reader registrati (per adesso uno solo)
  dispatcher->start(dispatcher);

  // Invio dei messaggi al dispatcher, messaggi che dovrebbe ricevere solo
  // il primo reader, ovvero l'unico che per ora registrato
  for(i = 0; i < 3; i++)
    dispatcher->addMessageOnProviderBuffer(dispatcher, factory_createExpectedMessage());

  // Permetto al dispatcher di consegnare qualche messaggio proveniente dal provider
  usleep(10000);

  // Registro reader2, da adesso in poi anche lui dovrebbe ricevere i messaggi
  reader2->subscribe(reader2, dispatcher);

  // Inoltro al dispatcher qualche altro messaggio, prima che il provider consegni la poison pill
  for(i = 0; i < 3; i++)
    dispatcher->addMessageOnProviderBuffer(dispatcher, factory_createExpectedMessage());

  // Consegno la poison pill
  dispatcher->addMessageOnProviderBuffer(dispatcher, POISON_PILL_MSG);

  // Attendo che tutte le poison pill siano state inviate sul buffer dei reader
  dispatcher->wait(dispatcher);

  // Faccio una sleep per essere certo che tutti i reader
  // abbiano finito di consumare i messaggi già presenti nel buffer
  usleep(1000);

  CU_ASSERT_EQUAL(reader1->getNumberOfMessagesConsumed(reader1), 6);
  CU_ASSERT_EQUAL(reader2->getNumberOfMessagesConsumed(reader2), 3);
}
