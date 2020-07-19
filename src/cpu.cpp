#include "cpu.h"

#include "instruction-mode.h"

#include <functional>
#include <vector>

using namespace matnes::cpu;

void Cpu::tick() {
    if (_extraClockCycles > 0) {
        --_extraClockCycles;
        return;
    }

    uint8_t opCode = ramBig(_programCounter);
    auto instruction = instructionModes[opCode];
    src(_programCounter);
    src2(_programCounter + 2); // May be unused
    _programCounter += instruction.instructionLength;

    _extraClockCycles +=
        instruction.executionTime - 1; // This tick counts as one

    auto &memory = (this->*instruction.memoryFunction)();
    (this->*instruction.instructionFunction)(memory);
}
