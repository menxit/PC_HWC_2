#ifndef HWC2_POISON_PILL_H
#define HWC2_POISON_PILL_H

#include <msg_t/msg_t.h>
#include <stddef.h>

#define POISON_PILL_MSG getMsgPoisonPillInstance()

msg_t* getMsgPoisonPillInstance();
int isPoisonPillMsg(msg_t *msg);

#endif
