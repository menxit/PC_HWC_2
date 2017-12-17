#ifndef HWC2_FACTORY_H
#define HWC2_FACTORY_H

#include "../Reader/Reader.h"
#include "../Provider/Provider.h"
#include "../Accepter/Accepter.h"
#include "../mocks/DispatcherMock/DispatcherMock.h"

msg_t *factory_createExpectedMessage();
buffer_t *factory_createEmptyBuffer(unsigned int sizeBuffer);
buffer_t *factory_createFullBuffer(unsigned int sizeBuffer);
buffer_t *factory_createHalfFullBuffer(unsigned sizeBuffer);
list_t *factory_createListOfMessages(int N);
Provider *factory_createProviderWithDispatcherMock();
Reader *factory_createReader();
Reader *factory_createReaderSleep1();
Reader *factory_createReaderSleep5();
Reader *factory_createSlowReader();
DispatcherMock *factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill();
DispatcherMock *factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesWithoutPoisonPill();
Accepter *factory_createAccepterWithEmptyAccepterBuffer();
Accepter *factory_createAccepterWithAFullAccepterBuffer();
Dispatcher *factory_createDispatcherThatWillReceiveNMessagesFromProvider(int N);

#endif
