#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <chrono>
#include <thread>
#include <memory.h>
#include <opcodetable.h>
class Cpu
{
    private:
        Memory &memory;
        OpcodeTable &opcodeTable;
        int instructionCycleTimeMicroseconds = 16667; //approximation of 60hz
        void countdownTimers();
        void emulateCycle();

    public:
        Cpu(Memory &memory, OpcodeTable &opcodeTable) : memory(memory), opcodeTable(opcodeTable) {};
        void startEmulationLoop();
};

#endif
