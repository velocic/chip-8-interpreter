#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <chrono>
#include <memory.h>
#include <opcodetable.h>

class Cpu
{
    private:
        Memory &memory;
        OpcodeTable &opcodeTable;
    public:
        Cpu(Memory &memory, OpcodeTable &opcodeTable) : memory(memory), opcodeTable(opcodeTable) {};
        void initializeEnvironment();
        void emulateCycle();
};

#endif
