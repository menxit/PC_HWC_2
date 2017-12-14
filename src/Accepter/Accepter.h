#ifndef HWC2_ACCEPTER_H
#define HWC2_ACCEPTER_H

#include <msg_t/msg_t.h>
#include <buffer_t/buffer_t.h>
#include <list.h>
#include "../Reader/Reader.h"
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
   * @Public
   * Si restituisce il controllo quando la richiesta viene presa in carico
   * dall'Accepter, non dal Dispatcher.
   *
   * @return
   */
  msg_t* (*sendRequest)(struct Accepter*, Reader *reader);

  /**
   * @Public
   * Restituisce la lista dei reader attualmente registrati al Dispatcher.
   */
  list_t *(*getSubscribedReaders)(struct Accepter*);

} Accepter;

Accepter *_new_Accepter(Dispatcher *dispatcher);


#endif
