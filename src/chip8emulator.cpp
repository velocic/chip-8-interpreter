#include <cpu.h>
#include <interface/video.h>
#include <interface/controls.h>
#include <memory.h>
#include <opcodetable.h>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<unsigned char> loadFile(std::ifstream &inFile);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Proper usage is \"" << argv[0] <<" [file path]\".";
        std::cout << std::endl;
        return 1;
    }

    //Start up SDL for video/sounds/input
    SDL_Init(SDL_INIT_VIDEO);

    //Read in the file
    std::ifstream inFile(argv[1], std::ios::binary);

    if (!inFile.is_open()) {
        std::cout << "Unable to open " << argv[1] << std::endl;
        return 1;
    }

    std::vector<unsigned char> fileContents = loadFile(inFile);
    inFile.close();

    //Set fileContents to max size of valid program ROM. Pad with 0s
    //if necessary
    fileContents.resize(0xDFF, 0);

    Memory memory;
    memory.initialize(fileContents);
    OpcodeTable opcodeTable(memory);
    Video video(
        "Chip 8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480
    );
    Controls controls(SDL_GetKeyboardState(NULL));
    SDL_Event event;
    Cpu chip8(memory, opcodeTable, video, controls, event);

    //Start emulating!
    chip8.startEmulationLoop();

    //Close SDL before exiting
    SDL_Quit();

    return 0;
}

std::vector<unsigned char> loadFile(std::ifstream &inFile)
{
    auto iterator = std::istreambuf_iterator<char>(inFile);
    auto end = std::istreambuf_iterator<char>();

    std::vector<unsigned char> contents(iterator, end);

    return contents;
}

