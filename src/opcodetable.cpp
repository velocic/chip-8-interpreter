#include <opcodetable.h>

void OpcodeTable::decodeAndExecuteOpcode(unsigned short opcode)
{
    (this->*opcodeJumpTable[opcode >> 8])();
}

void OpcodeTable::initializeOpcodeJumpTable()
{
    //first, initialize each array to noOp function pointers
    std::map<std::pair<int, int>, opcodeFunction> blankOpcodeHandlerMap;
    blankOpcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x00, 0xFF), &OpcodeTable::noOp));
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcodeJumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0x0JumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0x8JumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0xEJumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0xFJumpTable);

    //now, fill the jump tables with their appropriate function pointers
    mapHexRangesToOpcodeValues(getJumpTableOpcodeMap(), opcodeJumpTable);
    mapHexRangesToOpcodeValues(get0x0JumpTableOpcodeMap(), opcode0x0JumpTable);
    initializeOpcode0x8JumpTable();
    mapHexRangesToOpcodeValues(get0xEJumpTableOpcodeMap(), opcode0xEJumpTable);
    mapHexRangesToOpcodeValues(get0xFJumpTableOpcodeMap(), opcode0xFJumpTable);
}

void OpcodeTable::initializeRandomNumberGenerator()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    
    rng = std::mt19937(seed);
    uniformDist = std::uniform_int_distribution<unsigned short>(0, 255);
}

void OpcodeTable::mapHexRangesToOpcodeValues(const std::map<std::pair<int, int>, opcodeFunction> &opcodeHandlerMap, opcodeFunction (&jumpTable)[256])
{
    int rangeStart;
    int rangeEnd;
    //iterate through the map grabbing each range of values
    //and the corresponding opcode hanlder to assign to the range
    for (auto it = opcodeHandlerMap.begin(); it != opcodeHandlerMap.end(); ++it) {
       rangeStart = it->first.first;
       rangeEnd = it->first.second;

       for (int i = rangeStart; i <= rangeEnd; ++i) {
           jumpTable[i] = it->second;
       }
    }
}

std::map<std::pair<int, int>, OpcodeTable::opcodeFunction> OpcodeTable::getJumpTableOpcodeMap()
{
    //jump table opcode handler map
    //0x00 - 0x00 &navigateOpcode0x0JumpTable   0x01 - 0x0F &opcode0x0NNN   0x10 - 0x1F &opcode0x1NNN
    //0x20 - 0x2F &opcode0x2NNN                 0x30 - 0x3F &opcode0x3XNN   0x40 - 0x4F &opcode0x4XNN
    //0x50 - 0x5F &opcode0x5XY0                 0x60 - 0x6F &opcode0x6XNN   0x70 - 0x7F &opcode0x7XNN
    //0x80 - 0x8F &navigateOpcode0x8JumpTable   0x90 - 0x9F &opcode0x9XY0   0xA0 - 0xAF &opcode0xANNN
    //0xB0 - 0xBF &opcode0xBNNN                 0xC0 - 0xCF &opcode0xCXNN   0xD0 - 0xDF &opcode0xDXYN
    //0xE0 - 0xEF &navigateOpcode0xEJumpTable   0xF0 - 0xFF &navigateOpcode0xFJumpTable

    std::map<std::pair<int, int>, opcodeFunction> opcodeHandlerMap;

    //map of std::pair<int, int> and pointers to opcode function handlers. the first int of the pair defines the starting index
    //to map the opcode function handler to, and the second int defintes the ending index to assign the opcode function handler to
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x00, 0x00), &OpcodeTable::navigateOpcode0x0JumpTable));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x01, 0x0F), &OpcodeTable::opcode0x0NNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x10, 0x1F), &OpcodeTable::opcode0x1NNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x20, 0x2F), &OpcodeTable::opcode0x2NNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x30, 0x3F), &OpcodeTable::opcode0x3XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x40, 0x4F), &OpcodeTable::opcode0x4XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x50, 0x5F), &OpcodeTable::opcode0x5XY0));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x60, 0x6F), &OpcodeTable::opcode0x6XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x70, 0x7F), &OpcodeTable::opcode0x7XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x80, 0x8F), &OpcodeTable::navigateOpcode0x8JumpTable));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x90, 0x9F), &OpcodeTable::opcode0x9XY0));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xA0, 0xAF), &OpcodeTable::opcode0xANNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xB0, 0xBF), &OpcodeTable::opcode0xBNNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xC0, 0xCF), &OpcodeTable::opcode0xCXNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xD0, 0xDF), &OpcodeTable::opcode0xDXYN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xE0, 0xEF), &OpcodeTable::navigateOpcode0xEJumpTable));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xF0, 0xFF), &OpcodeTable::navigateOpcode0xFJumpTable));

    return opcodeHandlerMap;
}

