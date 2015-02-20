#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <chrono>
#include <thread>
#include <memory.h>
#include <opcodetable.h>
#include <interface/video.h>
#include <interface/controls.h>
class Cpu
{
    private:
        Memory &memory;
        OpcodeTable &opcodeTable;
        Video &video;
        Controls &controls;
        SDL_Event &event;
        int instructionCycleTimeMicroseconds = 16667; //approximation of 60hz
        bool userRequestedQuit = false;
        void countdownTimers();
        void emulateCycle();
        void pollEvents();

    public:
        Cpu(Memory &memory, OpcodeTable &opcodeTable, Video &video, Controls &controls, SDL_Event &event)
            : memory(memory), opcodeTable(opcodeTable), video(video), controls(controls), event(event) {};
        void startEmulationLoop();
};

#endif
