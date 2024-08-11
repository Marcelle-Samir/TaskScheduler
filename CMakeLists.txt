cmake_minimum_required(VERSION 3.10)
option(PACKAGE_TESTS "Build the tests" ON)

project(Hitachi-plugin VERSION 1.0 DESCRIPTION "Hitachi Plugin" LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)
endif()

add_subdirectory(src)
add_subdirectory(test)