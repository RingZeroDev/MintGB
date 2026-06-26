cmake_minimum_required(VERSION 3.10)
project(MintGB)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(mmu)

add_library(CPU STATIC
    src/cpu.cpp
    src/lsm.cpp
    src/alu.cpp
    src/control.cpp
    src/execute.cpp
)

target_include_directories(CPU
    PRIVATE
        include/cpu
    PUBLIC
        include
        ../common/include/common
)

target_link_libraries(CPU 
    PUBLIC
        MMU
)

add_library(TestCPU STATIC
    src/cpu.cpp
    src/lsm.cpp
    src/alu.cpp
    src/control.cpp
    src/execute.cpp
)

target_include_directories(TestCPU
    PRIVATE
        include/cpu
    PUBLIC
        include
        ../common/include/common
)

target_link_libraries(TestCPU 
    PUBLIC
        TestMMU
)

target_compile_definitions(TestCPU PUBLIC TEST_CPU)

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
        TestCPU

)