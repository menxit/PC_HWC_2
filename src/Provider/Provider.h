#include <msg_t/msg_t.h>
#include <buffer_t/buffer_t.h>
#include <list.h>
#include "../Dispatcher/Dispatcher.h"
#ifndef HWC2_PROVIDER_H
#define HWC2_PROVIDER_H

typedef struct Provider {

  /**
   * @Private
   * Sleep tra l'invio di un messaggio all'altro
   */
  unsigned int _sleep;

  /**
   * @Private
   * Lista dei messaggi che devono essere inviati.
   */
  list_t *_messagesToSend;

  /**
   * @Private
   * Dispatcher al quale devono essere inviati i messaggi.
   */
  Dispatcher *_dispatcher;

  /**
   * Semaforo binario. Se è settato a 1 significa che il Provider non è stato
   * avviato, altrimenti significa che è stato avviato. Viene utilizzato un semaforo
   * per rendere i metodi pubblici start e wait thread-safe.
   */
  pc_sem_t *_providerIsNotRunning;

  /**
   * Identificatore del flusso di esecuzione associato all'esecuzione del Provider.
   */
  pthread_t _startTaskID;

  /**
   * @Private
   * Task che dovrà eseguire il flusso di esecuzione creato dal metodo pubblico start.
   * Questo flusso di esecuzione si occuperà di prendere ciascun messaggio da inviare
   * e aggiungerlo con una put bloccante nel provider_buffer del dispatcher. Una volta
   * aggiunti tutti i messaggi (in maniera ordinata) aggiungerà come ultimo messaggio
   * una POISON_PILL_MSG.
   */
  void *(*_startTask)(void*);

  /**
   * @Public
   * Si richiede la creazione di un flusso di esecuzione che ha il compito di inviare i messaggi
   * al Dispatcher. Se l'avvio del Provider è andata a buon fine si otterrà 1, altrimenti si otterrà
   * un PROVIDER_ERROR. Si ottiene un errore nel caso in cui sia stato eseguito start mentre Provider
   * è ancora in esecuzione.
   * @return
   */
  int (*start)(struct Provider*);

  /**
   * @Public
   * Attendi che il Provider abbia terminato la sua esecuzione, ovvero quando ha inviato
   * al Dispatcher la poison pill.
   */
  void (*wait)(struct Provider*);

} Provider;


Provider *_new_Provider(list_t *messagesToSend, Dispatcher *dispatcher, unsigned int sleep);

#endif
