#include <stdlib.h>
#include "DispatcherMock.h"

static int _addMessageOnProviderBuffer(DispatcherMock *this, msg_t *message) {
  usleep((useconds_t) this->_addMessageOnProviderBufferSleep);
  return this->_addMessageOnProviderBufferReturnValue;
}

static buffer_t *_subscribe(DispatcherMock this) {
  usleep((useconds_t) this._subscribeSleep);
  return this._subscribeReturnValue;
}

DispatcherMock *_new_DispatcherMock(int addMessageOnProviderBufferReturnValue,
                                    int addMessageOnProviderBufferSleep,
                                    buffer_t *subscribeReturnValue,
                                    int subscribeSleep) {
  DispatcherMock *this = malloc(sizeof(DispatcherMock));
  this->_addMessageOnProviderBufferReturnValue = addMessageOnProviderBufferReturnValue;
  this->_addMessageOnProviderBufferSleep = addMessageOnProviderBufferSleep;
  this->_subscribeReturnValue = subscribeReturnValue;
  this->_subscribeSleep = subscribeSleep;
  this->addMessageOnProviderBuffer = _addMessageOnProviderBuffer;
  this->subscribe = _subscribe;
  return this;
}