std::map<std::pair<int, int>, OpcodeTable::opcodeFunction> OpcodeTable::get0x0JumpTableOpcodeMap()
{
    //0x00E0     - &opcode0x00E0
    //0x00EE     - &opcode0x00EE
    //all others - &noOp
    std::map<std::pair<int, int>, opcodeFunction> opcode0x0HandlerMap;
    opcode0x0HandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xE0, 0xE0), &OpcodeTable::opcode0x00E0));
    opcode0x0HandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xEE, 0xEE), &OpcodeTable::opcode0x00EE));

    return opcode0x0HandlerMap;
}

void OpcodeTable::initializeOpcode0x8JumpTable()
{
    //mapping here is a little weird, can't rely on mapHexRangesToOpcodeValues() to handle it
    //basically, the least-significant nibble determines which opcodeHandler we use

    //if least-significant nibble equals 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 or 0xE, then assign the appropriate handler
    //else, assign noOp to opcode0x8JumpTable at the current index
    for (unsigned char i = 0; i < 255; ++i) {
        switch (i & 0x0F) {
            case 0x00:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY0;
                break;
            case 0x01:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY1;
                break;
            case 0x02:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY2;
                break;
            case 0x03:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY3;
                break;
            case 0x04:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY4;
                break;
            case 0x05:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY5;
                break;
            case 0x06:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY6;
                break;
            case 0x07:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XY7;
                break;
            case 0x0E:
                opcode0x8JumpTable[i] = &OpcodeTable::opcode0x8XYE;
                break;
            default:
                opcode0x8JumpTable[i] = &OpcodeTable::noOp;
                break;
        }
    }
}

std::map<std::pair<int, int>, OpcodeTable::opcodeFunction> OpcodeTable::get0xEJumpTableOpcodeMap()
{
    //0xE09E     - 0xEF9E &opcode0xEX9E
    //0xE0A1     - 0xEFA1 &opcode0xEXA1
    //all others - &noOp
    std::map<std::pair<int, int>, opcodeFunction> opcode0xEHandlerMap;
    opcode0xEHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x9E, 0x9E), &OpcodeTable::opcode0xEX9E));
    opcode0xEHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xA1, 0xA1), &OpcodeTable::opcode0xEXA1));

    return opcode0xEHandlerMap;
}

