#ifndef HWC2_SUITE_DISPATCHER_H
#define HWC2_SUITE_DISPATCHER_H

void aSubscribedReaderShouldReceiveMessages(void);
void twoSubscribedReaderShouldReceiveMessages(void);
void aSlowReaderShouldBeDeleted(void);
void onDispatcherWaitTheListOfBufferProviderShouldBeEmpty(void);
void onDispatcherWaitTheListOfReaderShouldReceiveAPoisonPill(void);
void dispatcherShouldSendOnlyNewMessagesToReaders(void);

#endif
