cmake_minimum_required(VERSION 3.10)
project(CPU)
set(CMAKE_CXX_STANDARD 20)

add_library(CPU STATIC)

target_sources(CPU 
    PRIVATE 
        src/cpu.cpp
        src/register.cpp
        src/decode.cpp
        src/decode_cb.cpp
        src/instructions/x8_alu.cpp
        src/instructions/x16_alu.cpp
        src/instructions/x8_lsm.cpp
        src/instructions/x16_lsm.cpp
        src/instructions/control_br.cpp
        src/instructions/control_misc.cpp
        src/instructions/x8_rsb.cpp
)

target_include_directories(CPU 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cpu
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CPU MMU Hardware)

find_package(Catch2 3 REQUIRED)

add_executable(tests
    tests/cpu_tests.cpp
)

target_link_libraries(tests Catch2::Catch2WithMain CPU)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(json INTERFACE)
target_include_directories(json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nlohmann)

add_executable(single_step_tests
    tests/single_step_tests.cpp
)

target_include_directories(single_step_tests 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include        
)

target_precompile_headers(single_step_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nlohmann/json.hpp)
target_link_libraries(single_step_tests json CPU MMU)

include(CTest)
include(Catch)
catch_discover_tests(tests)
enable_testing()