#include <cpu.h>

void Cpu::startEmulationLoop()
{
    while (userRequestedQuit == false && controls.userRequestedExit() == false) {
        //get time
        auto cycleStartTime = std::chrono::high_resolution_clock::now();
        emulateCycle();

        //get elapsed time in a format we can calculate against
        auto cycleElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - cycleStartTime).count();

        //sleep to synchronize instruction cycle time
        std::this_thread::sleep_for(std::chrono::microseconds(instructionCycleTimeMicroseconds - cycleElapsedTime));
    }
}

void Cpu::emulateCycle()
{
    countdownTimers();
    pollEvents();
    memory.setKeypadState(controls.getKeypadState());
    memory.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(memory.getCurrentOpcode());

    if (memory.getDrawFlag() == true) {
        video.clearScreen();
        video.drawScreen(memory.getGraphics());
        memory.setDrawFlag(false);
    }
}

void Cpu::countdownTimers()
{
    unsigned char delayTimer = memory.getDelayTimer();
    unsigned char soundTimer = memory.getSoundTimer();

    if (delayTimer > 0) {
        memory.setDelayTimer(--delayTimer);
    }

    if (soundTimer > 0) {
        memory.setSoundTimer(--soundTimer);
    }
}

//Event queue must be polled before detecting keyboard state
void Cpu::pollEvents()
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            userRequestedQuit = true;
        }
    }
}
