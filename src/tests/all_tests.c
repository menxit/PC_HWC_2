#include <CUnit/CUnit.h>
#include <msg_t/msg_t.h>
#include "../poison_pill/poison_pill.h"

void poisonPillMsgShouldBeASingleton(void) {
  msg_t* m1 = POISON_PILL_MSG;
  msg_t* m2 = POISON_PILL_MSG;
  // devono restituire lo stesso indirizzo di memoria
  CU_ASSERT_EQUAL(m1, m2);
}

void isPoisonPillMsgShouldReturnTrueIfItIsAPoisonPill(void) {
  msg_t* m1 = POISON_PILL_MSG;
  CU_ASSERT_TRUE(isPoisonPillMsg(m1));
}

void isPoisonPillMsgShouldReturnFalseIfItIsNotAPoisonPill(void) {
  msg_t* m1 = msg_init("a message");
  CU_ASSERT_FALSE(isPoisonPillMsg(m1));
}
