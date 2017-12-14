#############################################
# Include dependencies                      #
#############################################
include_directories(dependencies/hwc1/src)
include_directories(dependencies/hwc2_list)

add_subdirectory(dependencies/hwc1)
add_subdirectory(dependencies/hwc2_list)

#############################################
# Main HWC2                                 #
#############################################
set(HWC2_SOURCE main.c poison_pill/poison_pill.c poison_pill/poison_pill.h)
add_executable(HWC2 ${HWC2_SOURCE})
target_link_libraries(HWC2 pthread)
target_link_libraries(HWC2 hwc1)
target_link_libraries(HWC2 hwc2_list)
add_library(hwc2 ${HWC2_SOURCE})

#############################################
# Tests HWC2                                #
#############################################
find_library(CUNIT_LIBRARY cunit libcunit CUnit HINTS ~/homebrew/Cellar/cunit/2.1-3/lib /lib64 /usr/lib64)
get_filename_component(CUNIT_PATH ${CUNIT_LIBRARY} DIRECTORY)
include_directories("${CUNIT_PATH}/../include")
set(HWC2_TEST_SOURCE TestDriver.c tests/suite_poison_pill.c tests/suite_poison_pill.h poison_pill/poison_pill.c poison_pill/poison_pill.h Provider/Provider.h Provider/Provider.c tests/suite_provider.c tests/suite_provider.h Dispatcher/Dispatcher.c Dispatcher/Dispatcher.h Accepter/Accepter.h Accepter/Accepter.c Reader/Reader.h Reader/Reader.c tests/mocks/DispatcherMock/DispatcherMock.h tests/mocks/DispatcherMock/DispatcherMock.c tests/Factory/Factory.h tests/Factory/Factory.c)
add_executable(HWC2_TEST ${HWC2_TEST_SOURCE})
target_link_libraries(HWC2_TEST ${CUNIT_LIBRARY})
target_link_libraries(HWC2_TEST pthread)
target_link_libraries(HWC2_TEST hwc1)
target_link_libraries(HWC2_TEST hwc2_list)
