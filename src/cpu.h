#pragma once

// http://nesdev.com/6502.txt

#include <array>
#include <cstdint>

class Cpu {
public:
    constexpr Cpu() = default;

    constexpr bool statusFlag(uint8_t index) const {
        return _status | (1 << index);
    }

    constexpr void statusFlag(uint8_t index, bool value) {
        if (value) {
            _status = (_status | (1 << index));
        }
        else {
            _status = (_status | ~(static_cast<uint8_t>(1 << index)));
        }
    }

    constexpr void A(uint8_t value) {
        _accumulator = value;
    }

    constexpr auto &A() {
        return _accumulator;
    }

    [[nodiscard]] constexpr uint8_t A() const {
        return _accumulator;
    }

    //! C
    [[nodiscard]] constexpr bool carry() const {
        return statusFlag(0);
    }

    constexpr void carry(bool value) {
        statusFlag(0, value);
    }

    //! Z
    [[nodiscard]] constexpr bool zero() const {
        return statusFlag(1);
    }

    constexpr void zero(bool value) {
        statusFlag(1, value);
    }

    //! I
    [[nodiscard]] constexpr bool disableInterupts() const {
        return statusFlag(2);
    }

    constexpr void disableInterupts(bool value) {
        statusFlag(2, value);
    }

    //! D
    [[nodiscard]] constexpr bool decimalFlag() const {
        return statusFlag(3);
    }

    constexpr void decimalFlag(bool value) {
        statusFlag(3, value);
    }

    //! B
    [[nodiscard]] constexpr bool softwareInterupt() const {
        return statusFlag(4);
    }

    constexpr void softwareInterupt(bool value) {
        statusFlag(4, value);
    }

    //! V
    [[nodiscard]] constexpr bool overflowFlag() const {
        return statusFlag(6);
    }

    constexpr void overflowFlag(bool value) {
        statusFlag(6, value);
    }

    //! S
    [[nodiscard]] constexpr bool signFlag() const {
        return statusFlag(7);
    }

    constexpr void signFlag(bool value) {
        statusFlag(7, value);
    }

    [[nodiscard]] constexpr uint8_t ram(uint8_t index1,
                                        uint8_t index2 = 0) const {
        return _ram[(static_cast<int>(index2) << 8) + index1];
    }

    [[nodiscard]] uint8_t &ram(uint8_t index1, uint8_t index2 = 0) {
        return _ram[(static_cast<int>(index2) << 8) + index1];
    }

private:
    // Registers
    int8_t _accumulator = 0;
    int8_t _xIndex = 0;
    int8_t _yIndex = 0;
    int8_t _status = 1 << 5;
    uint16_t _programCounter = 0;
    uint8_t _stackPointer = 0;

    std::array<uint8_t, 1024 * 2> _ram = {};
};
