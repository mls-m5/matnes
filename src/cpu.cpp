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

void initOperations() {

    auto &op = operations;
    // ADC
    op[0x69] = {2, 2, [](Cpu &cpu, uint8_t *data) {
                    ADC(cpu, data[1]);
                }};
    op[0x65] = {2, 4, [](Cpu &cpu, uint8_t *data) {
                    ADC(cpu, cpu.ram(data[1]));
                }};
    //...
    op[0x60] = {2, 4, [](Cpu &cpu, uint8_t *data) {
                    ADC(cpu, cpu.ram(data[1], data[2]));
                }};

    // AND
    op[0x29] = {2, 4, [](Cpu &cpu, uint8_t *data) {
                    AND(cpu, cpu.ram(data[1], data[2]));
                }};
}

} // namespace
