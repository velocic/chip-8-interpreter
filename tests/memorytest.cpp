#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <memory.h>

TEST_CASE("Test memory is properly initialized when constructed", "Memory")
{
    Memory m;
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

    CHECK(m.getCurrentOpcode() == 0);

    //Check font set loaded into memory
    CHECK(m.getMemoryAtAddress(0x50) == fontSet[0]);
    for (int i = 0; i < 80; ++i) {
        CHECK((int)m.getMemoryAtAddress(i + 0x50) == (int)fontSet[i]);
    }

    bool foundNonZeroDataInGraphicsArray = false;
    unsigned char *graphics = m.getGraphics();
    for (int i = 0; i < 2048; ++i) {
        if (graphics[i] != 0) {
            foundNonZeroDataInGraphicsArray = true;
        }
    }
    CHECK(foundNonZeroDataInGraphicsArray == false);

    bool foundNonZeroDataInKeypad = false;
    unsigned char *keypad = m.getKeypadState();
    for (int i = 0; i < 16; ++i) {
        if (keypad[i] != 0) {
            foundNonZeroDataInKeypad = true;
        }
    }
    CHECK(foundNonZeroDataInKeypad == false);

    bool foundNonZeroDataInRegisters = false;
    for (int i = 0; i < 16; ++i) {
        if (m.getRegister(i) != 0) {
            foundNonZeroDataInRegisters = true;
        }
    }
    CHECK(foundNonZeroDataInRegisters == false);
    CHECK(m.getIndex() == 0);
    CHECK(m.getProgramCounter() == 0x200);
    CHECK(m.getDelayTimer() == 0);
    CHECK(m.getSoundTimer() == 0);
    CHECK(m.getDrawFlag() == false);
}

TEST_CASE("Test fetchOpcode() grabs current opcode correctly", "Memory")
{
    Memory m;

    m.setMemoryAtAddress(0x200, 0x00);
    m.setMemoryAtAddress(0x201, 0xE0);

    m.fetchOpcode();
    CHECK(m.getCurrentOpcode() == 0x00E0);
}

TEST_CASE("Test advanceToNextInstruction() increments program counter properly", "Memory")
{
    Memory m;

    CHECK(m.getProgramCounter() == 0x200);
    m.advanceToNextInstruction();
    CHECK(m.getProgramCounter() == 0x202);
    m.advanceToNextInstruction();
    CHECK(m.getProgramCounter() == 0x204);
}

TEST_CASE("Test fetchOpcode() grabs correct instruction", "Memory")
{
    //TODO: fill in after loading program to memory
}

TEST_CASE("Test flushGraphics() clears graphics array", "Memory")
{
    //TODO: fill in after implementing a way to set graphics
}

TEST_CASE("Test registerEquals() compares proper register to constant", "Memory")
{
    Memory m;

    CHECK(m.registerEquals(3, 50) == false);
    
    m.setRegister(3, 50);

    CHECK(m.registerEquals(3, 50) == true);
}

TEST_CASE("Test registersEqual() compares proper registers", "Memory")
{
    Memory m;

    CHECK(m.registersEqual(0x0, 0xF) == true);

    m.setRegister(0x0, 23);
    m.setRegister(0xF, 103);

    CHECK(m.registersEqual(0x0, 0xF) == false);
}

TEST_CASE("Test stack stores and retrieves 16 values", "Memory")
{
    Memory m;

    m.stackPush(1);
    m.stackPush(2);
    m.stackPush(3);
    m.stackPush(4);
    m.stackPush(5);
    m.stackPush(6);
    m.stackPush(7);
    m.stackPush(8);
    m.stackPush(9);
    m.stackPush(10);
    m.stackPush(11);
    m.stackPush(12);
    m.stackPush(13);
    m.stackPush(14);
    m.stackPush(15);
    m.stackPush(16);

    CHECK(m.stackPop() == 16);
    CHECK(m.stackPop() == 15);
    CHECK(m.stackPop() == 14);
    CHECK(m.stackPop() == 13);
    CHECK(m.stackPop() == 12);
    CHECK(m.stackPop() == 11);
    CHECK(m.stackPop() == 10);
    CHECK(m.stackPop() == 9);
    CHECK(m.stackPop() == 8);
    CHECK(m.stackPop() == 7);
    CHECK(m.stackPop() == 6);
    CHECK(m.stackPop() == 5);
    CHECK(m.stackPop() == 4);
    CHECK(m.stackPop() == 3);
    CHECK(m.stackPop() == 2);
    CHECK(m.stackPop() == 1);
}
