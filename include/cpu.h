#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <chrono>
#include <thread>
#include <memory.h>
#include <opcodetable.h>
#include <video.h>
class Cpu
{
    private:
        Memory &memory;
        OpcodeTable &opcodeTable;
        Video &video;
        int instructionCycleTimeMicroseconds = 16667; //approximation of 60hz
        void countdownTimers();
        void emulateCycle();

    public:
        Cpu(Memory &memory, OpcodeTable &opcodeTable, Video &video) : memory(memory), opcodeTable(opcodeTable), video(video) {};
        void startEmulationLoop();
};

#endif
