#include <cpu.h>

void Cpu::startEmulationLoop()
{
    //loop endlessly
    for (;;) {
        //get time
        auto cycleStartTime = std::chrono::high_resolution_clock::now();
        emulateCycle();

        //get elapsed time in a format we can calculate against
        auto cycleElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - cycleStartTime).count();

        //sleep to synchronize instruction cycle time
        std::this_thread::sleep_for(std::chrono::microseconds(instructionCycleTimeMicroseconds - cycleElapsedTime));
    }
    //TODO: a way to terminate the program cleanly
}

void Cpu::emulateCycle()
{
    countdownTimers();
    //handleUserControls();
    memory.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(memory.getCurrentOpcode());
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

// void Cpu::handleUserControls()
// {
// }

