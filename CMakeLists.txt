cmake_minimum_required(VERSION 3.17)
project(Code)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(capstone)
add_subdirectory(ELFIO)

add_executable(Code main.cpp Value.cpp Value.h Instruction.cpp Instruction.h elf_read.cpp elf_read.h)
target_link_libraries(Code capstone-static elfio)

add_executable(Test test.cpp)
target_link_libraries(Test elfio)