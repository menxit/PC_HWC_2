#include <CUnit/CUnit.h>
#include "../Accepter/Accepter.h"
#include "../Factory/Factory.h"

void sendReaderWithAnEmptyBufferAccepterShouldReturnAMessageWithTheReaderAsContent(void) {
  Accepter *accepter = factory_createAccepterWithEmptyAccepterBuffer();
  Reader *reader = factory_createReader();
  msg_t *result = accepter->sendReader(accepter, reader);
  CU_ASSERT_EQUAL(result->content, reader);
}

void isRunningShouldReturnTrueIfAPoisonPillWasNotSent(void) {
  Accepter *accepter = factory_createAccepterWithEmptyAccepterBuffer();
  accepter->start(accepter);
  usleep(500);
  CU_ASSERT_TRUE(accepter->getIsRunning(accepter));
}

void isRunningShouldReturnFalseIfAPoisonPillIsReceived(void) {
  Accepter *accepter = factory_createAccepterWithEmptyAccepterBuffer();
  accepter->start(accepter);
  accepter->sendPoisonPill(accepter);
  usleep(500);
  CU_ASSERT_FALSE(accepter->getIsRunning(accepter));
}
