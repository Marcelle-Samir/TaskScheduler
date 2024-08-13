cmake_minimum_required(VERSION 3.14)
project(HitachiScheduler)

add_executable(HitachiScheduler_test HitachiScheduler_test.cpp)
target_link_libraries(HitachiScheduler_test gtest gtest_main pthread)

enable_testing()

add_test(NAME HitachiScheduler_test COMMAND HitachiScheduler_test)

include(GoogleTest)
gtest_discover_tests(HitachiScheduler_test)