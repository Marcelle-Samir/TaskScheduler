include(GoogleTest)

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


set(SOURCES
    HitachiScheduler_test.cpp
    ../src/HitachiScheduler.cpp
    ../src/SchedulingAlgorithm.cpp
    ../src/Task.cpp
)

set(HEADERS
    ../src/HitachiScheduler.h
    ../src/SchedulingAlgorithm.h
    ../src/Task.h
)

add_executable(HitachiScheduler_test ${SOURCES} ${HEADERS})

target_link_libraries(HitachiScheduler_test gtest gtest_main pthread)

gtest_discover_tests(HitachiScheduler_test)

add_test(NAME HitachiScheduler_test COMMAND HitachiScheduler_test)
