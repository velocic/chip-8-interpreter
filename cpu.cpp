#include <cpu.h>
void Cpu::initializeEnvironment()
{
    initializeOpcodeJumpTable();
}

void Cpu::emulateCycle()
{
}

void Cpu::mapHexRangesToOpcodeValues(const std::map<std::pair<int, int>, opcodeFunction> &opcodeHandlerMap, opcodeFunction (&jumpTable)[256])
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

void Cpu::initializeOpcodeJumpTable()
{
    //first, initialize each array to noOp function pointers
    std::map<std::pair<int, int>, opcodeFunction> blankOpcodeHandlerMap;
    blankOpcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x00, 0xFF), &Cpu::noOp));
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

std::map<std::pair<int, int>, Cpu::opcodeFunction> Cpu::getJumpTableOpcodeMap()
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
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x00, 0x00), &Cpu::navigateOpcode0x0JumpTable));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x01, 0x0F), &Cpu::opcode0x0NNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x10, 0x1F), &Cpu::opcode0x1NNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x20, 0x2F), &Cpu::opcode0x2NNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x30, 0x3F), &Cpu::opcode0x3XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x40, 0x4F), &Cpu::opcode0x4XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x50, 0x5F), &Cpu::opcode0x5XY0));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x60, 0x6F), &Cpu::opcode0x6XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x70, 0x7F), &Cpu::opcode0x7XNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x80, 0x8F), &Cpu::navigateOpcode0x8JumpTable));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x90, 0x9F), &Cpu::opcode0x9XY0));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xA0, 0xAF), &Cpu::opcode0xANNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xB0, 0xBF), &Cpu::opcode0xBNNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xC0, 0xCF), &Cpu::opcode0xCXNN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xD0, 0xDF), &Cpu::opcode0xDXYN));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xE0, 0xEF), &Cpu::navigateOpcode0xEJumpTable));
    opcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xF0, 0xFF), &Cpu::navigateOpcode0xFJumpTable));

    return opcodeHandlerMap;
}

std::map<std::pair<int, int>, Cpu::opcodeFunction> Cpu::get0x0JumpTableOpcodeMap()
{
    //0x00E0     - &opcode0x00E0
    //0x00EE     - &opcode0x00EE
    //all others - &noOp
    std::map<std::pair<int, int>, opcodeFunction> opcode0x0HandlerMap;
    opcode0x0HandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xE0, 0xE0), &Cpu::opcode0x00E0));
    opcode0x0HandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xEE, 0xEE), &Cpu::opcode0x00EE));

    return opcode0x0HandlerMap;
}

void Cpu::initializeOpcode0x8JumpTable()
{
    //mapping here is a little weird, can't rely on mapHexRangesToOpcodeValues() to handle it
    //basically, the least-significant nibble determines which opcodeHandler we use

    //if least-significant nibble equals 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 or 0xE, then assign the appropriate handler
    //else, assign noOp to opcode0x8JumpTable at the current index
    for (unsigned char i = 0; i < 255; ++i) {
        switch (i & 0x0F) {
            case 0x00:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY0;
                break;
            case 0x01:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY1;
                break;
            case 0x02:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY2;
                break;
            case 0x03:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY3;
                break;
            case 0x04:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY4;
                break;
            case 0x05:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY5;
                break;
            case 0x06:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY6;
                break;
            case 0x07:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XY7;
                break;
            case 0x0E:
                opcode0x8JumpTable[i] = &Cpu::opcode0x8XYE;
                break;
            default:
                opcode0x8JumpTable[i] = &Cpu::noOp;
                break;
        }
    }
}

std::map<std::pair<int, int>, Cpu::opcodeFunction> Cpu::get0xEJumpTableOpcodeMap()
{
    //0xE09E     - 0xEF9E &opcode0xEX9E
    //0xE0A1     - 0xEFA1 &opcode0xEXA1
    //all others - &noOp
    std::map<std::pair<int, int>, opcodeFunction> opcode0xEHandlerMap;
    opcode0xEHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x9E, 0x9E), &Cpu::opcode0xEX9E));
    opcode0xEHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xA1, 0xA1), &Cpu::opcode0xEXA1));

    return opcode0xEHandlerMap;
}

