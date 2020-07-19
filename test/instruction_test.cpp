
#include "cpu.h"
#include "mls-unit-test/unittest.h"
#include "staticmessage.h"

using namespace matnes::cpu;

// Note that most of theese tests will fail at compile time and not at runtime

TEST_SUIT_BEGIN

//    constexpr auto value = cpu(0b1).dummy();
//    StaticMessage<value>();

TEST_CASE("BCC, BCS, BEQ, BMI, BNE, BPL, BVC, BVS") {
    //    typedef void (Cpu::*StatusFunctionType)(bool);
    typedef decltype(&Cpu::BCC) FunctionType;

    constexpr auto start = Cpu().PC();

    constexpr auto cpu = [](FunctionType f, uint8_t status, uint8_t newAddres) {
        Cpu cpu;

        cpu.status(status);
        (cpu.*f)(newAddres);

        return cpu;
    };

    static_assert(cpu(&Cpu::BCC, uint8_t(0), 10).PC() == start + 10, "BCC");
    static_assert(cpu(&Cpu::BCC, ~uint8_t(0), 10).PC() == 0, "BCC");

    static_assert(cpu(&Cpu::BCS, ~uint8_t(0), 10).PC() == start + 10, "BCS");
    static_assert(cpu(&Cpu::BCS, uint8_t(0), 10).PC() == 0, "BCS");

    static_assert(cpu(&Cpu::BEQ, ~uint8_t(0), 10).PC() == start + 10, "BEQ");
    static_assert(cpu(&Cpu::BEQ, uint8_t(0), 10).PC() == 0, "BEQ");

    static_assert(cpu(&Cpu::BMI, ~uint8_t(0), 10).PC() == start + 10, "BMI");
    static_assert(cpu(&Cpu::BMI, uint8_t(0), 10).PC() == 0, "BMI");

    static_assert(cpu(&Cpu::BNE, uint8_t(0), 10).PC() == start + 10, "BNE");
    static_assert(cpu(&Cpu::BNE, ~uint8_t(0), 10).PC() == 0, "BNE");

    static_assert(cpu(&Cpu::BPL, uint8_t(0), 10).PC() == start + 10, "BPL");
    static_assert(cpu(&Cpu::BPL, ~uint8_t(0), 10).PC() == 0, "BPL");

    static_assert(cpu(&Cpu::BVC, uint8_t(0), 10).PC() == start + 10, "BVC");
    static_assert(cpu(&Cpu::BVC, ~uint8_t(0), 10).PC() == 0, "BVC");

    static_assert(cpu(&Cpu::BVS, ~uint8_t(0), 10).PC() == start + 10, "BVS");
    static_assert(cpu(&Cpu::BVS, uint8_t(0), 10).PC() == 0, "BVS");
}

TEST_CASE("CLC, CLD, CLI, CLV") {
    typedef decltype(&Cpu::CLC) FunctionType;

    constexpr auto cpu = [](FunctionType f) {
        Cpu cpu;
        cpu.status(~0);
        (cpu.*f)(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(&Cpu::CLC).carry() == false, "CLC");
    static_assert(cpu(&Cpu::CLD).decimalFlag() == false, "CLD");
    static_assert(cpu(&Cpu::CLI).disableInterupts() == false, "CLI");
    static_assert(cpu(&Cpu::CLV).overflowFlag() == false, "CLV");
}

TEST_CASE("CMP") {
    constexpr auto cpu = [](uint8_t accumulator, uint8_t value) {
        Cpu cpu;
        cpu.A(accumulator);
        cpu.CMP(value);
        return cpu;
    };

    static_assert(cpu(10, 10).zero(), "CMP");
    static_assert(cpu(10, 11).zero() == false, "CMP");
}

TEST_CASE("CPX") {
    constexpr auto cpu = [](uint8_t accumulator, uint8_t value) {
        Cpu cpu;
        cpu.X(accumulator);
        cpu.CPX(value);
        return cpu;
    };

    static_assert(cpu(10, 10).zero(), "CPX");
    static_assert(cpu(10, 11).zero() == false, "CPX");
}

TEST_CASE("CPY") {
    constexpr auto cpu = [](uint8_t accumulator, uint8_t value) {
        Cpu cpu;
        cpu.Y(accumulator);
        cpu.CPY(value);
        return cpu;
    };

    static_assert(cpu(10, 10).zero(), "CPY");
    static_assert(cpu(10, 11).zero() == false, "CPY");
}

TEST_CASE("DEC") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.DEC(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(121).dummy() == 120, "DEX");
}

