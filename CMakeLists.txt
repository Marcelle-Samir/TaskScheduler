cmake_minimum_required(VERSION 3.14)
project(HitachiScheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the main scheduler executable
add_executable(HitachiScheduler src/main.cpp src/HitachiScheduler.cpp)

# Link the pthread library
target_link_libraries(HitachiScheduler pthread)

# Include directories for GoogleTest
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

# Add the test executable and link with the main scheduler code
add_executable(HitachiScheduler_test test/HitachiScheduler_test.cpp src/HitachiScheduler.cpp)

target_link_libraries(HitachiScheduler_test gtest gtest_main pthread)

enable_testing()

add_test(NAME HitachiScheduler_test COMMAND HitachiScheduler_test)

include(GoogleTest)
gtest_discover_tests(HitachiScheduler_test)
