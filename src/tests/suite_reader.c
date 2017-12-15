#include <CUnit/CUnit.h>
#include "Factory/Factory.h"

void test_readerSubscribedShouldConsumeMessages(void) {
  Reader *reader = factory_createReaderWithDispatcherMock();
  DispatcherMock *dispatcher = factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill();
  reader->subscribe(reader, (Dispatcher*)dispatcher);
  usleep(100);
  CU_ASSERT_EQUAL(reader->getNumberOfMessagesConsumed(reader), 10);
}

void test_readerSubscribedThatReceiveAPoisonPillShouldStop(void) {
  Reader *reader = factory_createReaderWithDispatcherMock();
  DispatcherMock *dispatcher = factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesAndPoisonPill();
  reader->subscribe(reader, (Dispatcher*)dispatcher);
  usleep(100);
  CU_ASSERT_FALSE(reader->getIsSubscribed(reader));
}

void test_readerSubscribedThatDoesNotReceiveAPoisonPillShouldNotStop(void) {
  Reader *reader = factory_createReaderWithDispatcherMock();
  DispatcherMock *dispatcher = factory_createDispatcherMockThatOnSubscribeReturnABufferWithTenMessagesWithoutPoisonPill();
  reader->subscribe(reader, (Dispatcher*)dispatcher);
  usleep(100);
  CU_ASSERT_TRUE(reader->getIsSubscribed(reader));
}
