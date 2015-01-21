#include <cpu.h>

void Cpu::initializeEnvironment()
{
}

void Cpu::emulateCycle()
{
    memory.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(memory.getCurrentOpcode());
}

