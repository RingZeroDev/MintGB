cmake_minimum_required(VERSION 3.10)
project(MintGB)
set(CMAKE_CXX_STANDARD 20)

add_library(BitUtils INTERFACE)

target_include_directories(CPU
    INTERFACE
        include/common
)