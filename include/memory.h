#ifndef MEMORY_H
#define MEMORY_H

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

        void initialize();
    public:
        Memory();
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
        bool registerEquals(unsigned char registerX, unsigned char constant);
        bool registersEqual(unsigned char registerX, unsigned char registerY);
        void setDelayTimer(unsigned char time);
        void setDrawFlag(bool drawFlag);
        // bool setGraphicsAtAddress(unsigned short address, unsigned char startColumn, unsigned char value);
        bool setGraphicsAtAddress(unsigned short drawAddress, unsigned short spriteAddress, unsigned char spriteHeight);
        void setIndex(unsigned short value);
        void setMemoryAtAddress(unsigned short address, unsigned char value);
        void setProgramCounter(unsigned short newCounter);
        void setRegister(unsigned char registerX, unsigned char value);
        void setSoundTimer(unsigned char time);
        unsigned short stackPop();
        void stackPush(unsigned short value);
};

#endif