TEST_CASE("DEX") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.X(value);
        cpu.DEX(value);
        return cpu;
    };

    static_assert(cpu(121).X() == 120, "DEX");
}

TEST_CASE("DEY") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.Y(value);
        cpu.DEY(value);
        return cpu;
    };

    static_assert(cpu(121).Y() == 120, "DEY");
}

TEST_CASE("EOR") {
    constexpr auto cpu = [](uint8_t accumulator, uint8_t value) {
        Cpu cpu;
        cpu.A(accumulator);
        cpu.EOR(value);
        return cpu;
    };

    static_assert(cpu(123, 121).A() == (123 ^ 121), "EOR");
}

TEST_CASE("INC") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.INC(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(101).dummy() == 102, "INC");
}

TEST_CASE("INX") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.X(value);
        cpu.INX(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(101).X() == 102, "INX");
}

TEST_CASE("INY") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.Y(value);
        cpu.INY(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(101).Y() == 102, "INY");
}

TEST_CASE("JMP") {
    constexpr auto cpu = [](uint16_t value) {
        Cpu cpu;
        // continue here this does not work...
        cpu.srcBig(value); // Set operators
        cpu.JMP(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(1010).PC() == 1010, "JMP");
}

TEST_CASE("JSR") {
    constexpr auto cpu = [](uint16_t value) {
        Cpu cpu;
        cpu.srcBig(value); // Set operators
        cpu.JSR(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(1010).PC() == 1010, "JSR");
}

TEST_CASE("LDA") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.LDA(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(10).A() == 10, "LDA");
}

TEST_CASE("LDX") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.LDX(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(10).X() == 10, "LDX");
}

TEST_CASE("LDY") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.LDY(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(10).Y() == 10, "LDY");
}

TEST_CASE("LSR") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.LSR(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(10).dummy() == (10 >> 1), "LSR");
    static_assert(cpu(102).dummy() == (102 >> 1), "LSR");
    static_assert(cpu(1).dummy() == 0, "LSR");
}

TEST_CASE("NOP") {
    // What to test?
}

TEST_CASE("ORA") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.push(value);
        cpu.PLA(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(10).A() == 10, "ORA");
}

TEST_CASE("PLA") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.push(value);
        cpu.PLA(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(10).A() == 10, "PLA");
}

TEST_CASE("PLP") {
    constexpr auto cpu = [](uint8_t value) {
        Cpu cpu;
        cpu.push(value);
        cpu.PLP(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(10).status() == 10, "PLP");
}

TEST_CASE("ROL") {
    constexpr auto cpu = [](uint8_t value, bool carry) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.carry(carry);
        cpu.ROL(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(0b101, 0).dummy() == 0b1010, "ROL");
    static_assert(cpu(0b101, 0).carry() == false, "ROL");

    static_assert(cpu(0b100, 0).dummy() == 0b1000, "ROL");
    static_assert(cpu(0b100, 0).carry() == false, "ROL");

    static_assert(cpu(0b1, 0).dummy() == 0b10, "ROL");
    static_assert(cpu(0b1, 0).carry() == false, "ROL");

    static_assert(cpu(0b0, 1).dummy() == (1), "ROL");
    static_assert(cpu(0b0, 1).carry() == false, "ROL");
}

TEST_CASE("ROR") {
    constexpr auto cpu = [](uint8_t value, bool carry) {
        Cpu cpu;
        cpu.dummy(value);
        cpu.carry(carry);
        cpu.ROR(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(0b101, 0).dummy() == 0b10, "ROR");
    static_assert(cpu(0b101, 0).carry() == true, "ROR");

    static_assert(cpu(0b100, 0).dummy() == 0b10, "ROR");
    static_assert(cpu(0b100, 0).carry() == false, "ROR");

    static_assert(cpu(0b1, 0).dummy() == 0, "ROR");
    static_assert(cpu(0b1, 0).carry() == true, "ROR");

    static_assert(cpu(0b0, 1).dummy() == (1 << 7), "ROR");
    static_assert(cpu(0b0, 1).carry() == false, "ROR");
}

TEST_CASE("RTI") {
    constexpr auto cpu = [](uint16_t stackValue, uint8_t status) {
        Cpu cpu;
        cpu.pushBig(stackValue);
        cpu.push(status);
        cpu.RTI(cpu.dummy());
        return cpu;
    };

    static_assert(cpu(1048, 10).PC() == 1048, "RTI");
    static_assert(cpu(1048, 10).status() == 10, "RTI");
}

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
