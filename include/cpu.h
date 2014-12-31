#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <chrono>
#include <memory.h>
#include <opcodetable.h>

class Cpu
{
    private:
        OpcodeTable opcodeTable;
        Memory memory;
    public:
        // Cpu(OpcodeTable opcodeTable, Memory memory) : opcodeTable(opcodeTable), memory(memory) {};
        Cpu() {};
        void initializeEnvironment();
        void emulateCycle();
};

#endif
