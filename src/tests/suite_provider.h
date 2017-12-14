#ifndef HWC2_SUITE_PROVIDER_H
#define HWC2_SUITE_PROVIDER_H

void test_startProviderWhenItIsNotRunningShouldNotFail(void);
void test_startProviderWhenItIsRunningShouldFail(void);
void test_startProviderAfterWaitShouldWork(void);

#endif
