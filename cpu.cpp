#include <cpu.h>

void Cpu::initializeEnvironment()
{
    initializeOpcodeJumpTable();
}

void Cpu::emulateCycle()
{
}

void Cpu::initializeOpcodeJumpTable()
{
    //opcode handlers to map to the jump table
    opcodeFunction opcodes[16] = {
        &Cpu::opcode0x0JumpTable,
        &Cpu::opcode0x1NNN,
        &Cpu::opcode0x2NNN,
        &Cpu::opcode0x3XNN,
        &Cpu::opcode0x4XNN,
        &Cpu::opcode0x5XY0,
        &Cpu::opcode0x6XNN,
        &Cpu::opcode0x7XNN,
        &Cpu::opcode0x8JumpTable,
        &Cpu::opcode0x9XY0,
        &Cpu::opcode0xANNN,
        &Cpu::opcode0xBNNN,
        &Cpu::opcode0xCXNN,
        &Cpu::opcode0xDXYN,
        &Cpu::opcode0xEJumpTable,
        &Cpu::opcode0xFJumpTable
    };

    //fill the opcodeJumpTable
    int opcodePrefix = -1;
    for (int i = 0; i < 256; i++) {
        if (i % 16 == 0) {
            ++opcodePrefix;
        }

        opcodeJumpTable[i] = opcodes[opcodePrefix];
    }
}

void Cpu::opcode0x0JumpTable()
{
}

void Cpu::opcode0x8JumpTable()
{
}

void Cpu::opcode0xEJumpTable()
{
}

void Cpu::opcode0xFJumpTable()
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
