#pragma once

#include "instructions.h"

struct InstructionMode {
    constexpr InstructionMode(Instruction instruction,
                              MemoryMode mode = Implied)
        : instruction(instruction), mode(mode) {
    }

    constexpr InstructionMode() = default;
    constexpr InstructionMode(const InstructionMode &) = default;
    constexpr InstructionMode &operator=(const InstructionMode &) = default;

    Instruction instruction = NOP;
    MemoryMode mode = Implied;
};

constexpr InstructionMode instructionModes[] = {
    [0x00] = {BRK},
    [0x01] = {ORA, _IndirectX_},

    [0x05] = {ORA, ZeroPage},
    [0x06] = {ASL, ZeroPage},

    [0x08] = {PHP},
    [0x09] = {ORA, Immediate},
    [0x0A] = {ASL, Accumulator},

    [0x0D] = {ORA, Absolute},
    [0x0E] = {ASL, Absolute},

    [0x10] = {BPL},
    [0x11] = {ORA, _Indirect_Y},
    [0x31] = {AND, _Indirect_Y},

    [0x15] = {ORA, ZeroPageX},
    [0x16] = {ASL, ZeroPageX},

    [0x18] = {CLC},
    [0x19] = {ORA, AbsoluteY},

    [0x1D] = {ORA, AbsoluteX},
    [0x1E] = {ASL, AbsoluteX},

    [0x20] = {JSR},
    [0x21] = {AND, _IndirectX_},

    [0x24] = {BIT, ZeroPage},
    [0x25] = {AND, ZeroPage},
    [0x26] = {ROL, ZeroPage},

    [0x28] = {PLP},
    [0x29] = {AND, Immediate},
    [0x2A] = {ROL, Accumulator},

    [0x2C] = {BIT, Absolute},
    [0x2D] = {AND, Absolute},
    [0x2E] = {ROL, Absolute},

    [0x30] = {BMI},

    [0x35] = {AND, ZeroPageX},
    [0x36] = {ROL, ZeroPageX},

    [0x38] = {SEC},
    [0x39] = {AND, AbsoluteY},

    [0x3D] = {AND, AbsoluteX},
    [0x3E] = {ROL, AbsoluteX},

    [0x40] = {RTI},
    [0x41] = {EOR, _IndirectX_},
    [0x61] = {ADC, _IndirectX_},

    [0x45] = {EOR, ZeroPage},
    [0x46] = {LSR, ZeroPage},

    [0x48] = {PHA},
    [0x49] = {EOR, Immediate},
    [0x4A] = {LSR, Accumulator},

    [0x4C] = {JMP, AbsoluteIndirect}, // Absolute indirect
    [0x4D] = {EOR, Absolute},
    [0x4E] = {LSR, Absolute},

    [0x50] = {BVC},
    [0x5D] = {EOR, AbsoluteX},
    [0x51] = {EOR, _Indirect_Y},
    [0x71] = {ADC, _Indirect_Y},

    [0x55] = {EOR, ZeroPageX},
    [0x56] = {LSR, ZeroPageX},

    [0x58] = {CLI},
    [0x59] = {EOR, AbsoluteY},

    [0x5E] = {LSR, AbsoluteX},

    [0x60] = {RTS},

    [0x65] = {ADC, ZeroPage},
    [0x66] = {ROR, ZeroPage},

    [0x68] = {PLA},
    [0x69] = {ADC, Immediate},
    [0x6A] = {ROR, Accumulator},

    [0x6C] = {JMP, Indirect},
    [0x6D] = {ADC, Absolute},
    [0x6E] = {ROR, Absolute},

    [0x7D] = {ADC, AbsoluteX},
    [0x70] = {BVS},

    [0x75] = {ADC, ZeroPageX},
    [0x76] = {ROR, ZeroPageX},

    [0x78] = {SEI},
    [0x79] = {ADC, AbsoluteY},

    [0x7E] = {ROR, AbsoluteX},

    [0x80] = {STA, Absolute},
    [0x81] = {STA, _IndirectX_},
    [0xA1] = {LDA, _IndirectX_},

    [0x84] = {STY, ZeroPage},
    [0x85] = {STA, ZeroPage},
    [0x86] = {STX, ZeroPage},

    [0x88] = {DEY},

    [0x8A] = {TXA},

    [0x8C] = {STY, Absolute},
    [0x8E] = {STX, Absolute},

    [0x90] = {BCC},
    [0x9D] = {STA, AbsoluteX},
    [0x91] = {STA, _Indirect_Y},
    [0xB1] = {LDA, _Indirect_Y},

    [0x94] = {STY, ZeroPageX},
    [0x95] = {STA, ZeroPageX},
    [0x96] = {STX, ZeroPageY},

    [0x98] = {TYA},
    [0x99] = {STA, AbsoluteY},
    [0x9A] = {TXS},

    [0xA0] = {LDY, Immediate},
    [0xA2] = {LDX, Immediate},

    [0xA4] = {LDY, ZeroPage},
    [0xA5] = {LDA, ZeroPage},
    [0xA6] = {LDX, ZeroPage},

    [0xA8] = {TAY},
    [0xA9] = {LDA, Immediate},
    [0xAA] = {TAX},

    [0xAC] = {LDY, Absolute},
    [0xAD] = {LDA, Absolute},
    [0xAE] = {LDX, Absolute},

    [0xB0] = {BCS},

    [0xB4] = {LDY, ZeroPageX},
    [0xB5] = {LDA, ZeroPageX},
    [0xB6] = {LDX, ZeroPageY},

    [0xB8] = {CLV},
    [0xB9] = {LDA, AbsoluteY},
    [0xBA] = {TSX},

    [0xBC] = {LDY, AbsoluteX},
    [0xBD] = {LDA, AbsoluteX},
    [0xBE] = {LDX, AbsoluteY},

    [0xC0] = {CPY, Immediate},
    [0xC1] = {CMP, _IndirectX_},
    [0xE1] = {SBC, _IndirectX_},

    [0xC4] = {CPY, ZeroPage},
    [0xC5] = {CMP, ZeroPage},
    [0xC6] = {DEC, ZeroPage},

    [0xC8] = {INY},
    [0xC9] = {CMP, Immediate},
    [0xCA] = {DEX},

    [0xCC] = {CPY, Absolute},
    [0xCD] = {CMP, Absolute},
    [0xCE] = {DEC, Absolute},

    [0xD0] = {BNE},
    [0xD1] = {CMP, _Indirect_Y},
    [0xF1] = {SBC, _Indirect_Y},

    [0xD5] = {CMP, ZeroPageX},
    [0xD6] = {DEC, ZeroPageX},

    [0xD8] = {CLD},
    [0xD9] = {CMP, AbsoluteY},

    [0xDD] = {CMP, AbsoluteX},
    [0xDE] = {DEC, AbsoluteX},

    [0xE0] = {CPX, Immediate},

    [0xE4] = {CPX, ZeroPage},
    [0xE5] = {SBC, ZeroPage},
    [0xE6] = {INC, ZeroPage},

    [0xE8] = {INX},
    [0xE9] = {SBC, Immediate},
    [0xEA] = {NOP},

    [0xEC] = {CPX, Absolute},
    [0xED] = {SBC, Absolute},
    [0xEE] = {INC, Absolute},

    [0xF0] = {BEQ},

    [0xF5] = {SBC, ZeroPageX},
    [0xF6] = {INC, ZeroPageX},

    [0xF8] = {SED},
    [0xF9] = {SBC, AbsoluteY},

    [0xFD] = {SBC, AbsoluteX},
    [0xFE] = {INC, AbsoluteX},
};
