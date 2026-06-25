cmake_minimum_required(VERSION 3.10)
project(MintGB)
set(CMAKE_CXX_STANDARD 20)

add_library(MMU STATIC
    src/mmu.cpp
)

target_include_directories(MMU
    PRIVATE
        include/mmu
    PUBLIC
        include
)

add_library(TestMMU STATIC
    src/test_mmu.cpp
)

target_include_directories(TestMMU
    PRIVATE
        include/mmu
    PUBLIC
        include
)

