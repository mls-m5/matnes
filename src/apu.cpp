

#include "apu.h"

bool matnes::Apu::write(uint16_t address, uint8_t value) {
    if (inRange(address)) {
        // Everything relative to the first register
        registers.pulse1[address - 0x4000] = value;
        return true;
    }
    else {
        return false;
    }
}

std::optional<uint8_t> matnes::Apu::read(uint16_t address) {
    if (address == 0x4015) {
        return registers.status;
    }
    else {
        return {};
    }
}
