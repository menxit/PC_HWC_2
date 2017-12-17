#ifndef HWC2_DISPATCHER_H
#define HWC2_DISPATCHER_H

#include <buffer_t/buffer_t.h>
#include <list.h>

typedef struct Dispatcher {

  /**
   * @Private
   * Buffer contenente i messaggi che il Dispatcher deve ancora smistare ai suoi reader.
   */
  buffer_t *_bufferProvider;

  /**
   * @Private
   * Lista di buffer dai quali i reader possono estrarre i messaggi. Ciascun nodo della lista
   * contiene un buffer associato a un determinato Reader.
   */
  list_t *_listOfBufferReader;

  /**
   * @Private
   * Semaforo necessario a gestire l'uso di _listOfBufferReader
   */
  pc_sem_t *_useListOfBufferReader;

  /**
   * @Private
   * ID del flusso di esecuzione associato al dispatcher.
   */
  pthread_t *_startTaskID;

  /**
   * @Private
   * Genera un flusso di esecuzione per ciascun reader che fa una put_bloccante
   * di una poison pill. Il controllo viene restituito quando tutti i reader hanno
   * ricevuto la poison pill.
   *
   * @return
   */
  int (*_broadcastPoisonPill)(struct Dispatcher *);

  /**
   * @Private
   * Questo metodo si occupa di inviare il messaggio corrente a ciascun reader presente nella
   * lista mediante una put_non_bloccante. Se la put non bloccante fallisce, allora viene creato
   * un flusso di esecuzione che ha come unico compito quello di fare una put_bloccante di una poison
   * pill su quel reader.
   *
   * @param this
   * @param message
   */
  int (*_broadcastMessage)(struct Dispatcher *this, msg_t *message);

  /**
   * @Public
   * Aggiunge un messaggio in _bufferProvider.
   * Questa chiamata è bloccante.
   *
   * @param message
   * @return
   */
  int (*addMessageOnProviderBuffer)(struct Dispatcher *, msg_t *message);

  /**
   * @Public
   * Un reader può registrarsi a un Dispatcher. In cambio ottiene un buffer_t*
   * dal quale può estrarre messaggi.
   *
   * @return
   */
  buffer_t *(*subscribe)(struct Dispatcher *);

  /**
   * @Public
   * Richiamando questo metodo il Dispatcher inizia a inviare i messaggi che riceve dal Provider
   * ai reader registrati.
   *
   * @param this
   */
  void (*start)(struct Dispatcher *this);

  /**
   * @Public
   * Metodo che attende che il Dispatcher abbia terminato di inviare tutte le poison pill
   * ai vari Reader.
   *
   * @param this
   */
  void (*wait)(struct Dispatcher *this);

} Dispatcher;

Dispatcher *_new_Dispatcher();


#endif
