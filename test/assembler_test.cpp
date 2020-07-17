

#define DO_NOT_CATCH_ERRORS

#include "assembler.h"
#include "mls-unit-test/unittest.h"
#include <vector>

#include <regex>

TEST_SUIT_BEGIN

// http://nesdev.com/6502.txt

TEST_CASE("ADC") {
    ASSERT_EQ(translateAssemblyLine("ADC #$AB"), "\x69\xab");
    ASSERT_EQ(translateAssemblyLine("ADC $AB"), "\x65\xab");
    ASSERT_EQ(translateAssemblyLine("ADC $ABCD"), "\x60\xab\xcd");
}

TEST_CASE("LDA") {
    ASSERT_EQ(translateAssemblyLine("LDA #$AB"), "\xa9\xab");
}

TEST_SUIT_END
