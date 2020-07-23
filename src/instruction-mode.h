#pragma once

#include "cpu.h"
#include "execution-times.h"
#include "instruction-lookup.h"
#include "instructions.h"
#include <experimental/iterator>

namespace matnes {
namespace cpu {

struct InstructionInfo {
    typedef decltype(&Cpu::accumculator) MemoryFunctionType;
    typedef decltype(&Cpu::ADC) InstructionFunctionType;

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

    constexpr uint8_t getInstructionLength(MemoryMode mode) {
        switch (mode) {
        case Absolute:
        case AbsoluteX:
        case AbsoluteY:
        case Indirect:
            return 3;
        case Accumulator:
        case Implied:
            return 1;
        default:
            return 2;
        }
    }

    constexpr InstructionInfo(uint8_t opCode,
                              Instruction instruction = NOP,
                              MemoryMode mode = Implied)
        : instruction(instruction), mode(mode),
          extraPagingTime(getExecutionTime(instruction, mode) < 0),
          opCode(opCode), executionTime(getExecutionTime(instruction, mode)),
          instructionLength(getInstructionLength(mode)),
          memoryFunction(getMemoryFunction(mode)),
          instructionFunction(getInstructionFunction(instruction)) {
    }

    constexpr InstructionInfo() = default;
    constexpr InstructionInfo(const InstructionInfo &) = default;
    constexpr InstructionInfo &operator=(const InstructionInfo &) = default;

