#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <memory.h>

TEST_CASE("Test memory is properly initialized when constructed", "Memory")
{
    Memory m;

    CHECK(m.getCurrentOpcode() == 0);

    //set this flag so we aren't running one assertion for every single index
    //of m's memory array
    bool foundNonZeroDataInMemoryArray = false;
    for (int i = 0; i < 4096; ++i) {
        if (m.getMemoryAtAddress(i) != 0) {
            foundNonZeroDataInMemoryArray = true;
        }
    }
    CHECK(foundNonZeroDataInMemoryArray == false);

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

TEST_CASE("Test advanceToNextInstruction() increments program counter properly", "Memory")
{
    Memory m;

    CHECK(m.getProgramCounter() == 0x200);
    m.advanceToNextInstruction();
    CHECK(m.getProgramCounter() == 0x202);
    m.advanceToNextInstruction();
    CHECK(m.getProgramCounter() == 0x204);
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
    CHECK(m.stackPop() == 3);
    CHECK(m.stackPop() == 2);
    CHECK(m.stackPop() == 1);
}
