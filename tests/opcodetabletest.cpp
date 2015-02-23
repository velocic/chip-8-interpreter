#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <memory.h>
#include <opcodetable.h>
#include <iostream>
#include <vector>

//for visually debugging graphics
void printGraphicsArrayToConsole(unsigned char *graphics)
{
        for (int i = 0; i < 2048; ++i) {
            std::cout << (int)graphics[i];
            if (((i+1) % 64 == 0)) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
}

void prettyPrintGraphicsArrayToConsole(unsigned char *graphics)
{
        //draw top bounds of box
        for (int i = 0; i < 64; ++i) {
            std::cout << '_';
        }
        std::cout << std::endl;
        //render the graphics array, with pipes to draw horizontal borders
        for (int i = 0; i < 2048; ++i) {
            if (i == 0) {
                std::cout << '|';
            }
            if (graphics[i] == 1) {
                std::cout << '@';
            } else {
                std::cout << ' ';
            }
            if (((i+1) % 64 == 0)) {
                std::cout << '|'<< i << std::endl << '|';
            }
        }
        //draw bottom bounds of box
        for(int i = 0; i < 64; ++i) {
            std::cout << '-';
        }

        std::cout << std::endl;
}

/*
 * Graphics should be blanked out, draw flag should be set,
 * and we should advance to the next instruction
 */
TEST_CASE("Test 0x00E0 handler", "OpcodeTable")
{ 
    //Setup
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0x00);
    m.setMemoryAtAddress(0x201, 0xE0);
    m.fetchOpcode();

    //Run opcode handler
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    //Analzye results
    unsigned char *graphics = m.getGraphics();
    bool graphicsHasNonZeroData = false;
    for (int i = 0; i < 2048; ++i) {
        if (graphics[i] != 0) {
            graphicsHasNonZeroData = true;
        }
    }

    CHECK(graphicsHasNonZeroData == false);
    CHECK(m.getDrawFlag() == true);
    CHECK(m.getProgramCounter() == 0x202);
}

/*
 * Program Counter should be set to the value
 * at the top of the stack
 */
TEST_CASE("test 0x00EE handler", "OpcodeTable")
{
    //Setup
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0x00);
    m.setMemoryAtAddress(0x201, 0xEE);
    m.stackPush(0x300);
    m.stackPush(0x305);
    m.fetchOpcode();

    //Run opcode handler
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    //Analyze results
    CHECK(m.getProgramCounter() == 0x305);
}

/*
 * Program Counter should be set to the value of NNN
 */
TEST_CASE("test 0x1NNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0x12);
    m.setMemoryAtAddress(0x201, 0x33);
    m.setMemoryAtAddress(0x233, 0x13);
    m.setMemoryAtAddress(0x234, 0x55);
    m.fetchOpcode();

    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x233);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());
    CHECK(m.getProgramCounter() == 0x355);
}

/*
 * Program counter prior to the opcode should be stored
 * on top of the stack, then Program counter should be
 * set to the value of NNN
 */
TEST_CASE("test 0x2NNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0x25);
    m.setMemoryAtAddress(0x201, 0xAC);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x5AC);
    CHECK(m.stackPop() == 0x200);
}

/*
 * If the value of register X is equal to NN, Program
 * Counter += 4, otherwise Program Counter += 2
 */
TEST_CASE("test 0x3XNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setRegister(0xA, 0x83);
    m.setMemoryAtAddress(0x200, 0x3A);
    m.setMemoryAtAddress(0x201, 0x00);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x202);

    m.setMemoryAtAddress(0x202, 0x3A);
    m.setMemoryAtAddress(0x203, 0x83);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x206);
}

/*
 * If the value of register X is NOT equal to NN, Program
 * Counter += 4, otherwise Program Counter += 2
 */
TEST_CASE("test 0x4XNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setRegister(0xF, 0xFF);
    m.setMemoryAtAddress(0x200, 0x4F);
    m.setMemoryAtAddress(0x201, 0x38);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x204);

    m.setMemoryAtAddress(0x204, 0x4F);
    m.setMemoryAtAddress(0x205, 0xFF);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x206);
}

/*
 * If the value of register X == the value of 
 * register Y, Program Counter += 4, else 
 * Program Counter += 2
 */
