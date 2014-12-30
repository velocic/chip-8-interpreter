#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <map>
#include <random>
#include <chrono>

class Cpu
{
    private:
        OpcodeTable opcodeTable;
        Memory memory;
    public:
        Cpu(OpcodeTable opcodeTable, Memory memory) : opcodeTable(opcodeTable), memory(memory) {};
        void initializeEnvironment();
        void emulateCycle();
};

#endif
