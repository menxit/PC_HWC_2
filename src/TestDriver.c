#include <stdlib.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "tests/all_tests.h"

int main(int argc, char **argv) {

  //PER PRIMA COSA inizializzare cunit
  CU_ErrorCode ret = CU_initialize_registry();
  if (ret == CUE_SUCCESS)
	printf("inizio dei test\n");
  if (ret == CUE_NOMEMORY) {
	printf("ERRORE NEI TEST\n");
	exit(-1);
  }

  CU_TestInfo suiteListe[] = {
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
  };

  CU_SuiteInfo suites[] = {
	  {.pName = "Tests", .pTests = suiteListe},
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
