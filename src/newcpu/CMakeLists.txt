cmake_minimum_required(VERSION 3.10)
project(MintGB)
set(CMAKE_CXX_STANDARD 20)

add_library(CPU STATIC
    src/cpu.cpp
)

target_include_directories(CPU
    PRIVATE
        include/cpu
    PUBLIC
        include
        ../common/include/common
)

find_package(Catch2 3 REQUIRED)

add_executable(UnitTests
    tests/unit_tests.cpp
)

target_include_directories(UnitTests 
    PRIVATE 
        include
)

target_link_libraries(UnitTests 
    PRIVATE
        Catch2::Catch2WithMain 
        CPU
)