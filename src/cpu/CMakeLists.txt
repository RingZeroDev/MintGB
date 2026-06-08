cmake_minimum_required(VERSION 3.10)
project(SM83)
set(CMAKE_CXX_STANDARD 20)

add_library(SM83 STATIC)

target_sources(SM83 
    PRIVATE 
        src/sm83.cpp
        src/register.cpp
        src/flags.cpp
)
target_include_directories(SM83 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cpu
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(Catch2 3 REQUIRED)

add_executable(tests
    tests/sm83-tests.cpp
)

target_link_libraries(tests Catch2::Catch2WithMain SM83)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(CTest)
include(Catch)
catch_discover_tests(tests)
enable_testing()