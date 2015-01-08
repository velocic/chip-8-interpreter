#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <memory.h>
#include <opcodetable.h>
#include <iostream>

/*
 * Graphics should be blanked out, draw flag should be set,
 * and we should advance to the next instruction
 */
TEST_CASE("Test 0x00E0 handler", "OpcodeTable")
{ 
    //Setup
    Memory m;
    OpcodeTable opcodeTable(m);

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
    SECTION("Sets sprite to proper position in graphics memory") {
    }

    SECTION("Sets collision detection flag when sprites collide") {
    }

    SECTION("Sets draw flag to true when called") {
    }

    SECTION("Increments program counter as expected") {
    }

    SECTION("Sets sprite properly when sprite travels outside window bounds") {
    }
}