TEST_CASE("test 0x5XY0 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setRegister(0x0, 0xA3);
    m.setRegister(0xC, 0xD9);
    m.setMemoryAtAddress(0x200, 0x50);
    m.setMemoryAtAddress(0x201, 0xC0);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x202);

    m.setRegister(0xC, 0xA3);
    m.setMemoryAtAddress(0x202, 0x50);
    m.setMemoryAtAddress(0x203, 0xC0);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x206);
}

/*
 * Register X should be loaded with the value of NN
 */
TEST_CASE("test 0x6XNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0x62);
    m.setMemoryAtAddress(0x201, 0xFF);
    m.setMemoryAtAddress(0x202, 0x6E);
    m.setMemoryAtAddress(0x203, 0x01);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x2) == 0xFF);

    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xE) == 0x01);

    //check that we properly advanced PC
    CHECK(m.getProgramCounter() == 0x204);
}

/*
 * Register X should contain the sum of
 * register X's previous value and NN. Then,
 * PC should be set to 0x202
 */
TEST_CASE("test 0x7XNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0x7B);
    m.setMemoryAtAddress(0x201, 0x08);
    m.setRegister(0xB, 0xA3);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xB) == 0xAB);
    CHECK(m.getProgramCounter() == 0x202);
}

/*
 * The value of register Y should be stored into
 * register X
 */
TEST_CASE("test 0x8XY0 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0x81);
    m.setMemoryAtAddress(0x201, 0x20);
    m.setRegister(0x2, 0x9F);

    CHECK(m.getRegister(0x1) != m.getRegister(0x2));

    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x1) == 0x9F);
    CHECK(m.getProgramCounter() == 0x202);
}

/*
 * Bitwise OR register X with register Y, and register
 * X should then contain the result. Then, advance PC
 * by 2
 */
TEST_CASE("test 0x8XY1 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setRegister(0x4, 0x07);
    m.setRegister(0x5, 0xA0);
    m.setMemoryAtAddress(0x200, 0x84);
    m.setMemoryAtAddress(0x201, 0x51);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x4) == 0xA7);
    CHECK(m.getProgramCounter() == 0x202);
}

/*
 * Bitwise AND register X with register Y, and register
 * X should contain the result. Then, advance PC by 2
 */
TEST_CASE("test 0x8XY2 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setRegister(0x6, 0xB5);
    m.setRegister(0x7, 0x71);
    m.setMemoryAtAddress(0x200, 0x86);
    m.setMemoryAtAddress(0x201, 0x72);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x6) == 0x31); 
    CHECK(m.getProgramCounter() == 0x202);
}

/*
 * Bitwise XOR register X with register Y, register
 * X should contain the result. Then, advance PC by 2
 */
TEST_CASE("test 0x8XY3 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setRegister(0x8, 0xD4);
    m.setRegister(0x9, 0x37);
    m.setMemoryAtAddress(0x200, 0x88);
    m.setMemoryAtAddress(0x201, 0x93);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x8) == 0xE3);
    CHECK(m.getProgramCounter() == 0x202);
}

/*
 * If register X + register Y > 255, set register 0xF to 1,
 * else register 0xF should be set to 0. Then, register X
 * should be set to the sum of register X and register Y
 */
TEST_CASE("test 0x8XY4 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    //sum > 255
    m.setRegister(0xA, 0xFD);
    m.setRegister(0xB, 0x04);
    m.setMemoryAtAddress(0x200, 0x8A);
    m.setMemoryAtAddress(0x201, 0xB4);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xA) == 0x01);
    CHECK(m.getRegister(0xF) == 0x01);

    //sum < 255
    m.setRegister(0xC, 0x20);
    m.setRegister(0xD, 0x30);
    m.setRegister(0xF, 0xFF); //set 0xF to some junk value
    m.setMemoryAtAddress(0x202, 0x8C);
    m.setMemoryAtAddress(0x203, 0xD4);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xC) == 0x50);
    CHECK(m.getRegister(0xF) == 0x00);

    //check PC has advanced properly
    CHECK(m.getProgramCounter() == 0x204);
}

/*
 * Set register 0xF to 1 if register X > register Y,
 * else set register 0xF to 0. Then, set register X
 * to Register X - Register Y. Then, advance PC by 2
 */
