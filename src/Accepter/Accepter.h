#ifndef HWC2_ACCEPTER_H
#define HWC2_ACCEPTER_H

#include <msg_t/msg_t.h>
#include <buffer_t/buffer_t.h>
#include <list.h>
#include "../Reader/Reader.h"
#define ACCEPTER_SEND_REQUEST_ERROR NULL;

typedef struct Accepter {

  /**
   * @Private
   * Contiene tutte le richieste di nuovi reader che devono ancora essere
   * inviate al Dispatcher.
   */
  buffer_t *_bufferAccepter;

  /**
   * @Private
   * Elenco dei reader registrati al Dispatcher.
   */
  list_t *_listOfSubscribedReaders;

  /**
   * @Private
   * Dispatcher al quale l'Accepter invia le richieste
   */
  Dispatcher *_dispatcher;

  /**
   * @Private
   * Numero di richieste inviate al Dispatcher
   */
  unsigned long _requestSentToDispatcher;

  /**
   * @Private
   * Questa proprietà vale 1 se l'Accepter sta inviando richieste al Dispatcher 0 altrimenti.
   */
  unsigned short _isRunning;

  /**
   * @Private
   * Identificativo del flusso di esecuzione di _startTask
   */
  pthread_t *_startTaskID;

  /**
   * @Private
   * Metodo che deve eseguire il flusso di esecuzione avviato allo start dell'Accepter.
   *
   * @param args
   * @return
   */
  void* (*_startTask)(void* args);

  /**
	 * @Public
	 * Si restituisce il controllo quando la richiesta viene presa in carico
	 * dall'Accepter, non dal Dispatcher.
	 *
	 * @return
	 */
  msg_t* (*sendRequest)(struct Accepter*, msg_t *msg);

  /**
  * @Public
  * Si restituisce il controllo quando la richiesta viene presa in carico
  * dall'Accepter, non dal Dispatcher.
  *
  * @return
  */
  msg_t* (*sendReader)(struct Accepter*, Reader *reader);

  /**
   * @Public
   * Restituisce la lista dei reader attualmente registrati al Dispatcher.
   */
  list_t *(*getSubscribedReaders)(struct Accepter*);

  /**
   * @Public
   * The Accepter start to send requests to Dispatcher
   *
   * @return
   */
  void (*start)(struct Accepter*);

  /**
  * @Public
  * The Accepter start to send requests to Dispatcher
  *
  * @return
  */
  void (*wait)(struct Accepter*);

  /**
   * @Public
   * Un flusso di esecuzione esterno può inviare una Poison Pill all'Accepter
   * mediante questo metodo.
   *
   * @return
   */
  msg_t *(*sendPoisonPill)(struct Accepter*);

  /**
   * @Public
   * Restituisce 1 se l'Accepter sta inviando richieste al Dispatcher 0 altrimenti
   *
   * @return
   */
  unsigned short (*getIsRunning)(struct Accepter*);

} Accepter;

Accepter *_new_Accepter(Dispatcher *dispatcher);


#endif
