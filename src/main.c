#include <printf.h>
#include "poison_pill/poison_pill.h"

int main() {
  msg_t* m1 = POISON_PILL_MSG;
  msg_t* m2 = POISON_PILL_MSG;
  msg_t* m3 = msg_init("ciao");
  printf("Content: %s\n", (char*)m1->content);

  if(isPoisonPillMsg(m1)) {
    printf("è un poison pill!!!");
  }

  if(isPoisonPillMsg(m3)) {
    printf("è un poison pill!!!");
  }

  return 0;
}
