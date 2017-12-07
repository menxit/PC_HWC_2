#include <msg_t/msg_t.h>
#include <stddef.h>
#include <string.h>
#include "poison_pill.h"

msg_t *POISON_PILL_MSG_INSTANCE = NULL;
char *POISON_PILL = "POISON_PILL";

/**
 * Retrieve the instance of POISON_PILL_MSG
 * @return
 */
msg_t* getMsgPoisonPillInstance() {
  if(POISON_PILL_MSG_INSTANCE == NULL) {
    POISON_PILL_MSG_INSTANCE = msg_init(POISON_PILL);
    return POISON_PILL_MSG_INSTANCE;
  }
  return POISON_PILL_MSG_INSTANCE;
}

/**
 * Check that it is a poison pill message
 *
 * @param msg
 * @return
 */
int isPoisonPillMsg(msg_t *msg) {
  return strcmp(msg->content, POISON_PILL) == 0;
}