    Instruction instruction = Illegal;
    MemoryMode mode = Implied;
    bool extraPagingTime = false;
    uint8_t opCode = 0;
    uint8_t executionTime = 0;
    uint8_t instructionLength = 0;
    MemoryFunctionType memoryFunction = nullptr;
    InstructionFunctionType instructionFunction = nullptr;
};

constexpr InstructionInfo instructionModes[] = {
    {0x0, BRK},
    {0x1, ORA, _IndirectX_},
    {0x2}, // unused
    {0x3}, // unused
    {0x4}, // unused
    {0x5, ORA, ZeroPage},
    {0x6, ASL, ZeroPage},
    {0x7}, // unused
    {0x8, PHP},
    {0x9, ORA, Immediate},
    {0xA, ASL, Accumulator},
    {0xB}, // unused
    {0xC}, // unused
    {0xD, ORA, Absolute},
    {0xE, ASL, Absolute},
    {0xF}, // unused
    {0x10, BPL},
    {0x11, ORA, _Indirect_Y},
    {0x12}, // unused
    {0x13}, // unused
    {0x14}, // unused
    {0x15, ORA, ZeroPageX},
    {0x16, ASL, ZeroPageX},
    {0x17}, // unused
    {0x18, CLC},
    {0x19, ORA, AbsoluteY},
    {0x1A}, // unused
    {0x1B}, // unused
    {0x1C}, // unused
    {0x1D, ORA, AbsoluteX},
    {0x1E, ASL, AbsoluteX},
    {0x1F}, // unused
    {0x20, JSR},
    {0x21, AND, _IndirectX_},
    {0x22}, // unused
    {0x23}, // unused
    {0x24, BIT, ZeroPage},
    {0x25, AND, ZeroPage},
    {0x26, ROL, ZeroPage},
    {0x27}, // unused
    {0x28, PLP},
    {0x29, AND, Immediate},
    {0x2A, ROL, Accumulator},
    {0x2B}, // unused
    {0x2C, BIT, Absolute},
    {0x2D, AND, Absolute},
    {0x2E, ROL, Absolute},
    {0x2F}, // unused
    {0x30, BMI},
    {0x31, AND, _Indirect_Y},
    {0x32}, // unused
    {0x33}, // unused
    {0x34}, // unused
    {0x35, AND, ZeroPageX},
    {0x36, ROL, ZeroPageX},
    {0x37}, // unused
    {0x38, SEC},
    {0x39, AND, AbsoluteY},
    {0x3A}, // unused
    {0x3B}, // unused
    {0x3C}, // unused
    {0x3D, AND, AbsoluteX},
    {0x3E, ROL, AbsoluteX},
    {0x3F}, // unused
    {0x40, RTI},
    {0x41, EOR, _IndirectX_},
    {0x42}, // unused
    {0x43}, // unused
    {0x44}, // unused
    {0x45, EOR, ZeroPage},
    {0x46, LSR, ZeroPage},
    {0x47}, // unused
    {0x48, PHA},
    {0x49, EOR, Immediate},
    {0x4A, LSR, Accumulator},
    {0x4B}, // unused
    {0x4C, JMP, Absolute},
    {0x4D, EOR, Absolute},
    {0x4E, LSR, Absolute},
    {0x4F}, // unused
    {0x50, BVC},
    {0x51, EOR, _Indirect_Y},
    {0x52}, // unused
    {0x53}, // unused
    {0x54}, // unused
    {0x55, EOR, ZeroPageX},
    {0x56, LSR, ZeroPageX},
    {0x57}, // unused
    {0x58, CLI},
    {0x59, EOR, AbsoluteY},
    {0x5A}, // unused
    {0x5B}, // unused
    {0x5C}, // unused
    {0x5D, EOR, AbsoluteX},
    {0x5E, LSR, AbsoluteX},
    {0x5F}, // unused
    {0x60, RTS},
    {0x61, ADC, _IndirectX_},
    {0x62}, // unused
    {0x63}, // unused
    {0x64}, // unused
    {0x65, ADC, ZeroPage},
    {0x66, ROR, ZeroPage},
    {0x67}, // unused
    {0x68, PLA},
    {0x69, ADC, Immediate},
    {0x6A, ROR, Accumulator},
    {0x6B}, // unused
    {0x6C, JMP, Indirect},
    {0x6D, ADC, Absolute},
    {0x6E, ROR, Absolute},
    {0x6F}, // unused
    {0x70, BVS},
    {0x71, ADC, _Indirect_Y},
    {0x72}, // unused
    {0x73}, // unused
    {0x74}, // unused
    {0x75, ADC, ZeroPageX},
    {0x76, ROR, ZeroPageX},
    {0x77}, // unused
    {0x78, SEI},
    {0x79, ADC, AbsoluteY},
    {0x7A}, // unused
    {0x7B}, // unused
    {0x7C}, // unused
    {0x7D, ADC, AbsoluteX},
    {0x7E, ROR, AbsoluteX},
    {0x7F}, // unused
    {0x80}, // unused
    {0x81, STA, Absolute},
    {0x82, STA, _IndirectX_},
    {0x83}, // unused
    {0x84}, // unused
    {0x85, STY, ZeroPage},
    {0x86, STA, ZeroPage},
    {0x87, STX, ZeroPage},
    {0x88}, // unused
    {0x89, DEY},
    {0x8A}, // unused
    {0x8B, TXA},
    {0x8C}, // unused
    {0x8D, STY, Absolute},
    {0x8E, STX, Absolute},
    {0x8F}, // unused
    {0x90, BCC},
    {0x91, STA, _Indirect_Y},
    {0x92}, // unused
    {0x93}, // unused
    {0x94, STY, ZeroPageX},
    {0x95, STA, ZeroPageX},
    {0x96, STX, ZeroPageY},
    {0x97}, // unused
    {0x98, TYA},
    {0x99, STA, AbsoluteY},
    {0x9A, TXS},
    {0x9B}, // unused
    {0x9C}, // unused
    {0x9D, STA, AbsoluteX},
    {0x9E}, // unused
    {0x9F}, // unused
    {0xA0, LDY, Immediate},
    {0xA1, LDA, _IndirectX_},
    {0xA2, LDX, Immediate},
    {0xA3}, // unused
    {0xA4, LDY, ZeroPage},
    {0xA5, LDA, ZeroPage},
    {0xA6, LDX, ZeroPage},
    {0xA7}, // unused
    {0xA8, TAY},
    {0xA9, LDA, Immediate},
    {0xAA, TAX},
    {0xAB}, // unused
    {0xAC, LDY, Absolute},
    {0xAD, LDA, Absolute},
    {0xAE, LDX, Absolute},
    {0xAF}, // unused
    {0xB0, BCS},
    {0xB1, LDA, _Indirect_Y},
    {0xB2}, // unused
    {0xB3}, // unused
    {0xB4, LDY, ZeroPageX},
    {0xB5, LDA, ZeroPageX},
    {0xB6, LDX, ZeroPageY},
    {0xB7}, // unused
    {0xB8, CLV},
    {0xB9, LDA, AbsoluteY},
    {0xBA, TSX},
    {0xBB}, // unused
    {0xBC, LDY, AbsoluteX},
    {0xBD, LDA, AbsoluteX},
    {0xBE, LDX, AbsoluteY},
    {0xBF}, // unused
    {0xC0, CPY, Immediate},
    {0xC1, CMP, _IndirectX_},
    {0xC2}, // unused
    {0xC3}, // unused
    {0xC4, CPY, ZeroPage},
    {0xC5, CMP, ZeroPage},
    {0xC6, DEC, ZeroPage},
    {0xC7}, // unused
    {0xC8, INY},
    {0xC9, CMP, Immediate},
    {0xCA, DEX},
    {0xCB}, // unused
    {0xCC, CPY, Absolute},
    {0xCD, CMP, Absolute},
    {0xCE, DEC, Absolute},
    {0xCF}, // unused
    {0xD0, BNE},
    {0xD1, CMP, _Indirect_Y},
    {0xD2}, // unused
    {0xD3}, // unused
    {0xD4}, // unused
    {0xD5, CMP, ZeroPageX},
    {0xD6, DEC, ZeroPageX},
    {0xD7}, // unused
    {0xD8, CLD},
    {0xD9, CMP, AbsoluteY},
    {0xDA}, // unused
    {0xDB}, // unused
    {0xDC}, // unused
    {0xDD, CMP, AbsoluteX},
    {0xDE, DEC, AbsoluteX},
    {0xDF}, // unused
    {0xE0, CPX, Immediate},
    {0xE1, SBC, _IndirectX_},
    {0xE2}, // unused
    {0xE3}, // unused
    {0xE4, CPX, ZeroPage},
    {0xE5, SBC, ZeroPage},
    {0xE6, INC, ZeroPage},
    {0xE7}, // unused
    {0xE8, INX},
    {0xE9, SBC, Immediate},
    {0xEA, NOP},
    {0xEB}, // unused
    {0xEC, CPX, Absolute},
    {0xED, SBC, Absolute},
    {0xEE, INC, Absolute},
    {0xEF}, // unused
    {0xF0, BEQ},
    {0xF1, SBC, _Indirect_Y},
    {0xF2}, // unused
    {0xF3}, // unused
    {0xF4}, // unused
    {0xF5, SBC, ZeroPageX},
    {0xF6, INC, ZeroPageX},
    {0xF7}, // unused
    {0xF8, SED},
    {0xF9, SBC, AbsoluteY},
    {0xFA}, // unused
    {0xFB}, // unused
    {0xFC}, // unused
    {0xFD, SBC, AbsoluteX},
    {0xFE, INC, AbsoluteX},
    {0xFF}, // unusedd
};

constexpr InstructionInfo getInfoFromInstructionAndMode(Instruction instruction,
                                                        MemoryMode mode) {
    for (size_t index = 0; index < std::size(instructionModes); ++index) {
        auto &instructionInfo = instructionModes[index];
        if (instructionInfo.instruction == instruction &&
            instructionInfo.mode == mode) {
            return instructionInfo;
        }
    }
    return {0, {}};
}

} // namespace cpu
} // namespace matnes
