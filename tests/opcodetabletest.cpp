#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <memory.h>
#include <opcodetable.h>

/*
 * Graphics should be blanked out, draw flag should be set,
 * and we should advance to the next instruction
 */
TEST_CASE("Test 0x00E0 handler", "OpcodeTable")
{ 
    Memory m;
    OpcodeTable opcodes(m);

    m.setMemoryAtAddress(0x200, 0x00);
    m.setMemoryAtAddress(0x201, 0xE0);
}
