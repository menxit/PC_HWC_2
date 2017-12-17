#include <stdlib.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "tests/suite_poison_pill.h"
#include "tests/suite_provider.h"
#include "tests/suite_reader.h"
#include "tests/suite_accepter.h"
#include "tests/suite_dispatcher.h"

int main(int argc, char **argv) {

  //PER PRIMA COSA inizializzare cunit
  CU_ErrorCode ret = CU_initialize_registry();
  if (ret == CUE_SUCCESS)
    printf("Inizio dei test\n");
  if (ret == CUE_NOMEMORY) {
    printf("Errore nei test\n");
    exit(-1);
  }

  CU_TestInfo suitePoisonPill[] = {
    {
      "✓ POISON_PILL_MSG should be a singleton",
      poisonPillMsgShouldBeASingleton
    },
    {
      "✓ isPoisonPillMsg should return true if the message is a poison pill",
      isPoisonPillMsgShouldReturnTrueIfItIsAPoisonPill
    },
    {
      "✓ isPoisonPillMsg should return false if the message is not a poison pill",
      isPoisonPillMsgShouldReturnFalseIfItIsNotAPoisonPill
    },
    CU_TEST_INFO_NULL,
  };

  CU_TestInfo suiteProvider[] = {
    {
      "✓ Start Provider when it is not running should not fail",
      test_startProviderWhenItIsNotRunningShouldNotFail
    },
    {
      "✓ Start Provider when it is running should fail",
      test_startProviderWhenItIsRunningShouldFail
    },
    {
      "✓ Start Provider after wait should work",
      test_startProviderAfterWaitShouldWork
    },
    CU_TEST_INFO_NULL,
  };

  CU_TestInfo suiteReader[] = {
    {
      "✓ Reader subscribed should consume messages",
      test_readerSubscribedShouldConsumeMessages,
    },
    {
      "✓ Reader subscribed that receive a poison pill should stop",
      test_readerSubscribedThatReceiveAPoisonPillShouldStop,
    },
    {
      "✓  Reader subscribed that does not receive a poison pill should not stop",
      test_readerSubscribedThatDoesNotReceiveAPoisonPillShouldNotStop,
    },
    CU_TEST_INFO_NULL,
  };

  CU_TestInfo suiteAccepter[] = {
    {
      "✓ When a request is sent to an accepter with an empty buffer, it should return a message with the reader as content",
      sendReaderWithAnEmptyBufferAccepterShouldReturnAMessageWithTheReaderAsContent
    },
    {
      "✓ isRunning should return true if a poison pill is not sent",
      isRunningShouldReturnTrueIfAPoisonPillWasNotSent
    },
    {
      "✓ isRunning should return false if a poison pill is received",
      isRunningShouldReturnFalseIfAPoisonPillIsReceived
    },
    CU_TEST_INFO_NULL,
  };

  CU_TestInfo suiteDispatcher[] = {
    {
      "✓ A subscribed reader should receive all the messages",
      aSubscribedReaderShouldReceiveMessages
    },
    {
      "✓ Two subscribed reader should receive all the messages",
      twoSubscribedReaderShouldReceiveMessages
    },
    {
      "✓ A slow reader should be deleted",
      aSlowReaderShouldBeDeleted
    },
    CU_TEST_INFO_NULL,
  };

  CU_SuiteInfo suites[] = {
    {.pName = "Suite Poison Pill", .pTests = suitePoisonPill},
    {.pName = "Suite Provider", .pTests = suiteProvider},
    {.pName = "Suite Reader", .pTests = suiteReader},
    {.pName = "Suite Accepter", .pTests = suiteAccepter},
    {.pName = "Suite Dispatcher", .pTests = suiteDispatcher},
    CU_SUITE_INFO_NULL
  };

  CU_ErrorCode error = CU_register_suites(suites);
  if (error != CUE_SUCCESS) {
    printf("Errore con CUnit - codice %d\n", error);
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_ErrorCode tuttoOk = CU_basic_run_tests();
  printf("CUnit error code: %d\n", tuttoOk);

  //lettura risultati
  unsigned int failures = CU_get_number_of_failures();
  if (failures == 0)
    printf("Barra Verde!\n");
  else
    printf("%d fallimenti\n", CU_get_number_of_failures());

  //alla fine, pulire. UNA SOLA volta
  CU_cleanup_registry();

  return 0;
}
