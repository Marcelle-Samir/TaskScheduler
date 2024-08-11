cmake_minimum_required(VERSION 3.14)
project(HitachiPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    HitachiPlugin_test
    HitachiPlugin_test.cpp
)

add_test(
    NAME HitachiPluginTest
    COMMAND BasicTest
)

target_link_libraries(
    HitachiPlugin_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(HitachiPlugin_test)