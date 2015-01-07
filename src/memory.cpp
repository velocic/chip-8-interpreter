#include <memory.h>

Memory::Memory()
{
    initialize();
}

void Memory::advanceToNextInstruction()
{
    programCounter += 2;
}

void Memory::fetchOpcode()
{
    opcode = memory[programCounter] << 8;
    opcode |= memory[programCounter+1];
}

void Memory::flushGraphics()
{
    for (int i = 0; i < 2048; ++i) {
        graphics[i] = 0x00;
    }
}

unsigned short Memory::getCurrentOpcode()
{
    return opcode;
}

unsigned char Memory::getDelayTimer()
{
    return delayTimer;
}

bool Memory::getDrawFlag()
{
    return drawFlag;
}

unsigned char (&Memory::getGraphics())[2048]
{
    return graphics;
}

unsigned short Memory::getIndex()
{
    return index;
}

unsigned char (&Memory::getKeypadState())[16]
{
    return keypad;
}

unsigned char Memory::getMemoryAtAddress(unsigned short address)
{
    //TODO: handle case of address > 0x0FFF
    return memory[address];
}

unsigned short Memory::getProgramCounter()
{
    return programCounter;
}

unsigned char Memory::getRegister(unsigned char registerX)
{
    //TODO: handle case of invalid register specified
    return v[registerX];
}

unsigned char Memory::getSoundTimer()
{
    return soundTimer;
}

void Memory::initialize()
{
    opcode = 0;
    flushGraphics();

    //zero-out the arrays
    for (int i = 0; i < 4096; ++i) {
        memory[i] = 0;

        if (i < 16) {
            stack[i] = 0;
            keypad[i] = 0;
            v[i] = 0;
        }
    }

    stackPointer = -1;
    index = 0;

    //programs start at 0x200, memory below that reserved for the interpreter
    programCounter = 0x200;
    delayTimer = 0;
    soundTimer = 0;

    //TODO: Load the font-set into memory
    //TODO: Load the program into memory

}

bool Memory::registerEquals(unsigned char registerX, unsigned char constant)
{
    //TODO: handle case of invalid register being passed, or when constant > 12-bit max
    if (v[registerX] == constant) {
        return true;
    }

    return false;
}

bool Memory::registersEqual(unsigned char registerX, unsigned char registerY)
{
    //TODO: handle case of invalid registers being passed
    if (v[registerX] == v[registerY]) {
        return true;
    }

    return false;
}

void Memory::setDelayTimer(unsigned char time)
{
    delayTimer = time;
}

void Memory::setDrawFlag(bool flag)
{
    drawFlag = flag;
}

/*
 * XORs each bit of value into graphics[address] to graphics[address+8].
 * Returns true (collision detected) if this operation flips a bit in the
 * graphics array from 1 to 0. Otherwise, this returns false (collision not
 * detected)
 */
bool Memory::setGraphicsAtAddress(unsigned short address, unsigned char value)
{
    bool collidedWithPixel = false;

    for (int i = 0; i < 8; ++i) {
        graphics[address + i] ^= ((value >> (7 - i)) & 0x01);

        if (graphics[address + i] != ((value >> (7 - i)) & 0x01)) {
            collidedWithPixel = true;
        }
    }

    return collidedWithPixel;
}

void Memory::setIndex(unsigned short value)
{
    //TODO: handle case of value > 0x0FFF
    index = value;
}

void Memory::setMemoryAtAddress(unsigned short address, unsigned char value)
{
    //TODO: handle case of address > 0x0FFF (max addressable memory space)
    memory[address] = value;
}

void Memory::setProgramCounter(unsigned short address)
{
    programCounter = address;
}

void Memory::setRegister(unsigned char registerX, unsigned char value)
{
    //TODO: handle case of invalid register specified
    v[registerX] = value;
}

void Memory::setSoundTimer(unsigned char time)
{
    soundTimer = time;
}

unsigned short Memory::stackPop()
{
    //TODO: handle case of empty stack
    return stack[stackPointer--];
}

void Memory::stackPush(unsigned short value)
{
    //TODO: handle case of full stack
    ++stackPointer;
    stack[stackPointer] = value;
}
