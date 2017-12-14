#ifndef HWC2_FACTORY_H
#define HWC2_FACTORY_H

#include <buffer_t/buffer_t.h>

msg_t *factory_createExpectedMessage();
buffer_t *factory_createEmptyBuffer(unsigned int sizeBuffer);
buffer_t *factory_createFullBuffer(unsigned int sizeBuffer);
buffer_t *factory_createHalfFullBuffer(unsigned sizeBuffer);
list_t *factory_createListOfMessages();
Provider *factory_createProviderWithDispatcherMock();

#endif
