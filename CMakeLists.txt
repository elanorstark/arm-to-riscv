cmake_minimum_required(VERSION 3.17)
project(Code)

set(CMAKE_CXX_STANDARD 14)

SET(CAPSTONE_ARCHITECTURE_DEFAULT OFF CACHE BOOL "" FORCE)
SET(CAPSTONE_ARM64_SUPPORT ON CACHE BOOL "" FORCE)
SET(CAPSTONE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(CAPSTONE_BUILD_CSTOOL OFF CACHE BOOL "" FORCE)

add_subdirectory(capstone)
add_subdirectory(ELFIO)

add_executable(Code main.cpp Value.cpp Value.h Instruction.cpp Instruction.h Elf_read.cpp Elf_read.h Memory.h Memory.cpp config.h)
target_link_libraries(Code capstone-static elfio)

add_executable(Test test.cpp)
target_link_libraries(Test elfio)