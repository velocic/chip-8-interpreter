#ifndef OPCODES_H
#define OPCODES_H

#include <chrono>
#include <map>
#include <random>
#include <memory.h>

class OpcodeTable
{
    typedef void (OpcodeTable::*opcodeFunction)();

    private:
        Memory &memory;
        opcodeFunction opcodeJumpTable[256]; //arrays of opcode function pointers
        opcodeFunction opcode0x0JumpTable[256];
        opcodeFunction opcode0x8JumpTable[256];
        opcodeFunction opcode0xEJumpTable[256];
        opcodeFunction opcode0xFJumpTable[256];
        std::mt19937 rng;
        std::uniform_int_distribution<unsigned short> uniformDist;

        void initializeOpcodeJumpTable();
        void initializeRandomNumberGenerator();
        void mapHexRangesToOpcodeValues(const std::map<std::pair<int, int>, opcodeFunction> &opcodeHandlerMap, opcodeFunction (&jumpTable)[256]);
        std::map<std::pair<int, int>, opcodeFunction> getJumpTableOpcodeMap();
        std::map<std::pair<int, int>, opcodeFunction> get0x0JumpTableOpcodeMap();
        void initializeOpcode0x8JumpTable();
        std::map<std::pair<int, int>, opcodeFunction> get0xEJumpTableOpcodeMap();
        std::map<std::pair<int, int>, opcodeFunction> get0xFJumpTableOpcodeMap();
        void navigateOpcode0x0JumpTable();
        void navigateOpcode0x8JumpTable();
        void navigateOpcode0xEJumpTable();
        void navigateOpcode0xFJumpTable();
        void noOp();
        void opcode0x0NNN();
        void opcode0x00E0();
        void opcode0x00EE();
        void opcode0x1NNN();
        void opcode0x2NNN();
        void opcode0x4XNN();
        void opcode0x3XNN();
        void opcode0x5XY0();
        void opcode0x6XNN();
        void opcode0x7XNN();
        void opcode0x8XY0();
        void opcode0x8XY1();
        void opcode0x8XY2();
        void opcode0x8XY3();
        void opcode0x8XY4();
        void opcode0x8XY5();
        void opcode0x8XY6();
        void opcode0x8XY7();
        void opcode0x8XYE();
        void opcode0x9XY0();
        void opcode0xANNN();
        void opcode0xBNNN();
        void opcode0xCXNN();
        void opcode0xDXYN();
        void opcode0xEX9E();
        void opcode0xEXA1();
        void opcode0xFX07();
        void opcode0xFX0A();
        void opcode0xFX15();
        void opcode0xFX18();
        void opcode0xFX1E();
        void opcode0xFX29();
        void opcode0xFX33();
        void opcode0xFX55();
        void opcode0xFX65();
    public:
        OpcodeTable(Memory &memory) : memory(memory) {};
        void decodeAndExecuteOpcode(const unsigned short &opcode);
};

#endif
