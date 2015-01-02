#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <memory.h>

TEST_CASE("Memory", "Test memory is properly initialized when constructed")
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

    //TODO: test stack is zeroed out
    //TODO: test stack pointer is zero

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

// TEST_CASE("Memory", "Test advanceToNextInstruction() increments program counter properly") {
//     // Memory m;
// }