TEST_CASE("test 0x8XY5 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    //X > Y
    m.setRegister(0xD, 0x25);
    m.setRegister(0x0, 0x10);
    m.setMemoryAtAddress(0x200, 0x8D);
    m.setMemoryAtAddress(0x201, 0x05);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xD) == 0x15);
    CHECK(m.getRegister(0xF) == 1);

    //X < Y
    m.setRegister(0x3, 0x25);
    m.setRegister(0x4, 0x50);
    m.setRegister(0xF, 0xFF);
    m.setMemoryAtAddress(0x202, 0x83);
    m.setMemoryAtAddress(0x203, 0x45);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x3) == 0xD5);
    CHECK(m.getRegister(0xF) == 0x00);
    
    //check PC has advanced properly
    CHECK(m.getProgramCounter() == 0x204);
}

/*
 * If the least-significant bit of register X is 1,
 * then set register 0xF to 1. Otherwise, set register
 * 0xF to 0. Then register X should be set to the value
 * of register x divided by 2
 */
TEST_CASE("test 0x8XY6 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    //test least-significant bit is 1
    m.setRegister(0x8, 0x51);
    m.setMemoryAtAddress(0x200, 0x88);
    m.setMemoryAtAddress(0x201, 0x66);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xF) == 0x01);
    CHECK(m.getRegister(0x8) == (0x51 >> 1));

    //test least-significant bit is 0
    m.setRegister(0x9, 0x50);
    m.setMemoryAtAddress(0x202, 0x89);
    m.setMemoryAtAddress(0x203, 0x06);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xF) == 0x00);
    CHECK(m.getRegister(0x9) == (0x50 >> 1));

    //check PC has advanced properly
    CHECK(m.getProgramCounter() == 0x204);
}

/*
 * If register Y > register X, set register 0xF to 1,
 * else set it to 0. Then, register X should equal
 * register Y minus register X
 */
TEST_CASE("test 0x8XY7 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    //test y > x
    m.setRegister(0x0, 0x05);
    m.setRegister(0x1, 0x25);
    m.setMemoryAtAddress(0x200, 0x80);
    m.setMemoryAtAddress(0x201, 0x17);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xF) == 1);
    CHECK(m.getRegister(0x0) == 0x20);

    //test y < x
    m.setRegister(0x2, 0x25);
    m.setRegister(0x3, 0x05);
    m.setMemoryAtAddress(0x202, 0x82);
    m.setMemoryAtAddress(0x203, 0x37);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xF) == 0);
    CHECK(m.getRegister(0x2) == (unsigned char)(0x05 - 0x25));

    //test y == x
    m.setRegister(0xF, 0xFF);
    m.setRegister(0x4, 0x25);
    m.setRegister(0x5, 0x25);
    m.setMemoryAtAddress(0x204, 0x84);
    m.setMemoryAtAddress(0x205, 0x57);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xF) == 0);
    CHECK(m.getRegister(0x4) == 0x00);

    CHECK(m.getProgramCounter() == 0x206);
}

/*
 * If the most-significant bit of register X is 1, register
 * 0xF should be set to 1. Otherwise, register 0xF should be
 * set to 0. Then, register X should contain register X * 2
 */
TEST_CASE("test 0x8XYE handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    //test most-significant bit is 1
    m.setRegister(0xE, 0xD2);
    m.setMemoryAtAddress(0x200, 0x8E);
    m.setMemoryAtAddress(0x201, 0x9E);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xF) == 1);
    CHECK(m.getRegister(0xE) == (unsigned char)(0xD2 << 1));

    //test most-significant bit is 0
    m.setRegister(0xA, 0x6E);
    m.setMemoryAtAddress(0x202, 0x8A);
    m.setMemoryAtAddress(0x203, 0xCE);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0xF) == 0);
    CHECK(m.getRegister(0xA) == (unsigned char)(0x6E << 1));

    CHECK(m.getProgramCounter() == 0x204);
}

/*
 * If register X is not equal to register Y, then
 * PC should be incremented by 4. Otherwise, it should
 * be incremented by 2 as normal
 */
