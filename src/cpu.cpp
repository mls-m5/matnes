#include "cpu.h"

#include "instruction-mode.h"

#include <functional>
#include <vector>

using namespace matnes::cpu;

namespace {

// Function, instruction length

struct Operation {
    uint8_t size;
    uint8_t cycles;
    void (*op)(Cpu &, uint8_t);
    uint8_t (*addressMode)(const Cpu &);
};

// void AND(Cpu &cpu, uint8_t data1, uint8_t data2 = 0) {
//    auto data = cpu.ram(data1, data2);
//    cpu.A(cpu.A() && data);
//}

////! Add to accumulator and set flags
// constexpr void ADC(Cpu &cpu, uint8_t value) {
//    auto tmp = static_cast<uint16_t>(cpu.A()) + value;
//    if (tmp > 255) {
//    }
//    cpu.A() += value;
//}

// using FunctionType =  std::function<void(Cpu &, uint8_t)>;
using FunctionType = void (*)(Cpu &, uint8_t);

// uint8_t &accumulator(Cpu &cpu) {
//    return cpu.A();
//}

// uint8_t absolute(const Cpu &cpu) {
//    return cpu.ram(cpu.src(), cpu.src2());
//}

// uint8_t absoluteZ(const Cpu &cpu) {
//    return cpu.ram(cpu.src());
//}

// struct Absolute {
//    void operator()(Cpu &cpu, uint8_t *data) {
//        f(cpu, cpu.ram(data[1], data[2]));
//    }

//    FunctionType f;
//};

struct AbsoluteZeroPage {
    void operator()(Cpu &cpu, uint8_t *data) {
        f(cpu, cpu.ram(data[1]));
    }

    FunctionType f;
};

struct Implied {
    void operator()(Cpu &cpu, uint8_t *) {
        f(cpu, 0);
    }

    FunctionType f;
};

struct AccumulatorValue {
    void operator()(Cpu &cpu, uint8_t *) {
        f(cpu, cpu.A());
    }

    FunctionType f;
};

struct Indirect {
    Indirect(FunctionType f) : f(f) {
    }

    void operator()(Cpu &cpu, uint8_t *data) {
        //        f(cpu, cpu.ram(data[1], data[2]));
        // todo: fix this
    }

    FunctionType f;
};

// void initOperations() {

//    auto &op = operations;

//    Operation o;
//    o.op = ADC;
//    o.addressMode = absolute;

//    op[0x69] = {2, 2, ADC, immediate};
//    op[0x65] = {2, 3, ADC, absolute}; // Gör så här
//    //...
//    op[0x60] = {2, 4, Absolute{ADC}};

//    // AND
//    op[0x29] = {2, 4, Immediate{ADC}};
//    op[0x25] = {2, 3, AbsoluteZeroPage{ADC}};
//    //..
//    op[0x20] = {2, 4, AbsoluteZeroPage{ADC}};
//}

// void step() {
//    Cpu cpu;

//    {
//        auto instruction = cpu.instruction();

//        auto info = instructionModes[instruction];

//        //        auto src =

//        //        auto src = op.addressMode(cpu);
//        //        op.op(cpu, src);
//    }
//}

} // namespace
