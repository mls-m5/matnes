#pragma once

#include "cpu.h"
#include "instructions.h"

namespace matnes {
namespace cpu {

struct InstructionLookup {
    Instruction instruction;
    const char *name;
    decltype(&Cpu::ADC) function;
};

//! Conversion between Instruction enum and string
//! in "ADC" will become {ADC, "ADC"}
#define in(name)                                                               \
    { name, #name, &Cpu::ADC }

constexpr InstructionLookup instructionNames[] = {
    in(ADC), // Add Memory to Accumulator with Carry
    in(AND), // "AND" Memory with Accumulator
    in(ASL), // Shift Left One Bit (Memory or Accumulator)

    in(BCC), // Branch on Carry Clear
    in(BCS), // Branch on Carry Set
    in(BEQ), // Branch on Result Zero
    in(BIT), // Test Bits in Memory with Accumulator
    in(BMI), // Branch on Result Minus
    in(BNE), // Branch on Result not Zero
    in(BPL), // Branch on Result Plus
    in(BRK), // Force Break
    in(BVC), // Branch on Overflow Clear
    in(BVS), // Branch on Overflow Set

    in(CLC), // Clear Carry Flag
    in(CLD), // Clear Decimal Mode
    in(CLI), // Clear interrupt Disable Bit
    in(CLV), // Clear Overflow Flag
    in(CMP), // Compare Memory and Accumulator
    in(CPX), // Compare Memory and Index X
    in(CPY), // Compare Memory and Index Y

    in(DEC), // Decrement Memory by One
    in(DEX), // Decrement Index X by One
    in(DEY), // Decrement Index Y by One

    in(EOR), //"Exclusive-Or" Memory with Accumulator

    in(INC), // Increment Memory by One
    in(INX), // Increment Index X by One
    in(INY), // Increment Index Y by One

    in(JMP), // Jump to New Location

    in(JSR), // Jump to New Location Saving Return Address

    in(LDA), // Load Accumulator with Memory
    in(LDX), // Load Index X with Memory
    in(LDY), // Load Index Y with Memory
    in(LSR), // Shift Right One Bit (Memory or Accumulator)

    in(NOP), // No Operation

    in(ORA), // "OR" Memory with Accumulator

    in(PHA), // Push Accumulator on Stack
    in(PHP), // Push Processor Status on Stack
    in(PLA), // Pull Accumulator from Stack
    in(PLP), // Pull Processor Status from Stack

    in(ROL), // Rotate One Bit Left (Memory or Accumulator)
    in(ROR), // Rotate One Bit Right (Memory or Accumulator)
    in(RTI), // Return from Interrupt
    in(RTS), // Return from Subroutine

    in(SBC), // Subtract Memory from Accumulator with Borrow
    in(SEC), // Set Carry Flag
    in(SED), // Set Decimal Mode
    in(SEI), // Set Interrupt Disable Status
    in(STA), // Store Accumulator in Memory
    in(STX), // Store Index X in Memory
    in(STY), // Store Index Y in Memory

    in(TAX), // Transfer Accumulator to Index X
    in(TAY), // Transfer Accumulator to Index Y
    in(TSX), // Transfer Stack Pointer to Index X
    in(TXA), // Transfer Index X to Accumulator
    in(TXS), // Transfer Index X to Stack Pointer
    in(TYA), // Transfer Index Y to Accumulator
};

#undef in

constexpr auto getInstructionName(Instruction instruction) {
    return instructionNames[instruction];
}

constexpr auto getInstructionFunction(Instruction instruction) {
    return instructionNames[instruction].function;
}

Instruction getInstructionEnum(const std::string &name) {
    for (auto &pair : instructionNames) {
        if (pair.name == name) {
            return pair.instruction;
        }
    }

    return NOP;
}

//! The question is then.. why would anybody want to lookup with a string in
//! compile time... Good question, but it is possible
constexpr Instruction getInstructionEnum(const char *name) {
    if (name[0] != 0 && name[1] != 0 && name[2] != 0) {
        for (auto &pair : instructionNames) {
            auto n = pair.name;
            if (n[0] == name[0] && n[1] == name[1] && n[2] == name[2]) {
                return pair.instruction;
            }
        }
    }
    else {
        return NOP;
    }
}

} // namespace cpu
} // namespace matnes