std::map<std::pair<int, int>, OpcodeTable::opcodeFunction> OpcodeTable::get0xFJumpTableOpcodeMap()
{
    //0xF007 - 0xFF07 &opcode0xFX07     0xF00A - 0xFF0A &opcode0xFX0A       0xF015 - 0xFF15 &opcode0xFX15
    //0xF018 - 0xFF18 &opcode0xFX18     0xF01E - 0xFF1E &opcode0xFX1E       0xF029 - 0xFF29 &opcode0xFX29
    //0xF033 - 0xFF33 &opcode0xFX33     0xF055 - 0xFF55 &opcode0xFF55       0xF065 - 0xFF65 &opcode0xFF65
    //                                     all others - &noOp
    std::map<std::pair<int, int>, opcodeFunction> opcode0xFHandlerMap;
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x07, 0x07), &OpcodeTable::opcode0xFX07));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x0A, 0x0A), &OpcodeTable::opcode0xFX0A));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x15, 0x15), &OpcodeTable::opcode0xFX15));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x18, 0x18), &OpcodeTable::opcode0xFX18));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x1E, 0x1E), &OpcodeTable::opcode0xFX1E));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x29, 0x29), &OpcodeTable::opcode0xFX29));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x33, 0x33), &OpcodeTable::opcode0xFX33));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x55, 0x55), &OpcodeTable::opcode0xFX55));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x65, 0x65), &OpcodeTable::opcode0xFX65));

    return opcode0xFHandlerMap;
}

void OpcodeTable::navigateOpcode0x0JumpTable()
{
    (this->*opcode0x0JumpTable[memory.getCurrentOpcode() & 0x00FF])();
}

void OpcodeTable::navigateOpcode0x8JumpTable()
{
    (this->*opcode0x8JumpTable[memory.getCurrentOpcode() & 0x00FF])();
}

void OpcodeTable::navigateOpcode0xEJumpTable()
{
    (this->*opcode0xEJumpTable[memory.getCurrentOpcode() & 0x00FF])();
}

void OpcodeTable::navigateOpcode0xFJumpTable()
{
    (this->*opcode0xFJumpTable[memory.getCurrentOpcode() & 0x00FF])();
}

void OpcodeTable::noOp()
{
    /*TODO: log warning if we get into this function. If this gets called, we
     * either navigated the opcode table incorrectly, or the rom file called
     * an invalid entry of the table
     */
    memory.advanceToNextInstruction();
}

//apparently obsolete (outside of running on ancient hardware)
void OpcodeTable::opcode0x0NNN()
{
    memory.advanceToNextInstruction();
}

//clear graphics buffer
void OpcodeTable::opcode0x00E0()
{
    memory.flushGraphics();
    memory.setDrawFlag(true);
    memory.advanceToNextInstruction();
}

//return from subroutine
void OpcodeTable::opcode0x00EE()
{
    memory.setProgramCounter(memory.stackPop());
}

//jump to (opcode & 0x0FFF)
void OpcodeTable::opcode0x1NNN()
{
    memory.setProgramCounter(memory.getCurrentOpcode() & 0x0FFF);
}

//call subroutine at (opcode & 0x0FFF)
void OpcodeTable::opcode0x2NNN()
{
    memory.stackPush(memory.getProgramCounter());
    memory.setProgramCounter(memory.getCurrentOpcode() & 0x0FFF);
}

//skip next instruction if Vx == NN
void OpcodeTable::opcode0x3XNN()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char nn = opcode & 0x00FF;

    if (memory.registerEquals(x, nn)) {
        memory.advanceToNextInstruction();
    }

    memory.advanceToNextInstruction();
}

//skip next instruction if Vx != NN
void OpcodeTable::opcode0x4XNN()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char nn = opcode & 0x00FF;

    if (!memory.registerEquals(x, nn)) {
        memory.advanceToNextInstruction();
    }

    memory.advanceToNextInstruction();
}

//skip next instruction if Vx == Vy
void OpcodeTable::opcode0x5XY0()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    if (memory.registersEqual(x, y)) {
        memory.advanceToNextInstruction();
    }

    memory.advanceToNextInstruction();
}

//load NN into Vx
void OpcodeTable::opcode0x6XNN()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char nn = opcode & 0x00FF;

    memory.setRegister(x, nn);

    memory.advanceToNextInstruction();
}

//add NN to Vx, store result in Vx (doesn't set carry flag)
void OpcodeTable::opcode0x7XNN()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char nn = opcode & 0x00FF;

    memory.setRegister(x, memory.getRegister(x) + nn);

    memory.advanceToNextInstruction();
}

