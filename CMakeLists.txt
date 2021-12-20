set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
cmake_minimum_required(VERSION 3.16)

project(TestCompiler)

include(CheckIPOSupported)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(parsetree)

target_include_directories(${PROJECT_NAME} PUBLIC parsetree)

target_link_directories(${PROJECT_NAME} PUBLIC parsetree)

target_link_libraries(${PROJECT_NAME} parsetree segmenter) 