std::map<std::pair<int, int>, Cpu::opcodeFunction> Cpu::get0xFJumpTableOpcodeMap()
{
    //0xF007 - 0xFF07 &opcode0xFX07     0xF00A - 0xFF0A &opcode0xFX0A       0xF015 - 0xFF15 &opcode0xFX15
    //0xF018 - 0xFF18 &opcode0xFX18     0xF01E - 0xFF1E &opcode0xFX1E       0xF029 - 0xFF29 &opcode0xFX29
    //0xF033 - 0xFF33 &opcode0xFX33     0xF055 - 0xFF55 &opcode0xFF55       0xF065 - 0xFF65 &opcode0xFF65
    //                                     all others - &noOp
    std::map<std::pair<int, int>, opcodeFunction> opcode0xFHandlerMap;
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x07, 0x07), &Cpu::opcode0xFX07));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x0A, 0x0A), &Cpu::opcode0xFX0A));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x15, 0x15), &Cpu::opcode0xFX15));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x18, 0x18), &Cpu::opcode0xFX18));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x1E, 0x1E), &Cpu::opcode0xFX1E));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x29, 0x29), &Cpu::opcode0xFX29));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x33, 0x33), &Cpu::opcode0xFX33));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x55, 0x55), &Cpu::opcode0xFX55));
    opcode0xFHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x65, 0x65), &Cpu::opcode0xFX65));

    return opcode0xFHandlerMap;
}

void Cpu::navigateOpcode0x0JumpTable()
{
    (this->*opcode0x0JumpTable[opcode & 0x00FF])();
}

void Cpu::navigateOpcode0x8JumpTable()
{
    (this->*opcode0x8JumpTable[opcode & 0x00FF])();
}

void Cpu::navigateOpcode0xEJumpTable()
{
    (this->*opcode0xEJumpTable[opcode & 0x00FF])();
}

void Cpu::navigateOpcode0xFJumpTable()
{
    (this->*opcode0xFJumpTable[opcode & 0x00FF])();
}

void Cpu::noOp()
{
    //do nothing. if we got here, something went wrong or the source
    //binary called an invalid operation
    std::cout << "called noOp!" << std::endl;
    pc += 2;
}

//apparently obsolete (outside of running on ancient hardware)
void Cpu::opcode0x0NNN()
{
    pc += 2;
}

//clear graphics buffer
void Cpu::opcode0x00E0()
{
    for (int i = 0; i < 2048; ++i) {
        graphics[i] &= 0x00;
    }

    pc += 2;
}

//return from subroutine
void Cpu::opcode0x00EE()
{
    pc = stack[stackPointer];
    --stackPointer;
}

//jump to (opcode & 0x0FFF)
void Cpu::opcode0x1NNN()
{
    pc = (opcode & 0x0FFF);
}

//call subroutine at (opcode & 0x0FFF)
void Cpu::opcode0x2NNN()
{
    stack[stackPointer] = pc;
    ++stackPointer;
    pc = (opcode & 0x0FFF);
}

