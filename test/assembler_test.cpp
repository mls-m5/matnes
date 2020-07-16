

#define DO_NOT_CATCH_ERRORS

#include "assembler.h"
#include "mls-unit-test/unittest.h"
#include <vector>

#include <regex>

TEST_SUIT_BEGIN

TEST_CASE("Immediate mode") {
    ASSERT_EQ(translateAssemblyLine("ADC #$AB"), "\xa9\xab");
}

TEST_SUIT_END