//store Vy into Vx
void OpcodeTable::opcode0x8XY0()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    memory.setRegister(x, memory.getRegister(y));

    memory.advanceToNextInstruction();
}

//OR Vx w/ Vy, store result in Vx
void OpcodeTable::opcode0x8XY1()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    memory.setRegister(x, memory.getRegister(x) | memory.getRegister(y));

    memory.advanceToNextInstruction();
}

//AND Vx w/ Vy, store result in Vx
void OpcodeTable::opcode0x8XY2()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    memory.setRegister(x, memory.getRegister(x) & memory.getRegister(y));

    memory.advanceToNextInstruction();
}

//XOR Vx w/ Vy, store result in Vx
void OpcodeTable::opcode0x8XY3()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    memory.setRegister(x, memory.getRegister(x) ^ memory.getRegister(y));

    memory.advanceToNextInstruction();
}

//add Vy to Vx, store result in Vx. If result > 255, set v[0xF] to 1, else set v[0xF] to 0 (carry flag)
void OpcodeTable::opcode0x8XY4()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    // if Vy > (255 - Vx), we will overflow 1 byte, so set the carry flag to 1
    if (memory.getRegister(y) > (0xFF - memory.getRegister(x))) {
        memory.setRegister(0xF, 1);
    } else {
        memory.setRegister(0xF, 0);
    }

    memory.setRegister(x, memory.getRegister(x) + memory.getRegister(y));

    memory.advanceToNextInstruction();
}

//if Vx > Vy, set v[0xF] to 1, else set v[0xF] to 0. then, Vx = Vx - Vy
void OpcodeTable::opcode0x8XY5()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    if (memory.getRegister(x) > memory.getRegister(y)) {
        memory.setRegister(0xF, 1);
    } else {
        memory.setRegister(0xF, 0);
    }

    memory.setRegister(x, memory.getRegister(x) - memory.getRegister(y));

    memory.advanceToNextInstruction();
}

// if least-significant bit of Vx is 1, set v[0xF] to 1, else set it to 0.
// then, divide by 2
void OpcodeTable::opcode0x8XY6()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    if ((memory.getRegister(x) & 0x01) == 0x01) {
        memory.setRegister(0xF, 1);
    } else {
        memory.setRegister(0xF, 0);
    }

    memory.setRegister(x, memory.getRegister(x) >> 1);

    memory.advanceToNextInstruction();
}

//if Vy > Vx, set v[0xF] to 1, else set to 0. Then Vx = Vy - Vx
void OpcodeTable::opcode0x8XY7()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    if (memory.getRegister(y) > memory.getRegister(x)) {
        memory.setRegister(0xF, 1);
    } else {
        memory.setRegister(0xF, 0);
    }

    memory.setRegister(x, memory.getRegister(y) - memory.getRegister(x));

    memory.advanceToNextInstruction();
}

//if most-significant bit of Vx is 1, set v[0xF] to 1. else, set it to 0. Then, multiply Vx by 2
void OpcodeTable::opcode0x8XYE()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    if ((memory.getRegister(x) >> 7) == 0x01) {
        memory.setRegister(0xF, 1);
    } else {
        memory.setRegister(0xF, 0);
    }

    memory.setRegister(x, memory.getRegister(x) << 1);

    memory.advanceToNextInstruction();
}

//skip next instruction if Vx != Vy
void OpcodeTable::opcode0x9XY0()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;

    if (!memory.registersEqual(x, y)) {
        memory.advanceToNextInstruction();
    }

    memory.advanceToNextInstruction();
}

//set index register to NNN
void OpcodeTable::opcode0xANNN()
{
    memory.setIndex(memory.getCurrentOpcode() & 0x0FFF);

    memory.advanceToNextInstruction();
}

