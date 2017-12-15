#ifndef HWC2_READERMOCK_H
#define HWC2_READERMOCK_H

#include <buffer_t/buffer_t.h>
#include "../../../Dispatcher/Dispatcher.h"
typedef struct ReaderMock {

  unsigned short _isSubscribed;
  unsigned long _numberOfConsumedMessages;
  buffer_t *_buffer;
  void *(*_task)(msg_t* msg, void* args);
  void *(*_subscribeTask)(void *this);
  int (*subscribe)(struct Reader*, Dispatcher *dispatcher);
  unsigned long (*getNumberOfMessagesConsumed)(struct Reader*);
  unsigned short (*getIsSubscribed)(struct Reader*);



} ReaderMock;

ReaderMock *_new_ReaderMock(

);


#endif
