
#pragma once

#include "instructions.h"

// order:
// =========================0
// Accumulator
// Immediate
// Zero Page
// Zero Page,X
// Zero Page,Y
// Absolute
// Absolute,X
// Absolute,Y
// Implied
// Relative
// (Indirect,X)
// (Indirect),Y
// Abs. Indirect

// Negative numbers: Add one cycle if indexing across page boundary
// Negative number in parenthesis: Add one cycle if branch is taken, add
//   one aditional if branching operation crosses page boundary
// 0 means that the operation is not implemented on the nes

namespace nes {
namespace cpu {

// clang-format off
constexpr int executionTimeConstants[4][13][14] = {

// A   A   A   B   B   B   B   B   B   B   B   B   B   C
// D   N   S   C   C   E   I   M   N   P   R   V   V   L
// C   D   L   C   S   Q   T   I   E   L   K   C   S   C
//constexpr int x[13][14] =
{
{  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  3,  3,  5,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0},
{  4,  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  4,  4,  6,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0},
{ -4, -4,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{ -4, -4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2},
{  0,  0,  0, (-2),(-2),(-2), 0, (-2),(-2),(-2), 7, (-2),(-2), 0},
{  6,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{ -5, -5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
},


// C   C   C   C   C   C   D   D   D   E   I   I   I   J
// L   L   L   M   P   P   E   E   E   O   N   N   N   M
// D   I   V   P   X   Y   C   X   Y   R   C   X   Y   P
//constexpr int y[13][14] =
{
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  2,  2,  2,  0,  0,  0,  2,  0,  0,  0,  0},
{  0,  0,  0,  3,  3,  3,  5,  0,  0,  3,  5,  0,  0,  0},
{  0,  0,  0,  4,  0,  0,  6,  0,  0,  4,  6,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  4,  4,  4,  6,  0,  0,  4,  6,  0,  0,  3},
{  0,  0,  0, -4,  0,  0,  7,  0,  0, -4,  7,  0,  0,  0},
{  0,  0,  0, -4,  0,  0,  0,  0,  0, -4,  0,  0,  0,  0},
{  2,  2,  2,  0,  0,  0,  0,  2,  2,  0,  0,  2,  2,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0},
{  0,  0,  0, -5,  0,  0,  0,  0,  0, -5,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5},
},


// J   L   L   L   L   N   O   P   P   P   P   R   R   R
// S   D   D   D   S   O   R   H   H   L   L   O   O   T
// R   A   X   Y   R   P   A   A   P   A   P   L   R   I
//constexpr int z[13][14] =
{
{  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  2,  2,  0},
{  0,  2,  2,  2,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0},
{  0,  3,  3,  3,  5,  0,  3,  0,  0,  0,  0,  5,  5,  0},
{  0,  4,  0,  4,  6,  0,  4,  0,  0,  0,  0,  6,  6,  0},
{  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  6,  4,  4,  4,  6,  0,  4,  0,  0,  0,  0,  6,  6,  0},
{  0, -4,  0, -4,  7,  0, -4,  0,  0,  0,  0,  7,  7,  0},
{  0, -4, -4,  0,  0,  0, -4,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  2,  0,  3,  3,  4,  4,  0,  0,  6},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  6,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0},
{  0, -5,  0,  0,  0,  0, -5,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
},

// R   S   S   S   S   S   S   S   T   T   T   T   T   T
// T   B   E   E   E   T   T   T   A   A   S   X   X   Y
// S   C   C   D   I   A   X   Y   X   Y   X   A   S   A
//constexpr int w[13][14] =
{
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  3,  0,  0,  0,  3,  3,  3,  0,  0,  0,  0,  0,  0},
{  0,  4,  0,  0,  0,  4,  0,  4,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0},
{  0,  4,  0,  0,  0,  4,  4,  4,  0,  0,  0,  0,  0,  0},
{  0, -4,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0},
{  0, -4,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0},
{  6,  0,  2,  2,  2,  0,  0,  0,  2,  2,  2,  2,  2,  2},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  6,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0},
{  0, -5,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
},
};

// clang-format on

constexpr int getExecutionTime(Instruction instruction, MemoryMode mode) {
    auto subInstruction = instruction % 13;
    auto chunk = instruction - subInstruction;
    return executionTimeConstants[chunk][mode][subInstruction];
}

} // namespace cpu
} // namespace nes