TEST_CASE("test 0x9XY0 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    //test registers equal
    m.setRegister(0x0, 0xAF);
    m.setRegister(0x1, 0xAF);
    m.setMemoryAtAddress(0x200, 0x90);
    m.setMemoryAtAddress(0x201, 0x10);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x202);

    //test registers not equal
    m.setRegister(0x2, 0xBC);
    m.setRegister(0x3, 0x12);
    m.setMemoryAtAddress(0x202, 0x92);
    m.setMemoryAtAddress(0x203, 0x30);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x206);
}

/*
 * Index should contain the value NNN. We should increment PC
 * by 2 as normal
 */
TEST_CASE("test 0xANNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xA5);
    m.setMemoryAtAddress(0x201, 0x1D);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getIndex() == 0x51D);

    CHECK(m.getProgramCounter() == 0x202);
}

/*
 * Program counter should be set to the sum of register 0x0 and NNN
 */
TEST_CASE("test 0xBNNN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setRegister(0x0, 0x50);
    m.setMemoryAtAddress(0x200, 0xB3);
    m.setMemoryAtAddress(0x201, 0xFA);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getProgramCounter() == 0x44A);
}

/*
 * Random 8-bit value should be generated, bitwise ANDed with
 * NN, and stored in Vx. Hard to test currently, since we have
 * no way to predict/inject the RNG (and it doesn't really make
 * sense to set it up that way)
 */
TEST_CASE("test 0xCXNN handler", "OpcodeTable")
{
}

/*
 * Graphics array should contain an N-byte sprite beginning at coordinates
 * graphics[register X * register Y] and ending at graphics[(register X * register Y) + N].
 * The sprite written to graphics should match the sprite read from the range memory[index] to
 * memory[index + N]. If we overwrite any part of an existing sprite in graphics[], then we should
 * set register 0xF to 1. Otherwise, we should set register 0xF to 0.
 */
