#pragma once

#include "bus.h"
#include <array>
#include <cstdint>

namespace matnes {

namespace apu {

class PulseMemory {
public:
    constexpr PulseMemory() = default;

    constexpr void write(uint8_t index, uint8_t value) {
        _memory[index] = value;
    }

    constexpr uint8_t read(uint8_t index) const {
        return _memory[index];
    }

    //! For more information
    //! https://wiki.nesdev.com/w/index.php/APU#Pulse_.28.244000-4007.29
    constexpr uint8_t duty() {
        return _memory[0] >> 6;
    }

    //! Same bit as length counter halt
    constexpr bool envelopeLoop() {
        return (_memory[0] >> 5) & 1;
    }

    constexpr bool constatVolume() {
        return (_memory[0] >> 4) & 1;
    }

    //! Same as envelope
    constexpr uint8_t volume() {
        return _memory[0] & 0b1111;
    }

    constexpr bool sweepUnitEnabled() {
        return _memory[1] >> 7;
    }

    constexpr uint8_t period() {
        return (_memory[1] >> 4) & 0b111;
    }

    constexpr bool negative() {
        return _memory[1] & 0b11000;
    }

    constexpr uint8_t shift() {
        return _memory[1] & 0b111;
    }

    constexpr uint8_t timerLow() {
        return _memory[2];
    }

    constexpr uint8_t timerHigh() {
        return _memory[3] & 0b111;
    }

    constexpr uint16_t timer() {
        return timerLow() + (timerHigh() << 8);
    }

    constexpr uint8_t lengthCounterLoad() {
        return _memory[3] >> 3;
    }

private:
    std::array<uint8_t, 4> _memory = {};
};

class Pulse {
public:
    constexpr Pulse(PulseMemory &memory) : _memory(memory) {
    }

    constexpr void write(uint8_t index, uint8_t value) {
        if (index == 3) {
            _lengthCounter = _memory.lengthCounterLoad();
            _envelope = 15;
        }
        _memory.write(index, value);
    }

    constexpr uint8_t read(uint8_t index) const {
        return _memory.read(index);
    }

    constexpr void tick() {
        //        if (_lengthCounter) {
        //            --_lengthCounter;
        //        }
        //        if ( _memory._envelope) {
        //            --_envelope;
        //        }
    }

private:
    PulseMemory &_memory;
    uint8_t _lengthCounter = 0;
    uint8_t _envelope = 0;
};

} // namespace apu

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
