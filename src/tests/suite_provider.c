#include <CUnit/CUnit.h>
#include "../Provider/Provider.h"
#include "../Factory/Factory.h"

void test_startProviderWhenItIsNotRunningShouldNotFail(void) {
  Provider *provider = factory_createProviderWithDispatcherMock();
  CU_ASSERT_TRUE(provider->start(provider));
}

void test_startProviderWhenItIsRunningShouldFail(void) {
  Provider *provider = factory_createProviderWithDispatcherMock();
  provider->start(provider);
  CU_ASSERT_FALSE(provider->start(provider));
}

void test_startProviderAfterWaitShouldWork(void) {
  Provider *provider = factory_createProviderWithDispatcherMock();
  provider->start(provider);
  provider->wait(provider);
  CU_ASSERT_TRUE(provider->start(provider));
}