//skip next instruction if Vx == NN
void Cpu::opcode0x3XNN()
{
    if (v[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
        pc += 2;
    }

    pc += 2;
}

//skip next instruction if Vx != NN
void Cpu::opcode0x4XNN()
{
    if (v[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
        pc += 2;
    }

    pc += 2;
}

//skip next instruction if Vx == Vy
void Cpu::opcode0x5XY0()
{
    if (v[(opcode & 0x0F00 >> 8)] == v[(opcode & 0x00F0) >> 4]) {
        pc += 2;
    }

    pc += 2;
}

//load NN into Vx
void Cpu::opcode0x6XNN()
{
    v[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;

    pc += 2;
}

//add NN to Vx, store result in Vx (doesn't set carry flag)
void Cpu::opcode0x7XNN()
{
    v[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);

    pc += 2;
}

//store Vy into Vx
void Cpu::opcode0x8XY0()
{
    v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x00F0) >> 4];

    pc += 2;
}

//OR Vx w/ Vy, store result in Vx
void Cpu::opcode0x8XY1()
{
    v[(opcode & 0x0F00) >> 8] |= v[(opcode & 0x00F0) >> 4];

    pc += 2;
}

//AND Vx w/ Vy, store result in Vx
void Cpu::opcode0x8XY2()
{
    v[(opcode & 0x0F00) >> 8] &= v[(opcode & 0x00F0) >> 4];

    pc += 2;
}

//XOR Vx w/ Vy, store result in Vx
void Cpu::opcode0x8XY3()
{
    v[(opcode & 0x0F00) >> 8] ^= v[(opcode & 0x00F0) >> 4];

    pc += 2;
}

//add Vy to Vx, store result in Vx. If result > 255, set v[0xF] to 1, else set v[0xF] to 0 (carry flag)
void Cpu::opcode0x8XY4()
{
    //if Vy > (255 - Vx), we will overflow 1 byte, so set the carry flag to 1
    if (v[(opcode & 0x00F0) >> 4] > (0xFF - v[(opcode & 0x0F00) >> 8])) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[(opcode & 0x0F00) >> 8] += v[(opcode & 0x00F0) >> 4];

    pc += 2;
}

//if Vx > Vy, set v[0xF] to 1, else set v[0xF] to 0. then, Vx = Vx - Vy
void Cpu::opcode0x8XY5()
{
    if (v[(opcode & 0x0F00) >> 8] > v[(opcode & 0x00F0) >> 4]) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[(opcode & 0x0F00) >> 8] -= v[(opcode & 0x00F0) >> 4];

    pc += 2;
}

// if least-significant bit of Vx is 1, set v[0xF] to 1, else set it to 0.
// then, divide by 2
void Cpu::opcode0x8XY6()
{
    if ((v[(opcode & 0x0F00) >> 8] & 0x01) == 0x01) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[(opcode & 0x0F00) >> 8] >>= 1;

    pc += 2;
}

//if Vy > Vx, set v[0xF] to 1, else set to 0. Then Vx = Vy - Vx
void Cpu::opcode0x8XY7()
{
    if (v[(opcode & 0x00F0) >> 4] > v[(opcode & 0x0F00) >> 8]) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[(opcode & 0x0F00) >> 8] = (v[(opcode & 0x00F0) >> 4] - v[(opcode & 0x0F00) >> 8]);

    pc += 2;
}

//if most-significant bit of Vx is 1, set v[0xF] to 1. else, set it to 0. Then, multiply Vx by 2
void Cpu::opcode0x8XYE()
{
    if ((v[(opcode & 0x0F00) >> 8] >> 7) == 0x01) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[(opcode & 0x0F00) >> 8] = (v[opcode & 0x0F00] << 1);

    pc += 2;
}

//skip next instruction if Vx != Vy
void Cpu::opcode0x9XY0()
{
    if (v[(opcode & 0x0F00) >> 8] != v[(opcode & 0x00F0) >> 4]) {
        pc += 2;
    }

    pc += 2;
}

//set index register i to NNN
void Cpu::opcode0xANNN()
{
    i = (opcode & 0x0FFF);

    pc += 2;
}

//set pc to NNN + v[0x0]
void Cpu::opcode0xBNNN()
{
    pc = (opcode & 0x0FFF) + v[0x0];
}

//generate random number between 0 and 255, AND it with NN, then save the result in Vx
void Cpu::opcode0xCXNN()
{
    std::cout << "called 0xCXNN" << std::endl;
}

void Cpu::opcode0xDXYN()
{
    std::cout << "called 0xDXYN" << std::endl;
}

void Cpu::opcode0xEX9E()
{
    std::cout << "called 0xEX9E" << std::endl;
}

void Cpu::opcode0xEXA1()
{
    std::cout << "called 0xEXA1" << std::endl;
}

void Cpu::opcode0xFX07()
{
    std::cout << "called 0xFX07" << std::endl;
}

void Cpu::opcode0xFX0A()
{
    std::cout << "called 0xFX0A" << std::endl;
}

void Cpu::opcode0xFX15()
{
    std::cout << "called 0xFX15" << std::endl;
}

void Cpu::opcode0xFX18()
{
    std::cout << "called 0xFX18" << std::endl;
}

void Cpu::opcode0xFX1E()
{
    std::cout << "called 0xFX1E" << std::endl;
}

void Cpu::opcode0xFX29()
{
    std::cout << "called 0xFX29" << std::endl;
}

void Cpu::opcode0xFX33()
{
    std::cout << "called 0xFX33" << std::endl;
}

void Cpu::opcode0xFX55()
{
    std::cout << "called 0xFX55" << std::endl;
}

void Cpu::opcode0xFX65()
{
    std::cout << "called 0xFX65" << std::endl;
}
