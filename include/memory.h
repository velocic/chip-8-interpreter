#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
class Memory
{
    private:
        unsigned short opcode; //unsigned short = 2-bytes
        unsigned char memory[4096];
        unsigned char graphics[2048]; //64 pixels * 32 pixels
        unsigned short stack[16];
        short stackPointer;
        unsigned char keypad[16]; //supports 16 keys (0x0 - 0xF)
        unsigned char v[16]; //1-byte registers
        unsigned short index; //2-byte address index register
        unsigned short programCounter; //2-byte program counter
        unsigned char delayTimer; //1-byte delay timer
        unsigned char soundTimer; //1-byte sound timer
        bool drawFlag = false;
        unsigned char fontSet[80] =
        { 
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

    public:
        Memory() {};
        void advanceToNextInstruction();
        void fetchOpcode();
        void flushGraphics();
        unsigned short getCurrentOpcode();
        unsigned char getDelayTimer();
        bool getDrawFlag();
        unsigned char (&getGraphics())[2048];
        unsigned short getIndex();
        unsigned char (&getKeypadState())[16];
        unsigned char getMemoryAtAddress(unsigned short address);
        unsigned short getProgramCounter();
        unsigned char getRegister(unsigned char registerX);
        unsigned char getSoundTimer();
        void initialize(std::vector<unsigned char> gameROM);
        bool registerEquals(unsigned char registerX, unsigned char constant);
        bool registersEqual(unsigned char registerX, unsigned char registerY);
        void setDelayTimer(unsigned char time);
        void setDrawFlag(bool drawFlag);
        bool drawSpriteAtCoordinates(int drawPositionX, int drawPositionY, unsigned short spriteAddress, unsigned char spriteHeight);
        void setIndex(unsigned short value);
        void setMemoryAtAddress(unsigned short address, unsigned char value);
        void setProgramCounter(unsigned short newCounter);
        void setRegister(unsigned char registerX, unsigned char value);
        void setSoundTimer(unsigned char time);
        unsigned short stackPop();
        void stackPush(unsigned short value);
};

#endif
