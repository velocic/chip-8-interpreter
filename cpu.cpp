#include <cpu.h>

void Cpu::initializeEnvironment()
{
    initializeJumpTables();
}

void Cpu::emulateCycle()
{
}

void Cpu::initializeJumpTables()
{
    opcodeJumpTable[0x0] = &Cpu::navigate0x0JumpTable;
    opcodeJumpTable[0x1] = &Cpu::opcode0x1NNN;
    opcodeJumpTable[0x2] = &Cpu::opcode0x2NNN;
    opcodeJumpTable[0x3] = &Cpu::opcode0x3XNN;
    opcodeJumpTable[0x4] = &Cpu::opcode0x4XNN;
    opcodeJumpTable[0x5] = &Cpu::opcode0x5XY0;
    opcodeJumpTable[0x6] = &Cpu::opcode0x6XNN;
    opcodeJumpTable[0x7] = &Cpu::opcode0x7XNN;
    opcodeJumpTable[0x8] = &Cpu::navigate0x8JumpTable;
    opcodeJumpTable[0x9] = &Cpu::opcode0x9XY0;
    opcodeJumpTable[0xA] = &Cpu::opcode0xANNN;
    opcodeJumpTable[0xB] = &Cpu::opcode0xBNNN;
    opcodeJumpTable[0xC] = &Cpu::opcode0xCXNN;
    opcodeJumpTable[0xD] = &Cpu::opcode0xDXYN;
    opcodeJumpTable[0xE] = &Cpu::navigate0xEJumpTable;
    opcodeJumpTable[0xF] = &Cpu::navigate0xFJumpTable;

    //0x0
    opcode0x0JumpTable[0xE0] = &Cpu::opcode0x00E0;
    opcode0x0JumpTable[0xEE] = &Cpu::opcode0x00EE;

    //0x8
    opcode0x8JumpTable[0x0] = &Cpu::opcode0x8XY0;
    opcode0x8JumpTable[0x1] = &Cpu::opcode0x8XY1;
    opcode0x8JumpTable[0x2] = &Cpu::opcode0x8XY2;
    opcode0x8JumpTable[0x3] = &Cpu::opcode0x8XY3;
    opcode0x8JumpTable[0x4] = &Cpu::opcode0x8XY4;
    opcode0x8JumpTable[0x5] = &Cpu::opcode0x8XY5;
    opcode0x8JumpTable[0x6] = &Cpu::opcode0x8XY6;
    opcode0x8JumpTable[0x7] = &Cpu::opcode0x8XY7;
    opcode0x8JumpTable[0x8] = &Cpu::opcode0x8XYE;

    //0xE
    opcode0xEJumpTable[0x9] = &Cpu::opcode0xEX9E;
    opcode0xEJumpTable[0xA] = &Cpu::opcode0xEXA1;

    //0xF
    opcode0xFJumpTable[0x07] = &Cpu::opcode0xFX07;
    opcode0xFJumpTable[0x0A] = &Cpu::opcode0xFX0A;
    opcode0xFJumpTable[0x15] = &Cpu::opcode0xFX15;
    opcode0xFJumpTable[0x18] = &Cpu::opcode0xFX18;
    opcode0xFJumpTable[0x1E] = &Cpu::opcode0xFX1E;
    opcode0xFJumpTable[0x29] = &Cpu::opcode0xFX29;
    opcode0xFJumpTable[0x33] = &Cpu::opcode0xFX33;
    opcode0xFJumpTable[0x55] = &Cpu::opcode0xFX55;
    opcode0xFJumpTable[0x65] = &Cpu::opcode0xFX65;

}

void Cpu::navigateJumpTable()
{
}

void Cpu::navigate0x0JumpTable()
{
}

void Cpu::navigate0x8JumpTable()
{
}

void Cpu::navigate0xEJumpTable()
{
}

void Cpu::navigate0xFJumpTable()
{
}

void Cpu::opcode0x0NNN()
{
    //not sure how this works, not implementing for now
}

void Cpu::opcode0x00E0()
{
}

void Cpu::opcode0x00EE()
{
}

void Cpu::opcode0x1NNN()
{
}

void Cpu::opcode0x2NNN()
{
}
void Cpu::opcode0x3XNN()
{
}

void Cpu::opcode0x4XNN()
{
}

void Cpu::opcode0x5XY0()
{
}

void Cpu::opcode0x6XNN()
{
}

void Cpu::opcode0x7XNN()
{
}

void Cpu::opcode0x8XY0()
{
}

void Cpu::opcode0x8XY1()
{
}

void Cpu::opcode0x8XY2()
{
}

void Cpu::opcode0x8XY3()
{
}

void Cpu::opcode0x8XY4()
{
}

void Cpu::opcode0x8XY5()
{
}

void Cpu::opcode0x8XY6()
{
}

void Cpu::opcode0x8XY7()
{
}

void Cpu::opcode0x8XYE()
{
}

void Cpu::opcode0x9XY0()
{
}

void Cpu::opcode0xANNN()
{
}

void Cpu::opcode0xBNNN()
{
}

void Cpu::opcode0xCXNN()
{
}

void Cpu::opcode0xDXYN()
{
}

void Cpu::opcode0xEX9E()
{
}

void Cpu::opcode0xEXA1()
{
}

void Cpu::opcode0xFX07()
{
}

void Cpu::opcode0xFX0A()
{
}

void Cpu::opcode0xFX15()
{
}

void Cpu::opcode0xFX18()
{
}

void Cpu::opcode0xFX1E()
{
}

void Cpu::opcode0xFX29()
{
}

void Cpu::opcode0xFX33()
{
}

void Cpu::opcode0xFX55()
{
}

void Cpu::opcode0xFX65()
{
}
