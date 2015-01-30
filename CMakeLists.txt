cmake_minimum_required(VERSION 2.8)

project(Chip8Interpreter)

add_definitions(-std=c++11)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(
    ${PROJECT_NAME}
    src/chip8emulator.cpp
    src/cpu.cpp
    src/memory.cpp
    src/opcodetable.cpp
)

add_executable(
    MemoryTest
    tests/memorytest.cpp
    src/memory.cpp
)

add_executable(
    OpcodeTableTest
    tests/opcodetabletest.cpp
    src/memory.cpp
    src/opcodetable.cpp
)

install(
    TARGETS 
        Chip8Interpreter
        MemoryTest
        OpcodeTableTest
    DESTINATION
        bin
)