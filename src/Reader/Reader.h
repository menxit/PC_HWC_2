#ifndef HWC2_READER_H
#define HWC2_READER_H

#include "../Dispatcher/Dispatcher.h"
typedef struct Reader {

  /**
   * @Private
   * Indica se il reader si è registrato
   */
  unsigned short _isSubscribed;

  /**
   * @Private
   * Numero di messaggi consumati.
   */
  unsigned long _numberOfConsumedMessages;

  /**
   * @Private
   * Buffer dal quale il Reader estrae i messaggi pervenuti dal Dispatcher.
   */
  buffer_t *_buffer;

  /**
   * @Private
   * Task generico che il reader esegue quando riceve un nuovo messaggio dal Dispatcher.
   *
   * @param msg
   * @param args
   * @return
   */
  void *(*_task)(msg_t* msg, void* args);

  /**
   * @Private
   * Metodo che viene eseguito in un thread
   *
   * @param this
   * @return
   */
  void *(*_subscribeTask)(void *this);

    /**
	 * @Public
	 * Permette a un Reader di registrarsi a un Dispatcher. Questa è una chiamata bloccante e restituisce
	 * true se va a buon fine o false altrimenti. Il controllo viene restituito appena il reader è riuscito a iscriversi.
	 *
	 * @param dispatcher
	 * @param task
	 * @return
	 */
  int (*subscribe)(struct Reader*, Dispatcher *dispatcher);

  /**
   * @Public
   * Restituisce il numero di messaggi attualmente consumati dal reader.
   *
   * @return
   */
  unsigned long (*getNumberOfMessagesConsumed)(struct Reader*);

  /**
   * @Public
   * Restituisce 1 se il reader è registrato al dispatcher 0 altrimenti.
   *
   * @return
   */
  unsigned short (*getIsSubscribed)(struct Reader*);

} Reader;

Reader *_new_Reader(void *task(msg_t* msg, void* args));


#endif
