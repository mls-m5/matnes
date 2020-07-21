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

    if (instruction.instruction == Instruction::Illegal) {
        throw std::invalid_argument(
            "got invalid opcode " + std::to_string(opCode) +
            " at ram address " + std::to_string(_programCounter));
    }
    src(_programCounter + 1);
    src2(_programCounter + 2); // May be unused
    _programCounter += instruction.instructionLength;

    _extraClockCycles +=
        instruction.executionTime - 1; // This tick counts as one

    auto memory = (this->*instruction.memoryFunction)();
    (this->*instruction.instructionFunction)(memory);
}
