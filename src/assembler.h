#pragma once

#include <array>
#include <istream>

namespace matnes {

using AssembledMemoryT = std::array<uint8_t, 1024 * 2>;

//! Returns raw assembly code
//! Note that the return value may contnains zeroes other than in the end
std::string translateAssemblyLine(const std::string line);

//! Main entry point for the assembler
AssembledMemoryT assembleCode(std::istream &stream);

} // namespace matnes
