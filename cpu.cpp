#include <cpu.h>

void Cpu::initializeOpcodeTables()
{
    //0x0NNN, 0x00E0, 0x00EE
    this.opcodeFirstNibble[0] = &(this.executeSecondNibbleOpcodes());
    this.opcodeFirstNibble[1] = &(this.opcode0x1NNN());
    this.opcodeFirstNibble[2] = &(this.opcode0x2NNN());
    this.opcodeFirstNibble[3] = &(this.opcode0x3XNN());
    this.opcodeFirstNibble[4] = &(this.opcode0x4XNN());
    this.opcodeFirstNibble[5] = &(this.opcode0x5XY0());
    this.opcodeFirstNibble[6] = &(this.opcode0x6XNN());
    this.opcodeFirstNibble[7] = &(this.opcode0x7XNN());
    //0x8XY0 - 0x8XY7 and 0x8XYE
    this.opcodeFirstNibble[8] = &(this.executeSecondNibbleOpcodes());
    this.opcodeFirstNibble[9] = &(this.opcode0x9XY0());
    this.opcodeFirstNibble[0xA] = &(this.opcode0xANNN());
    this.opcodeFirstNibble[0xB] = &(this.opcode0xBNNN());
    this.opcodeFirstNibble[0xC] = &(this.opcode0xCXNN());
    this.opcodeFirstNibble[0xD] = &(this.opcode0xDXYN());
    this.opcodeFirstNibble[0xE] = &(this.executeSecondNibbleOpcodes());
    this.opcodeFirstNibble[0xF] = &(this.executeSecondNibbleOpcodes());
}

void Cpu::initializeEnvironment()
{
    this.initializeOpcodeTables();
}

void Cpu::emulateCycle()
{
}

void Cpu::executeFirstNibbleOpcodes()
{
}

void Cpu::executeSecondNibbleOpcodes()
{
}

void Cpu::opcode0x0NNN()
{
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
