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

unsigned char Memory::getRegister(unsigned char registerX)
{
    //TODO: handle case of invalid register specified
    return v[registerX];
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

void Memory::setDrawFlag(bool flag)
{
    drawFlag = flag;
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
