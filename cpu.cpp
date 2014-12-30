#include <cpu.h>

void Cpu::initializeEnvironment()
{
    initializeOpcodeJumpTable();
    initializeRandomNumberGenerator();
}

void Cpu::emulateCycle()
{
}

