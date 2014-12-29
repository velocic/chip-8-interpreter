Memory::Memory()
{
    //TODO: initialize all class members
}

void Memory::advanceToNextInstruction()
{
    programCounter += 2;
}

void Memory::fetchOpcode()
{
    opcode = memory[programCounter];
}

void Memory::flushGraphics()
{
    for (int i = 0; i < 2048; ++i) {
        graphics[i] &= 0x00;
    }
}

unsigned short getCurrentOpcode()
{
    return opcode;
}

bool Memory::getDrawFlag()
{
    return drawFlag;
}

unsigned short Memory::getProgramCounter()
{
    return programCounter;
}

bool registersEqual(unsigned char register1, unsigned char register2)
{
    if (v[register1] == v[register2]) {
        return true;
    }

    return false;
}

void Memory::setDrawFlag(bool flag)
{
    drawFlag = flag;
}

void Memory::setProgramCounter(unsigned short address)
{
    programCounter = address;
}

unsigned short Memory::stackPop()
{
    //TODO: handle case of empty stack
    return stack[stackPointer--];
}

void Memory::stackPush(unsigned short value)
{
    //TODO: handle case of full stack
    stack[stackPointer++] = value;
}
