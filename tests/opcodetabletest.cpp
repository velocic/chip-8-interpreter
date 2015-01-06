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