TEST_CASE("test 0xDXYN handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    /*
     * Set the following sprites to memory
     * 0x00-0x03
     *          *       00010000    0x10
     *         ***      00111000    0x38
     *        *****     01111100    0x7C
     *       *******    11111110    0xFE
     *
     * 0x04-0x08
     *       ********   11111111    0xFF
     *       ********   11111111    0xFF
     *       ********   11111111    0xFF
     *       ********   11111111    0xFF
     *       ********   11111111    0xFF
     *
     * 0x09 - 0x0F
     *          *       00010000    0x10
     *         ***      00111000    0x38
     *        *****     01111100    0x7C
     *       *******    11111110    0xFE
     *        *****     01111100    0x7C
     *         ***      00111000    0x38
     *          *       00010000    0x10
     */
    //Triangle
    m.setMemoryAtAddress(0x00, 0x10);
    m.setMemoryAtAddress(0x01, 0x38);
    m.setMemoryAtAddress(0x02, 0x7C);
    m.setMemoryAtAddress(0x03, 0xFE);

    //Square
    m.setMemoryAtAddress(0x04, 0xFF);
    m.setMemoryAtAddress(0x05, 0xFF);
    m.setMemoryAtAddress(0x06, 0xFF);
    m.setMemoryAtAddress(0x07, 0xFF);
    m.setMemoryAtAddress(0x08, 0xFF);

    //Diamond
    m.setMemoryAtAddress(0x09, 0x10);
    m.setMemoryAtAddress(0x0A, 0x38);
    m.setMemoryAtAddress(0x0B, 0x7C);
    m.setMemoryAtAddress(0x0C, 0xFE);
    m.setMemoryAtAddress(0x0D, 0x7C);
    m.setMemoryAtAddress(0x0E, 0x38);
    m.setMemoryAtAddress(0x0F, 0x10);

    SECTION("Sets sprite to proper position in graphics memory") {
        //set the coordinate position for the sprite
        m.setRegister(0x1, 10);
        m.setRegister(0x2, 20);

        //set the opcode to draw this sprite into location (register1, register2)
        m.setMemoryAtAddress(0x200, 0xD1);
        m.setMemoryAtAddress(0x201, 0x24);
        m.fetchOpcode();

        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        //check that this sprite was rendered properly in graphics memory
        bool spriteFirstRow[8]  = {0,0,0,1,0,0,0,0};
        bool spriteSecondRow[8] = {0,0,1,1,1,0,0,0};
        bool spriteThirdRow[8]  = {0,1,1,1,1,1,0,0};
        bool spriteFourthRow[8] = {1,1,1,1,1,1,1,0};
        bool firstRowOfSpriteRenderedCorrectly = true;
        bool secondRowOfSpriteRenderedCorrectly = true;
        bool thirdRowOfSpriteRenderedCorrectly = true;
        bool fourthRowOfSpriteRenderedCorrectly = true;
        unsigned char *graphics = m.getGraphics();

        // printGraphicsArrayToConsole(graphics);

        for (int i = 0; i < 8; ++i) {
            if (graphics[1290 + i] != spriteFirstRow[i]) {
                firstRowOfSpriteRenderedCorrectly = false;
            }

            if (graphics[1354 + i] != spriteSecondRow[i]) {
                secondRowOfSpriteRenderedCorrectly = false;
            }

            if (graphics[1418 + i] != spriteThirdRow[i]) {
                thirdRowOfSpriteRenderedCorrectly = false;
            }

            if (graphics[1482 + i] != spriteFourthRow[i]) {
                fourthRowOfSpriteRenderedCorrectly = false;
            }
        }

        CHECK(firstRowOfSpriteRenderedCorrectly == true);
        CHECK(secondRowOfSpriteRenderedCorrectly == true);
        CHECK(thirdRowOfSpriteRenderedCorrectly == true);
        CHECK(fourthRowOfSpriteRenderedCorrectly == true);

    }

    SECTION("Sets collision detection flag when sprites collide") {
        //Draw triangle
        m.setRegister(0x0, 5);
        m.setRegister(0x1, 10);
        m.setIndex(0x00);
        m.setMemoryAtAddress(0x200, 0xD0);
        m.setMemoryAtAddress(0x201, 0x14);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        //Draw square
        m.setRegister(0x0, 25);
        m.setRegister(0x1, 15);
        m.setIndex(0x04);
        m.setMemoryAtAddress(0x202, 0xD0);
        m.setMemoryAtAddress(0x203, 0x15);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        //Draw diamond
        m.setRegister(0x0, 15);
        m.setRegister(0x1, 0);
        m.setIndex(0x09);
        m.setMemoryAtAddress(0x204, 0xD0);
        m.setMemoryAtAddress(0x205, 0x17);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        // printGraphicsArrayToConsole(m.getGraphics());

        //Check that collision detection flag not set
        //(the sprites rendered above should not actually overlap)
        CHECK(m.getRegister(0xF) == 0);

        //Now, test collision flag is set when sprites are colliding
        m.flushGraphics();

        //draw triangle
        m.setRegister(0x0, 30);
        m.setRegister(0x1, 10);
        m.setIndex(0x00);
        m.setMemoryAtAddress(0x206, 0xD0);
        m.setMemoryAtAddress(0x207, 0x14);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        //draw square overlapping part of triangle
        m.setRegister(0x0, 26);
        m.setRegister(0x1, 10);
        m.setIndex(0x04);
        m.setMemoryAtAddress(0x208, 0xD0);
        m.setMemoryAtAddress(0x209, 0x15);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        // printGraphicsArrayToConsole(m.getGraphics());

        CHECK(m.getRegister(0xF) == 1);
    }

    SECTION("Sets draw flag to true when called") {
        m.setRegister(0x0, 0x00);
        m.setRegister(0x1, 0x00);
        m.setIndex(0x00);
        m.setMemoryAtAddress(0x200, 0xD0);
        m.setMemoryAtAddress(0x201, 0x11);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        CHECK(m.getDrawFlag() == true);
    }

    SECTION("Increments program counter as expected") {
        m.setRegister(0x0, 0x00);
        m.setRegister(0x1, 0x00);
        m.setIndex(0x00);
        m.setMemoryAtAddress(0x200, 0xD0);
        m.setMemoryAtAddress(0x201, 0x11);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

        CHECK(m.getProgramCounter() == 0x202);
    }

    SECTION("Sets sprite properly when sprite travels outside window bounds in x-direction") {
        m.setRegister(0x0, 60);
        m.setRegister(0x1, 0);
        m.setIndex(0x04);
        m.setMemoryAtAddress(0x200, 0xD0);
        m.setMemoryAtAddress(0x201, 0x15);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());
        unsigned char *graphics = m.getGraphics();

        //check the whole shape, pixel by pixel
        for (int row = 0; row < 5; ++row) {
            CHECK(graphics[(row * 64) + 0] == 1);
            CHECK(graphics[(row * 64) + 1] == 1);
            CHECK(graphics[(row * 64) + 2] == 1);
            CHECK(graphics[(row * 64) + 3] == 1);
            CHECK(graphics[(row * 64) + 60] == 1);
            CHECK(graphics[(row * 64) + 61] == 1);
            CHECK(graphics[(row * 64) + 62] == 1);
            CHECK(graphics[(row * 64) + 63] == 1);
        }
        // prettyPrintGraphicsArrayToConsole(m.getGraphics());
    }

    SECTION("Sets sprite properly when sprite travels outside window bounds in y-direction") {
        m.setRegister(0x0, 0);
        m.setRegister(0x1, 30);
        m.setIndex(0x04);
        m.setMemoryAtAddress(0x200, 0xD0);
        m.setMemoryAtAddress(0x201, 0x15);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());
        unsigned char *graphics = m.getGraphics();

        for (int row = 0; row < 3; ++row) {
            CHECK(graphics[(row * 64) + 0] == 1);
            CHECK(graphics[(row * 64) + 1] == 1);
            CHECK(graphics[(row * 64) + 2] == 1);
            CHECK(graphics[(row * 64) + 3] == 1);
            CHECK(graphics[(row * 64) + 4] == 1);
            CHECK(graphics[(row * 64) + 5] == 1);
            CHECK(graphics[(row * 64) + 6] == 1);
            CHECK(graphics[(row * 64) + 7] == 1);
        }
        
        for (int row = 30; row < 32; ++row) {
            CHECK(graphics[(row * 64) + 0] == 1);
            CHECK(graphics[(row * 64) + 1] == 1);
            CHECK(graphics[(row * 64) + 2] == 1);
            CHECK(graphics[(row * 64) + 3] == 1);
            CHECK(graphics[(row * 64) + 4] == 1);
            CHECK(graphics[(row * 64) + 5] == 1);
            CHECK(graphics[(row * 64) + 6] == 1);
            CHECK(graphics[(row * 64) + 7] == 1);
        }

        // prettyPrintGraphicsArrayToConsole(m.getGraphics());
    }

    SECTION("Sets sprite properly when sprite travels outside window bounds in x and y directions") {
        m.setRegister(0x0, 60);
        m.setRegister(0x1, 30);
        m.setIndex(0x04);
        m.setMemoryAtAddress(0x200, 0xD0);
        m.setMemoryAtAddress(0x201, 0x15);
        m.fetchOpcode();
        opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());
        unsigned char *graphics = m.getGraphics();

        for (int row = 0; row < 3; ++row) {
            CHECK(graphics[(row * 64) + 0] == 1);
            CHECK(graphics[(row * 64) + 1] == 1);
            CHECK(graphics[(row * 64) + 2] == 1);
            CHECK(graphics[(row * 64) + 3] == 1);
            CHECK(graphics[(row * 64) + 60] == 1);
            CHECK(graphics[(row * 64) + 61] == 1);
            CHECK(graphics[(row * 64) + 62] == 1);
            CHECK(graphics[(row * 64) + 63] == 1);
        }
        
        for (int row = 30; row < 32; ++row) {
            CHECK(graphics[(row * 64) + 0] == 1);
            CHECK(graphics[(row * 64) + 1] == 1);
            CHECK(graphics[(row * 64) + 2] == 1);
            CHECK(graphics[(row * 64) + 3] == 1);
            CHECK(graphics[(row * 64) + 60] == 1);
            CHECK(graphics[(row * 64) + 61] == 1);
            CHECK(graphics[(row * 64) + 62] == 1);
            CHECK(graphics[(row * 64) + 63] == 1);
        }

        // prettyPrintGraphicsArrayToConsole(m.getGraphics());
    }
}


