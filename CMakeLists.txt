cmake_minimum_required(VERSION 3.10) 

project(MultiProcessApplication) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(parent parent.cpp)

add_executable(child1 child1.cpp)
add_executable(child2 child2.cpp)

if(MSVC)
    target_compile_definitions(parent PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(child1 PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(child2 PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
