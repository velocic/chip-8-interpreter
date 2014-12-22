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

       std::cout << "rangeStart = " << rangeStart << std::endl;
       std::cout << "rangeEnd = " << rangeEnd << std::endl;

       for (int i = rangeStart; i <= rangeEnd; ++i) {
           jumpTable[i] = it->second;
       }
    }
}

void Cpu::initializeOpcodeJumpTable()
{
    //first, initialize each array to noOp function pointers (just to make sure everything is initialized)
    std::map<std::pair<int, int>, opcodeFunction> blankOpcodeHandlerMap;
    blankOpcodeHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x00, 0xFF), &Cpu::noOp));
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcodeJumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0x0JumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0x8JumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0xEJumpTable);
    mapHexRangesToOpcodeValues(blankOpcodeHandlerMap, opcode0xFJumpTable);


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
    mapHexRangesToOpcodeValues(opcodeHandlerMap, opcodeJumpTable);

    //map opcode handlers to the sub-tables
    //0x0 opcode table
    std::map<std::pair<int, int>, opcodeFunction> opcode0x0HandlerMap;
    opcode0x0HandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xE0, 0xE0), &Cpu::opcode0x00E0));
    opcode0x0HandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xEE, 0xEE), &Cpu::opcode0x00EE));
    mapHexRangesToOpcodeValues(opcode0x0HandlerMap, opcode0x0JumpTable);
    //0x8 opcode table
    std::map<std::pair<int, int>, opcodeFunction> opcode0x8HandlerMap;
    //TODO: figure this out, 0x8 table structured differently
    //0xE opcode table
    std::map<std::pair<int, int>, opcodeFunction> opcode0xEHandlerMap;
    opcode0xEHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0x9E, 0x9E), &Cpu::opcode0xEX9E));
    opcode0xEHandlerMap.insert(std::pair<std::pair<int, int>, opcodeFunction>(std::pair<int, int>(0xA1, 0xA1), &Cpu::opcode0xEXA1));
    mapHexRangesToOpcodeValues(opcode0xEHandlerMap, opcode0xEJumpTable);
    //0xF opcode table
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
    mapHexRangesToOpcodeValues(opcode0xFHandlerMap, opcode0xFJumpTable);
}

void Cpu::initialize0x0JumpTable()
{
    // for (int i = 0; i < 256; i++) {
    //     if (i == 0xE0) {
    //         opcode0x0JumpTable[i] = &Cpu::opcode0x00E0;
    //     } else if (i == 0xEE) {
    //         opcode0x0JumpTable[i] = &Cpu::opcode0x00EE;
    //     } else {
    //         opcode0x0JumpTable[i] = &Cpu::noOp;
    //     }
    // }
}

void Cpu::initialize0x8JumpTable()
{

}

void Cpu::initialize0xEJumpTable()
{
}

void Cpu::initialize0xFJumpTable()
{
}

void Cpu::navigateOpcode0x0JumpTable()
{
    std::cout << "hopped into 0x0 jump table!" << std::endl;
}

void Cpu::navigateOpcode0x8JumpTable()
{
    std::cout << "hopped into 0x8 jump table!" << std::endl;
}

void Cpu::navigateOpcode0xEJumpTable()
{
    std::cout << "hopped into 0xE jump table!" << std::endl;
}

void Cpu::navigateOpcode0xFJumpTable()
{
    std::cout << "hopped into 0xF jump table!" << std::endl;
}

void Cpu::noOp()
{
    std::cout << "called noOp!" << std::endl;
    //do nothing
}

void Cpu::opcode0x0NNN()
{
    std::cout << "called 0x0NNN" << std::endl;
    //not sure how this works, not implementing for now
}

void Cpu::opcode0x00E0()
{
    std::cout << "called 0x00E0" << std::endl;
}

void Cpu::opcode0x00EE()
{
    std::cout << "called 0x00EE" << std::endl;
}

void Cpu::opcode0x1NNN()
{
    std::cout << "called 0x1NNN" << std::endl;
}

void Cpu::opcode0x2NNN()
{
    std::cout << "called 0x2NNN" << std::endl;
}
void Cpu::opcode0x3XNN()
{
    std::cout << "called 0x3XNN" << std::endl;
}

void Cpu::opcode0x4XNN()
{
    std::cout << "called 0x4XNN" << std::endl;
}

void Cpu::opcode0x5XY0()
{
    std::cout << "called 0x5XY0" << std::endl;
}

void Cpu::opcode0x6XNN()
{
    std::cout << "called 0x6XNN" << std::endl;
}

void Cpu::opcode0x7XNN()
{
    std::cout << "called 0x7XNN" << std::endl;
}

void Cpu::opcode0x8XY0()
{
    std::cout << "called 0x8XY0" << std::endl;
}

void Cpu::opcode0x8XY1()
{
    std::cout << "called 0x8XY1" << std::endl;
}

void Cpu::opcode0x8XY2()
{
    std::cout << "called 0x8XY2" << std::endl;
}

void Cpu::opcode0x8XY3()
{
    std::cout << "called 0x8XY3" << std::endl;
}

void Cpu::opcode0x8XY4()
{
    std::cout << "called 0x8XY4" << std::endl;
}

void Cpu::opcode0x8XY5()
{
    std::cout << "called 0x8XY5" << std::endl;
}

void Cpu::opcode0x8XY6()
{
    std::cout << "called 0x8XY6" << std::endl;
}

void Cpu::opcode0x8XY7()
{
    std::cout << "called 0x8XY7" << std::endl;
}

void Cpu::opcode0x8XYE()
{
    std::cout << "called 0x8XYE" << std::endl;
}

void Cpu::opcode0x9XY0()
{
    std::cout << "called 0x9XY0" << std::endl;
}

void Cpu::opcode0xANNN()
{
    std::cout << "called 0xANNN" << std::endl;
}

void Cpu::opcode0xBNNN()
{
    std::cout << "called 0xBNNN" << std::endl;
}

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