TEST_CASE("test 0xEX9E handler", "OpcodeTable")
{
    // Keypad state with "4" button on keyboard pressed
    unsigned char keypadState[16] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xE8);
    m.setMemoryAtAddress(0x201, 0x9E);
    m.setRegister(0x8, 0x3);
    m.setKeypadState(keypadState);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    //Check that next instruction was skipped (if it wasn't, programCounter would come back with 0x202)
    CHECK(m.getProgramCounter() == 0x204);

    m.setMemoryAtAddress(0x204, 0xE9);
    m.setMemoryAtAddress(0x205, 0x9E);
    m.setRegister(0x9, 0);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    //Check that next instruction was not skipped
    CHECK(m.getProgramCounter() == 0x206);
}

TEST_CASE("test 0xEXA1 handler", "OpcodeTable")
{
    // Keypad state with no buttons pressed 
    unsigned char keypadState[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xE8);
    m.setMemoryAtAddress(0x201, 0xA1);
    m.setRegister(0x8, 0x3);
    m.setKeypadState(keypadState);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    //Check that next instruction was skipped (if it wasn't, programCounter would come back with 0x202)
    CHECK(m.getProgramCounter() == 0x204);

    //alter keyState to show that the "4" key is pressed
    keypadState[3] = 1;
    m.setMemoryAtAddress(0x204, 0xE9);
    m.setMemoryAtAddress(0x205, 0xA1);
    m.setKeypadState(keypadState);
    m.setRegister(0x9, 3);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    //Check that next instruction was not skipped
    CHECK(m.getProgramCounter() == 0x206);
}

