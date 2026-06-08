cmake_minimum_required(VERSION 3.10)
project(SM83)
set(CMAKE_CXX_STANDARD 20)

add_library(SM83)

target_sources(SM83 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/sm83.cpp)
target_include_directories(SM83 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cpu
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)