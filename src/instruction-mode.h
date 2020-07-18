#pragma once

#include "cpu.h"
#include "execution-times.h"
#include "instructions.h"

namespace matnes {
namespace cpu {

typedef decltype(&Cpu::accumculator) MemoryFunctionType;

struct InstructionInfo {

    constexpr MemoryFunctionType getMemoryFunction(MemoryMode mode) {
        switch (mode) {
        case Accumulator:
            return &Cpu::accumculator;
        case Immediate:
            return &Cpu::immediate;
        case ZeroPage:
            return &Cpu::zeroPage;
        case ZeroPageX:
            return &Cpu::zeroPageX;
        case ZeroPageY:
            return &Cpu::zeroPageY;
        case Absolute:
            return &Cpu::absolute;
        case AbsoluteX:
            return &Cpu::absoluteX;
        case AbsoluteY:
            return &Cpu::absoluteY;
        case Implied:
            return &Cpu::implied;
        case Relative:
            return &Cpu::relative;
        case _IndirectX_:
            return &Cpu::indirectX;
        case _Indirect_Y:
            return &Cpu::indirectY;
        case AbsoluteIndirect:
            return &Cpu::absoluteIndirect;
        case Indirect: // Not supported for nes, but for more complex processors
            return &Cpu::indirect;
        }

        return nullptr;
    }

    constexpr InstructionInfo(Instruction instruction,
                              MemoryMode mode = Implied)
        : instruction(instruction), mode(mode),
          extraPagingTime(getExecutionTime(instruction, mode) < 0),
          memoryFunction(getMemoryFunction(mode)) {
    }

    constexpr InstructionInfo() = default;
    constexpr InstructionInfo(const InstructionInfo &) = default;
    constexpr InstructionInfo &operator=(const InstructionInfo &) = default;

