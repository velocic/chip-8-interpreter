#ifndef CPU_H
#define CPU_H

#include <iostream>

class Cpu
{
    typedef void (Cpu::*opcodeFunction)();

    private:
        //array of opcode function pointers
        opcodeFunction opcodeJumpTable[256];
        //16-bit opcode
        unsigned short opcode;
        unsigned char memory[4096];
        bool graphics[64*32];
        unsigned short stack[16];
        unsigned short stackPointer;
        //supports 16 keys 0x0 - 0xF
        unsigned char keypad[16];
        //8-bit registers
        unsigned char v[16];
        //16-bit address index register
        unsigned short i;
        //16-bit program counter
        unsigned short pc;
        //8-bit sound & delay timer
        unsigned char delayTimer;
        unsigned char soundTimer;
        
        void initializeOpcodeJumpTable();
        void opcode0x0JumpTable();
        void opcode0x8JumpTable();
        void opcode0xEJumpTable();
        void opcode0xFJumpTable();
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
