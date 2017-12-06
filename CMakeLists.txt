cmake_minimum_required(VERSION 2.8)
project(HWC2)
set(CMAKE_C_STANDARD 99)

#############################################
# Include dependencies                      #
#############################################
add_subdirectory(dependencies/hwc1)
include_directories(dependencies/hwc1/src)
add_subdirectory(dependencies/hwc2_list)
include_directories(dependencies/hwc2_list)

#############################################
# Main HWC2                                 #
#############################################
set(SOURCE_FILES src/main.c)
add_executable(HWC2 src/main.c)
target_link_libraries(HWC2 hwc1)
target_link_libraries(HWC2 hwc2_list)
target_link_libraries(HWC2 pthread)

#############################################
# Tests HWC2                                #
#############################################
find_library(CUNIT_LIBRARY cunit libcunit CUnit HINTS ~/homebrew/Cellar/cunit/2.1-3/lib /lib64 /usr/lib64)
get_filename_component(CUNIT_PATH ${CUNIT_LIBRARY} DIRECTORY)
include_directories("${CUNIT_PATH}/../include")
set(SOURCE_FILES TestDriver.c)
add_executable(HWC2_TEST test/TestDriver.c test/all_tests/all_tests.c test/all_tests/all_tests.h)
target_link_libraries(HWC2_TEST ${CUNIT_LIBRARY})
target_link_libraries(HWC2_TEST pthread)
target_link_libraries(HWC2_TEST hwc2_list)
