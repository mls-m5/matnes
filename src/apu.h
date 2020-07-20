#pragma once

#include "bus.h"
#include <cstdint>

namespace matnes {

class Apu : public IBusComponent {

    // Try to write to component, return true if successfull
    virtual bool write(uint16_t address, uint8_t value) override;

    // Try to read a byte return empty if not in range
    virtual std::optional<uint8_t> read(uint16_t address) override;

private:
    constexpr bool inRange(uint16_t address) {
        return address >= 0x4000 && address < 0x4020;
    }

    struct {
        // start at 0x4000
        uint8_t pulse1[4] = {};   // 0x0-0x3
        uint8_t pulse2[4] = {};   // 0x4-0x7
        uint8_t triangle[4] = {}; // 0x8-0xB
        uint8_t noise[4] = {};    // 0xC-0xF
        uint8_t dmc[4] = {};      // 0x10-0x13
        uint8_t unused = 0;       // 0x14
        uint8_t status = 0;       // 0x15
        uint8_t unused2 = 0;      // 0x16
        uint8_t frameCounter = 0; // 0x17
    } registers;
};

} // namespace matnes
