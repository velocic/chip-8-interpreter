#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
    private:
        unsigned short opcode; //unsigned short = 2-bytes
        unsigned char memory[4096];
        unsigned char graphics[2048]; //64 pixels * 32 pixels
        unsigned short stack[16];
        unsigned short stackPointer;
        unsigned char keypad[16]; //supports 16 keys (0x0 - 0xF)
        unsigned char v[16]; //1-byte registers
        unsigned short index; //2-byte address index register
        unsigned short programCounter; //2-byte program counter
        unsigned char delayTimer; //1-byte delay timer
        unsigned char soundTimer; //1-byte sound timer
        bool drawFlag = false;
    public:
        void stackPush(unsigned char &);
        unsigned char stackPop();
        void advanceToNextInstruction();
        unsigned short fetchOpcode();
        bool getDrawFlag();
        void resetDrawFlag();
}

#endif
