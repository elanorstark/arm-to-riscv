cmake_minimum_required(VERSION 3.17)
project(Code)

set(CMAKE_CXX_STANDARD 14)

add_executable(Code main.cpp Value.cpp Value.h Instruction.cpp Instruction.h)