//Register X should contain the value of the delay timer
TEST_CASE("test 0xFX07 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xF3);
    m.setMemoryAtAddress(0x201, 0x07);
    m.setDelayTimer(0x5A);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x3) == 0x5A);
    CHECK(m.getProgramCounter() == 0x202);
}

//Program should wait for user input, then store the pressed key value in register X
TEST_CASE("test 0xFX0A handler", "OpcodeTable")
{
    //this handler not yet implemented
}

//Delay timer should be set to register X
TEST_CASE("test 0xFX15 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xF2);
    m.setMemoryAtAddress(0x201, 0x15);
    m.setRegister(2, 0x05);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getDelayTimer() == 0x05);
    CHECK(m.getProgramCounter() == 0x202);
}

//Sound timer should contain the value of register X
TEST_CASE("test 0xFX18 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xFB);
    m.setMemoryAtAddress(0x201, 0x18);
    m.setRegister(0xB, 0xB0);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getSoundTimer() == 0xB0);
    CHECK(m.getProgramCounter() == 0x202);
}

//Index should contain the sum of Index and register X
TEST_CASE("test 0xFX1E handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xF9);
    m.setMemoryAtAddress(0x201, 0x1E);
    m.setIndex(0xFCB);
    m.setRegister(0x9, 0x03);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getIndex() == 0xFCE);
    CHECK(m.getProgramCounter() == 0x202);
}

//Index should contain the location of the sprite
//for the digit specified by register X
TEST_CASE("test 0xFX29 handler", "OpcodeTable")
{
    //this handler not yet implemented
}

//Memory locations Index, Index+1, and Index+2 should contain
//the Binary-Coded Decimal representation of the value held in
//register X
TEST_CASE("test 0xFX33 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xF0);
    m.setMemoryAtAddress(0x201, 0x33);
    m.setIndex(0xBC3);
    m.setRegister(0x0, 135);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getMemoryAtAddress(0xBC3) == 1);
    CHECK(m.getMemoryAtAddress(0xBC4) == 3);
    CHECK(m.getMemoryAtAddress(0xBC5) == 5);
    CHECK(m.getProgramCounter() == 0x202);
}

