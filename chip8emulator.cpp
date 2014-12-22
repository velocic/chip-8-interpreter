#include <iostream>
#include <cpu.h>

int main()
{
    Cpu chip8;
    chip8.initializeEnvironment();
    
    //debug, run the emulateCycle funciton once
    chip8.emulateCycle();

    return 0;
}
