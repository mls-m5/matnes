#pragma once

#include <array>
#include <iostream>

//! Returns raw assembly code
//! Note that the return value may contnains zeroes other than in the end
std::string translateAssemblyLine(const std::string line);

class Assembler {
public:
    Assembler(std::istream &stream);

    operator bool() const {
        return static_cast<bool>(_stream);
    }

private:
    std::istream &_stream;
    std::array<uint8_t, 1024 * 2> _memory = {};
};
