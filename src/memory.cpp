#include <memory.h>
#include <iostream>

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

void Memory::initialize(std::vector<unsigned char> gameROM)
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

    //Load the font-set into memory (memory 0x050 - 0x0A0)
    for (int i = 0; i < 80; ++i) {
        memory[i + 0x50] = fontSet[i];
    }

    //Load the program into memory (memory 0x200 - 0xFFF)
    for (std::vector<unsigned char>::size_type i = 0; i < gameROM.size(); ++i) {
        memory[i + 0x200] = gameROM[i];
    }
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
 * detected). Additionally, support horizontal and vertical screen-wrapping
 * if the sprite overflows horizontal or vertical screen bounds.
 */
bool Memory::drawSpriteAtCoordinates(int drawPositionX, int drawPositionY, unsigned short spriteAddress, unsigned char spriteHeight)
{
    unsigned char currentSpriteRow = 0;
    unsigned char currentBitOfSpriteRow = 0;
    bool collidedWithPixel = false;
    unsigned short calculatedOffset = 0;
    int tempDrawPositionX = 0;

    for (int rowLoopCounter = 0; rowLoopCounter < spriteHeight; ++rowLoopCounter)
    {
        currentSpriteRow = getMemoryAtAddress(getIndex() + rowLoopCounter);

        //vertical wrap-around check
        if (drawPositionY > 31) { //total number of rows (counting from 0)
            drawPositionY = 0;
        }

        tempDrawPositionX = drawPositionX;
        for (int columnLoopCounter = 0; columnLoopCounter < 8; ++columnLoopCounter)
        {
            //horizontal wrap-around check
            if (tempDrawPositionX > 63) { //one column-width (counting from 0)
                tempDrawPositionX = 0;
            }

            currentBitOfSpriteRow = ((currentSpriteRow >> (7 - columnLoopCounter)) & 0x01);
            calculatedOffset = (drawPositionY * 64) + tempDrawPositionX; //translate coordinates to 1d array offset
            graphics[calculatedOffset] ^= currentBitOfSpriteRow;

            //collision detection check
            if (graphics[calculatedOffset] != currentBitOfSpriteRow) {
                collidedWithPixel = true;
            }

            ++tempDrawPositionX;
        }

        ++drawPositionY;
    }

    return collidedWithPixel;
}

void Memory::setIndex(unsigned short value)
{
    //TODO: handle case of value > 0x0FFF
    index = value;
}

void Memory::setKeypadState(unsigned char SDLKeypadState[16])
{
    for (int i = 0; i < 16; ++i) {
        keypad[i] = SDLKeypadState[i];
    }
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
