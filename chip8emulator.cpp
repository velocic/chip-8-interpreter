#include <cpu.h>
#include <memory.h>
#include <opcodetable.h>

int main()
{
    // Memory memory;
    // OpcodeTable opcodeTable;
    Memory memory;
    OpcodeTable opcodeTable(memory);
    Cpu chip8(memory, opcodeTable);
    chip8.initializeEnvironment();
    
    //debug, run the emulateCycle funciton once
    chip8.emulateCycle();

    return 0;
}
