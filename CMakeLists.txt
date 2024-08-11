cmake_minimum_required(VERSION 3.10)

project(Hitachi-plugin VERSION 1.0 DESCRIPTION "Hitachi Plugin" LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_subdirectory(src)
add_subdirectory(test)