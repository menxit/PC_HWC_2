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
   * Lista di buffer dai quali i reader possono estrarre i messaggi.
   */
  list_t *_listOfBufferReader;

  /**
   * @Public
   * Aggiunge un messaggio in _bufferProvider.
   * Questa chiamata è bloccante.
   *
   * @param message
   * @return
   */
  int (*addMessageOnProviderBuffer)(struct Dispatcher*, msg_t *message);

  /**
   * @Public
   * Un reader può registrarsi a un Dispatcher. In cambio ottiene un buffer_t*
   * dal quale può estrarre messaggi.
   *
   * @return
   */
  buffer_t *(*subscribe)(struct Dispatcher*);

} Dispatcher;

Dispatcher *_new_Dispatcher();


#endif
