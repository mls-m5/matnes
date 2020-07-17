#include "cpu.h"

#include <functional>
#include <vector>

namespace {

// Function, instruction length

struct Operation {
    uint8_t size;
    uint8_t cycles;
    std::function<void(Cpu &cpu, uint8_t *data)> f;
};

std::array<Operation, 255> operations;

void AND(Cpu &cpu, uint8_t data1, uint8_t data2 = 0) {
    auto data = cpu.ram(data1, data2);
    cpu.A(cpu.A() && data);
}

//! Add to accumulator and set flags
constexpr void ADC(Cpu &cpu, uint8_t value) {
    auto tmp = static_cast<uint16_t>(cpu.A()) + value;
    if (tmp > 255) {
    }
    cpu.A() += value;
}

// using FunctionType =  std::function<void(Cpu &, uint8_t)>;
using FunctionType = void (*)(Cpu &, uint8_t);

struct Immediate {
    void operator()(Cpu &cpu, uint8_t *data) {
        f(cpu, data[1]);
    }

    FunctionType f;
};

struct Absolute {
    void operator()(Cpu &cpu, uint8_t *data) {
        f(cpu, cpu.ram(data[1], data[2]));
    }

    FunctionType f;
};

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

void initOperations() {

    auto &op = operations;

    op[0x69] = {2, 2, Immediate{ADC}};
    op[0x65] = {2, 3, AbsoluteZeroPage{ADC}};
    //...
    op[0x60] = {2, 4, Absolute{ADC}};

    // AND
    op[0x29] = {2, 4, Immediate{ADC}};
    op[0x25] = {2, 3, AbsoluteZeroPage{ADC}};
    //..
    op[0x20] = {2, 4, AbsoluteZeroPage{ADC}};
}

} // namespace
