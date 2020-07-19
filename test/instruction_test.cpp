
#include "cpu.h"
#include "mls-unit-test/unittest.h"
#include "staticmessage.h"

// Note that most of theese tests will fail at compile time and not at runtime

TEST_SUIT_BEGIN

// TEST_CASE("RTI") {
//    constexpr auto cpu = [](uint8_t stackValue) {
//        Cpu cpu;
//        cpu.pushBig(stackValue);
//        cpu.push(20);
//        cpu.RTI(cpu.dummy());
//        return cpu;
//    };

//    constexpr auto value = cpu(10).PC();

//    StaticMessage<value> m;
//    static_assert(cpu(10).PC() == 10, "RTI");
//}

TEST_CASE("RTS") {
    constexpr auto cpu = [](uint8_t stackValue) {
        Cpu cpu;
        cpu.push(stackValue);
        cpu.RTS(cpu.dummy());
        return cpu;
    };
    static_assert(cpu(10).PC() == 11, "RTS");
}

TEST_CASE("SBC") {
    constexpr auto cpu = [](uint8_t accumulator, bool carry, uint8_t value) {
        Cpu cpu;
        cpu.A(accumulator);
        cpu.carry(carry);
        cpu.SBC(value);
        return cpu;
    };
    static_assert(cpu(10, 0, 0).A() == 10, "SBC");
    static_assert(cpu(10, 0, 1).A() == 9, "SBC");
    static_assert(cpu(10, 0, 6).A() == 4, "SBC");
    static_assert(cpu(10, 1, 0).A() == 9, "SBC");
}

TEST_CASE("SED") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.SED(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().decimalFlag(), "SED");
}

TEST_CASE("SEI") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.SEI(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().disableInterupts(), "SEI");
}

TEST_CASE("STA") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.A(35);
        cpu.STA(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().dummy() == 35, "STA");
}

TEST_CASE("STX") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.X(35);
        cpu.STX(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().dummy() == 35, "STX");
}

TEST_CASE("STY") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.Y(35);
        cpu.STY(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().dummy() == 35, "STY");
}

TEST_CASE("TAX") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.A(35);
        cpu.TAX(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().X() == 35, "TAX");
}

TEST_CASE("TAY") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.A(35);
        cpu.TAY(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().Y() == 35, "TAY");
}

TEST_CASE("TSX") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.stack(35);
        cpu.TSX(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().X() == 35, "TSX");
}

TEST_CASE("TXA") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.X(35);
        cpu.TXA(cpu.dummy());
        return cpu;
    };
    static_assert(cpu().A() == 35, "TXA");
}

TEST_CASE("TXS") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.X(35);
        cpu.TXS(cpu.dummy());
        return cpu;
    };

    static_assert(cpu().stack() == 35, "TXS");
}

TEST_CASE("TYA") {
    constexpr auto cpu = []() {
        Cpu cpu;
        cpu.Y(42);
        cpu.TYA(cpu.dummy());
        return cpu;
    };

    static_assert(cpu().Y() == 42, "TYA");
}

TEST_SUIT_END
