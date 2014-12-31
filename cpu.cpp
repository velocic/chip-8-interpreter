#include <cpu.h>
Cpu::Cpu()
{
    memory = Memory();
    opcodeTable = OpcodeTable(memory);
}

void Cpu::initializeEnvironment()
{
}

void Cpu::emulateCycle()
{
}

