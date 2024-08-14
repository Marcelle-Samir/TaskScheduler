cmake_minimum_required(VERSION 3.12)
project(HitachiScheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
  src/main.cpp
  src/HitachiScheduler.cpp
  src/SchedulingAlgorithm.cpp
  src/Task.cpp
)

set(HEADERS
  src/HitachiScheduler.h
  src/SchedulingAlgorithm.h
  src/Task.h
)

add_executable(HitachiScheduler ${SOURCES} ${HEADERS})

target_link_libraries(HitachiScheduler pthread)

enable_testing()
add_subdirectory(test)

