cmake_minimum_required(VERSION 3.15)

project(HitachiPlugin VERSION 1.0 DESCRIPTION "Hitachi Plugin" LANGUAGES CXX)

option(PACKAGE_TESTS "Build the tests" ON)

add_library(project_options INTERFACE)                      
target_compile_features(project_options INTERFACE cxx_std_17)

set (CMAKE_CXX_STANDARD 17)
add_subdirectory(src)

if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)
endif()