    Instruction instruction = NOP;
    MemoryMode mode = Implied;
    bool extraPagingTime = false;
    int executionTime = 0;
    MemoryFunctionType memoryFunction = 0;
};

constexpr InstructionInfo instructionModes[] = {
    {BRK},
    {ORA, _IndirectX_},
    {}, // unused
    {}, // unused
    {}, // unused
    {ORA, ZeroPage},
    {ASL, ZeroPage},
    {}, // unused
    {PHP},
    {ORA, Immediate},
    {ASL, Accumulator},
    {}, // unused
    {}, // unused
    {ORA, Absolute},
    {ASL, Absolute},
    {}, // unused
    {BPL},
    {ORA, _Indirect_Y},
    {}, // unused
    {}, // unused
    {}, // unused
    {ORA, ZeroPageX},
    {ASL, ZeroPageX},
    {}, // unused
    {CLC},
    {ORA, AbsoluteY},
    {}, // unused
    {}, // unused
    {}, // unused
    {ORA, AbsoluteX},
    {ASL, AbsoluteX},
    {}, // unused
    {JSR},
    {AND, _IndirectX_},
    {}, // unused
    {}, // unused
    {BIT, ZeroPage},
    {AND, ZeroPage},
    {ROL, ZeroPage},
    {}, // unused
    {PLP},
    {AND, Immediate},
    {ROL, Accumulator},
    {}, // unused
    {BIT, Absolute},
    {AND, Absolute},
    {ROL, Absolute},
    {}, // unused
    {BMI},
    {AND, _Indirect_Y},
    {}, // unused
    {}, // unused
    {}, // unused
    {AND, ZeroPageX},
    {ROL, ZeroPageX},
    {}, // unused
    {SEC},
    {AND, AbsoluteY},
    {}, // unused
    {}, // unused
    {}, // unused
    {AND, AbsoluteX},
    {ROL, AbsoluteX},
    {}, // unused
    {RTI},
    {EOR, _IndirectX_},
    {}, // unused
    {}, // unused
    {}, // unused
    {EOR, ZeroPage},
    {LSR, ZeroPage},
    {}, // unused
    {PHA},
    {EOR, Immediate},
    {LSR, Accumulator},
    {}, // unused
    {JMP, Absolute},
    {EOR, Absolute},
    {LSR, Absolute},
    {}, // unused
    {BVC},
    {EOR, _Indirect_Y},
    {}, // unused
    {}, // unused
    {}, // unused
    {EOR, ZeroPageX},
    {LSR, ZeroPageX},
    {}, // unused
    {CLI},
    {EOR, AbsoluteY},
    {}, // unused
    {}, // unused
    {}, // unused
    {EOR, AbsoluteX},
    {LSR, AbsoluteX},
    {}, // unused
    {RTS},
    {ADC, _IndirectX_},
    {}, // unused
    {}, // unused
    {}, // unused
    {ADC, ZeroPage},
    {ROR, ZeroPage},
    {}, // unused
    {PLA},
    {ADC, Immediate},
    {ROR, Accumulator},
    {}, // unused
    {JMP, Indirect},
    {ADC, Absolute},
    {ROR, Absolute},
    {}, // unused
    {BVS},
    {ADC, _Indirect_Y},
    {}, // unused
    {}, // unused
    {}, // unused
    {ADC, ZeroPageX},
    {ROR, ZeroPageX},
    {}, // unused
    {SEI},
    {ADC, AbsoluteY},
    {}, // unused
    {}, // unused
    {}, // unused
    {ADC, AbsoluteX},
    {ROR, AbsoluteX},
    {}, // unused
    {}, // unused
    {STA, Absolute},
    {STA, _IndirectX_},
    {}, // unused
    {}, // unused
    {STY, ZeroPage},
    {STA, ZeroPage},
    {STX, ZeroPage},
    {}, // unused
    {DEY},
    {}, // unused
    {TXA},
    {}, // unused
    {STY, Absolute},
    {STX, Absolute},
    {}, // unused
    {BCC},
    {STA, _Indirect_Y},
    {}, // unused
    {}, // unused
    {STY, ZeroPageX},
    {STA, ZeroPageX},
    {STX, ZeroPageY},
    {}, // unused
    {TYA},
    {STA, AbsoluteY},
    {TXS},
    {}, // unused
    {}, // unused
    {STA, AbsoluteX},
    {}, // unused
    {}, // unused
    {LDY, Immediate},
    {LDA, _IndirectX_},
    {LDX, Immediate},
    {}, // unused
    {LDY, ZeroPage},
    {LDA, ZeroPage},
    {LDX, ZeroPage},
    {}, // unused
    {TAY},
    {LDA, Immediate},
    {TAX},
    {}, // unused
    {LDY, Absolute},
    {LDA, Absolute},
    {LDX, Absolute},
    {}, // unused
    {BCS},
    {LDA, _Indirect_Y},
    {}, // unused
    {}, // unused
    {LDY, ZeroPageX},
    {LDA, ZeroPageX},
    {LDX, ZeroPageY},
    {}, // unused
    {CLV},
    {LDA, AbsoluteY},
    {TSX},
    {}, // unused
    {LDY, AbsoluteX},
    {LDA, AbsoluteX},
    {LDX, AbsoluteY},
    {}, // unused
    {CPY, Immediate},
    {CMP, _IndirectX_},
    {}, // unused
    {}, // unused
    {CPY, ZeroPage},
    {CMP, ZeroPage},
    {DEC, ZeroPage},
    {}, // unused
    {INY},
    {CMP, Immediate},
    {DEX},
    {}, // unused
    {CPY, Absolute},
    {CMP, Absolute},
    {DEC, Absolute},
    {}, // unused
    {BNE},
    {CMP, _Indirect_Y},
    {}, // unused
    {}, // unused
    {}, // unused
    {CMP, ZeroPageX},
    {DEC, ZeroPageX},
    {}, // unused
    {CLD},
    {CMP, AbsoluteY},
    {}, // unused
    {}, // unused
    {}, // unused
    {CMP, AbsoluteX},
    {DEC, AbsoluteX},
    {}, // unused
    {CPX, Immediate},
    {SBC, _IndirectX_},
    {}, // unused
    {}, // unused
    {CPX, ZeroPage},
    {SBC, ZeroPage},
    {INC, ZeroPage},
    {}, // unused
    {INX},
    {SBC, Immediate},
    {NOP},
    {}, // unused
    {CPX, Absolute},
    {SBC, Absolute},
    {INC, Absolute},
    {}, // unused
    {BEQ},
    {SBC, _Indirect_Y},
    {}, // unused
    {}, // unused
    {}, // unused
    {SBC, ZeroPageX},
    {INC, ZeroPageX},
    {}, // unused
    {SED},
    {SBC, AbsoluteY},
    {}, // unused
    {}, // unused
    {}, // unused
    {SBC, AbsoluteX},
    {INC, AbsoluteX},
    {}, // unused
};

} // namespace cpu
} // namespace matnes