//set pc to NNN + v[0x0]
void OpcodeTable::opcode0xBNNN()
{
    memory.setProgramCounter((memory.getCurrentOpcode() & 0x0FFF) + memory.getRegister(0x0));
}

//generate random number between 0 and 255, AND it with NN, then save the result in Vx
void OpcodeTable::opcode0xCXNN()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char nn = opcode & 0x00FF;

    memory.setRegister(x, nn & uniformDist(rng));

    memory.advanceToNextInstruction();
}

/* Note: This explanation comes from Cowgod's Chip-8 Technical Reference v1.0 verbatim.
 * The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as 
 * sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels
 * to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the
 * coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more
 * information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
 */
void OpcodeTable::opcode0xDXYN()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;
    unsigned char y = (opcode & 0x00F0) >> 4;
    unsigned char n = opcode & 0x000F;
    unsigned short drawCoordinate = (memory.getRegister(y) * 64) + memory.getRegister(x);
    bool collisionDetected = memory.drawSpriteAtAddress(
        drawCoordinate,
        memory.getMemoryAtAddress(memory.getIndex()),
        n
    );

    memory.setRegister(0xF, collisionDetected);
    memory.setDrawFlag(true);
    memory.advanceToNextInstruction();
}

//if the key corresponding to Vx is pressed, skip next instruction
void OpcodeTable::opcode0xEX9E()
{
    //TODO: figure out a good way to get user input into this function
}

//if the key corresponding to Vx is NOT pressed, skip next instruction
void OpcodeTable::opcode0xEXA1()
{
    //TODO: figure out a good way to get user input into this function
}

//set Vx to the value of the delay timer
void OpcodeTable::opcode0xFX07()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    memory.setRegister(x, memory.getDelayTimer());

    memory.advanceToNextInstruction();
}

//wait for a key press, then store the key's value in Vx
void OpcodeTable::opcode0xFX0A()
{
    //TODO: figure out a good way to get user input into this function
}

//set the delay timer to Vx
void OpcodeTable::opcode0xFX15()
{
    //TODO: need to count the timer down somehow
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    memory.setDelayTimer(memory.getRegister(x));

    memory.advanceToNextInstruction();
}

//set the sound timer to Vx
void OpcodeTable::opcode0xFX18()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    //TODO: need to count the timer down somehow
    memory.setSoundTimer(memory.getRegister(x));

    memory.advanceToNextInstruction();
}

//add index and Vx, store result in index
void OpcodeTable::opcode0xFX1E()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    memory.setIndex(memory.getIndex() + memory.getRegister(x));

    memory.advanceToNextInstruction();
}

//set i to the location of the sprite for digit Vx
void OpcodeTable::opcode0xFX29()
{
    //TODO: implement after setting character set into memory
}

//store binary-coded decimal representation of Vx in memory locations i, i+1. and i+2
void OpcodeTable::opcode0xFX33()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char registerValue = memory.getRegister((opcode & 0x0F00) >> 8);
    unsigned char leastSignificantDigit = 0;

    for (int i = 2; i >= 0; --i) {
        leastSignificantDigit = registerValue % 10;
        registerValue -= leastSignificantDigit;
        registerValue /= 10;

        memory.setMemoryAtAddress(memory.getIndex() + i, leastSignificantDigit);
    }

    memory.advanceToNextInstruction();
}

//store registers V0 - Vx in memory starting at index
void OpcodeTable::opcode0xFX55()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; ++i) {
        memory.setMemoryAtAddress(memory.getIndex() + i, memory.getRegister(i));
    }

    memory.advanceToNextInstruction();
}

//read memory starting at index into registers V0 - Vx
void OpcodeTable::opcode0xFX65()
{
    unsigned short opcode = memory.getCurrentOpcode();
    unsigned char x = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; ++i) {
        memory.setRegister(i, memory.getMemoryAtAddress(memory.getIndex() + i));
    }

    memory.advanceToNextInstruction();
}
