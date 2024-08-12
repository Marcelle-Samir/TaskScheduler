cmake_minimum_required(VERSION 3.14)
project(HitachiScheduler)

# Include GoogleTest
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

# Add the test executable
add_executable(HitachiScheduler_test HitachiScheduler_test.cpp)

# Link the test executable with GoogleTest and pthread
target_link_libraries(HitachiScheduler_test gtest gtest_main pthread)

# Enable testing
enable_testing()

# Add the test
add_test(NAME HitachiScheduler_test COMMAND HitachiScheduler_test)

# Discover tests (correct the target name here)
include(GoogleTest)
gtest_discover_tests(HitachiScheduler_test)