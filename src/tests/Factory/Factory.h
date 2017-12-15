#ifndef HWC2_FACTORY_H
#define HWC2_FACTORY_H

#include "../../Reader/Reader.h"
#include "../../Provider/Provider.h"
#include "../mocks/DispatcherMock/DispatcherMock.h"

msg_t *factory_createExpectedMessage();
buffer_t *factory_createEmptyBuffer(unsigned int sizeBuffer);
buffer_t *factory_createFullBuffer(unsigned int sizeBuffer);
buffer_t *factory_createHalfFullBuffer(unsigned sizeBuffer);
list_t *factory_createListOfMessages();
Provider *factory_createProviderWithDispatcherMock();
Reader *factory_createReaderWithDispatcherMock();
DispatcherMock *factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill();
DispatcherMock *factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesWithoutPoisonPill();

#endif
