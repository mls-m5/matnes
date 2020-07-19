
#include "cpu.h"
#include "mls-unit-test/unittest.h"
#include "staticmessage.h"

using namespace matnes::cpu;

TEST_SUIT_BEGIN

TEST_CASE("disableInterrupts") {
    constexpr auto cpu = [](bool value) {
        Cpu cpu;
        cpu.disableInterupts(value);

        return cpu;
    };

    static_assert(cpu(true).disableInterupts(), "STA");
    static_assert(!cpu(false).disableInterupts(), "STA");
}

TEST_CASE("stack") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;

        cpu.push(value);

        return cpu;
    };

    static_assert(cpu(10).pull() == 10, "stack");
}

TEST_CASE("double stack") {
    constexpr auto cpu = [](uint16_t value) {
        Cpu cpu;

        cpu.pushBig(value);

        cpu.push(10);

        cpu.pull();

        return cpu;
    };

    static_assert(cpu(1012).pullBig() == 1012, "stack");
}

TEST_CASE("stackBig") {
    constexpr auto cpu = [](uint16_t value) {
        Cpu cpu;

        cpu.pushBig(value);

        return cpu;
    };

    static_assert(cpu(1010).pullBig() == 1010, "stack");
}

TEST_SUIT_END
