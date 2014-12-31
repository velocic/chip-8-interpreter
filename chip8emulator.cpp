#include <cpu.h>
#include <memory.h>
#include <opcodetable.h>

int main()
{
    Memory memory;
    OpcodeTable opcodeTable;
    Cpu chip8(opcodeTable, memory);
    chip8.initializeEnvironment();
    
    //debug, run the emulateCycle funciton once
    chip8.emulateCycle();

    return 0;
}