//Registers 0 - X should be stored into memory, starting
//at the location pointed to by Index
TEST_CASE("test 0xFX55 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xFF);
    m.setMemoryAtAddress(0x201, 0x55);
    m.setIndex(0xA20);
    m.setRegister(0x0, 0x0F);
    m.setRegister(0x1, 0x0E);
    m.setRegister(0x2, 0x0D);
    m.setRegister(0x3, 0x0C);
    m.setRegister(0x4, 0x0B);
    m.setRegister(0x5, 0x0A);
    m.setRegister(0x6, 0x09);
    m.setRegister(0x7, 0x08);
    m.setRegister(0x8, 0x07);
    m.setRegister(0x9, 0x06);
    m.setRegister(0xA, 0x05);
    m.setRegister(0xB, 0x04);
    m.setRegister(0xC, 0x03);
    m.setRegister(0xD, 0x02);
    m.setRegister(0xE, 0x01);
    m.setRegister(0xF, 0xFF);
    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getMemoryAtAddress(0xA20) == 0x0F);
    CHECK(m.getMemoryAtAddress(0xA21) == 0x0E);
    CHECK(m.getMemoryAtAddress(0xA22) == 0x0D);
    CHECK(m.getMemoryAtAddress(0xA23) == 0x0C);
    CHECK(m.getMemoryAtAddress(0xA24) == 0x0B);
    CHECK(m.getMemoryAtAddress(0xA25) == 0x0A);
    CHECK(m.getMemoryAtAddress(0xA26) == 0x09);
    CHECK(m.getMemoryAtAddress(0xA27) == 0x08);
    CHECK(m.getMemoryAtAddress(0xA28) == 0x07);
    CHECK(m.getMemoryAtAddress(0xA29) == 0x06);
    CHECK(m.getMemoryAtAddress(0xA2A) == 0x05);
    CHECK(m.getMemoryAtAddress(0xA2B) == 0x04);
    CHECK(m.getMemoryAtAddress(0xA2C) == 0x03);
    CHECK(m.getMemoryAtAddress(0xA2D) == 0x02);
    CHECK(m.getMemoryAtAddress(0xA2E) == 0x01);
    CHECK(m.getMemoryAtAddress(0xA2F) == 0xFF);

    CHECK(m.getProgramCounter() == 0x202);
}

//Starting at the memory location pointed to by Index,
//store those values into registers 0 - X
TEST_CASE("test 0xFX65 handler", "OpcodeTable")
{
    Memory m;
    OpcodeTable opcodeTable(m);
    std::vector<unsigned char> dummyFileContents;
    dummyFileContents.resize(0xDFF, 0);
    m.initialize(dummyFileContents);

    m.setMemoryAtAddress(0x200, 0xFF);
    m.setMemoryAtAddress(0x201, 0x65);
    m.setIndex(0xB20);

    m.setMemoryAtAddress(0xB20, 0x0F);
    m.setMemoryAtAddress(0xB21, 0x0E);
    m.setMemoryAtAddress(0xB22, 0x0D);
    m.setMemoryAtAddress(0xB23, 0x0C);
    m.setMemoryAtAddress(0xB24, 0x0B);
    m.setMemoryAtAddress(0xB25, 0x0A);
    m.setMemoryAtAddress(0xB26, 0x09);
    m.setMemoryAtAddress(0xB27, 0x08);
    m.setMemoryAtAddress(0xB28, 0x07);
    m.setMemoryAtAddress(0xB29, 0x06);
    m.setMemoryAtAddress(0xB2A, 0x05);
    m.setMemoryAtAddress(0xB2B, 0x04);
    m.setMemoryAtAddress(0xB2C, 0x03);
    m.setMemoryAtAddress(0xB2D, 0x02);
    m.setMemoryAtAddress(0xB2E, 0x01);
    m.setMemoryAtAddress(0xB2F, 0xFF);

    m.fetchOpcode();
    opcodeTable.decodeAndExecuteOpcode(m.getCurrentOpcode());

    CHECK(m.getRegister(0x0) == 0x0F);
    CHECK(m.getRegister(0x1) == 0x0E);
    CHECK(m.getRegister(0x2) == 0x0D);
    CHECK(m.getRegister(0x3) == 0x0C);
    CHECK(m.getRegister(0x4) == 0x0B);
    CHECK(m.getRegister(0x5) == 0x0A);
    CHECK(m.getRegister(0x6) == 0x09);
    CHECK(m.getRegister(0x7) == 0x08);
    CHECK(m.getRegister(0x8) == 0x07);
    CHECK(m.getRegister(0x9) == 0x06);
    CHECK(m.getRegister(0xA) == 0x05);
    CHECK(m.getRegister(0xB) == 0x04);
    CHECK(m.getRegister(0xC) == 0x03);
    CHECK(m.getRegister(0xD) == 0x02);
    CHECK(m.getRegister(0xE) == 0x01);
    CHECK(m.getRegister(0xF) == 0xFF);

    CHECK(m.getProgramCounter() == 0x202);
}
