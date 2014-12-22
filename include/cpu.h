#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <map>

class Cpu
{
    typedef void (Cpu::*opcodeFunction)();

    private:
        opcodeFunction opcodeJumpTable[256]; //arrays of opcode function pointers
        opcodeFunction opcode0x0JumpTable[256];
        opcodeFunction opcode0x8JumpTable[256];
        opcodeFunction opcode0xEJumpTable[256];
        opcodeFunction opcode0xFJumpTable[256];
        unsigned short opcode; //unsigned short = 2-bytes
        unsigned char memory[4096];
        unsigned char graphics[2048]; //64 pixels * 32 pixels
        unsigned short stack[16];
        unsigned short stackPointer;
        unsigned char keypad[16]; //supports 16 keys (0x0 - 0xF)
        unsigned char v[16]; //1-byte registers
        unsigned short i; //2-byte address index register
        unsigned short pc; //2-byte program counter
        unsigned char delayTimer; //1-byte delay timer
        unsigned char soundTimer; //1-byte sound timer
        
        void mapHexRangesToOpcodeValues(const std::map<std::pair<int, int>, opcodeFunction> &opcodeHandlerMap, opcodeFunction (&jumpTable)[256]);
        void initializeOpcodeJumpTable();
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
        void opcode0x3XNN();
        void opcode0x4XNN();
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
        Cpu() {};
        void initializeEnvironment();
        void emulateCycle();
};

#endif
