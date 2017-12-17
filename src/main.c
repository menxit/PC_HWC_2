#include <printf.h>
#include "Provider/Provider.h"
#include "Factory/Factory.h"

void *sendRequestReaders(void *args) {
  Accepter *accepter = (Accepter*)args;
  int i = 0;
  for(i = 0; i < 7; i++) {
    Reader *reader;
    if(i%3 == 0) {
      reader = factory_createReaderSleep5();
    } else {
      reader = factory_createReaderSleep1();
    }
    accepter->sendReader(accepter, reader);
    sleep(3);
    //printf("Richiesta creazione reader accettata dall'accepter.\n");
  }
  pthread_exit(0);
}

int main() {

  /**
   * Creo un Dispatcher
   */
  Dispatcher *dispatcher = _new_Dispatcher();

  /**
   * Creo un Provider con una lista di messaggi da inviare al dispatcher e avvio
   * il provider. Avviare il provider significa che inizia ad inviare i messaggi
   * uno ad uno al Dispatcher mediante delle put_bloccanti.
   */
  list_t *listOfMessagesProvider = factory_createListOfMessages(10);
  Provider *provider = _new_Provider(listOfMessagesProvider, dispatcher);

  /**
   * Creo un Accepter e lo avvio. Avviare un accepter significa le richieste di
   * creazione di Reader vengono inoltrate al Dispatcher mediante l'uso di put_bloccanti
   *
   */
  Accepter *accepter = _new_Accepter(dispatcher);
  accepter->start(accepter);

  /**
   * Vengono creati 100 Reader e inoltrati all'accepter mediante sendReader che è un metodo
   * bloccante. In particolare sendReader fa una put_bloccante sul buffer accepter.
   */
  pthread_t threadSendRequestReader;
  pthread_create(&threadSendRequestReader, NULL, sendRequestReaders, accepter);

  /**
  * Il Dispatcher inizia a inoltrare i messaggi ai vari Reader.
  */
  dispatcher->start(dispatcher);

  sleep(6);
  provider->start(provider);

  /**
   * Si attende che il Provider abbia terminato la sua esecuzione. Terminare la sua esecuzione
   * significa aver inoltrato al Dispatcher la poison pill.
   */
  provider->wait(provider);

  /**
   * Il flusso main non appena provider decide di terminare invia una poison pill all'accepter.
   */
  accepter->sendPoisonPill(accepter);

  /**
   * Attendo che l'accepter abbia ricevuto la poison pill.
   */
  accepter->wait(accepter);

  /**
   * Ottengo un iteratore dela lista di reader ancora presenti.
   */
  list_t *listSubscribedReaders = accepter->getSubscribedReaders(accepter);
  iterator_t *subscribedReaders = iterator_init(listSubscribedReaders);

  /**
   * Il main invia una poison pill ai reader rimasti.
   */
  while(hasNext(subscribedReaders)) {
    Reader *reader = next(subscribedReaders);
    reader->sendPoisonPill(reader);
  }

  pthread_join(threadSendRequestReader, NULL);

  return 0;
}
