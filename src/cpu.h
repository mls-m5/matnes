#pragma once

// http://nesdev.com/6502.txt

#include <array>
#include <cstdint>

// Convert between 8 and 16 bit integers
union Conversion {
    constexpr Conversion(uint16_t value) : u16(value) {
    }

    constexpr Conversion(uint8_t a, uint8_t b) : u8{a, b} {
    }

    uint8_t u8[2];
    uint16_t u16;
};

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

    [[nodiscard]] constexpr auto A() const {
        return _accumulator;
    }

    [[nodiscard]] constexpr auto X() const {
        return _xIndex;
    }

    [[nodiscard]] constexpr auto Y() const {
        return _yIndex;
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

    [[nodiscard]] constexpr uint8_t &ram(uint8_t index1, uint8_t index2 = 0) {
        return _ram[(static_cast<int>(index2) << 8) + index1];
    }

    // Reference ram directly with a uint16_t
    [[nodiscard]] constexpr uint8_t &ramBig(uint16_t index) {
        return _ram[index];
    }

    //! The current instruction
    [[nodiscard]] constexpr uint8_t instruction() const {
        return _ram.at(_programCounter);
    }

    //! Argument 1
    [[nodiscard]] constexpr uint8_t src() const {
        return _ram.at(_programCounter + 1);
    }

    // Argument 2
    [[nodiscard]] constexpr uint8_t src2() const {
        return _ram.at(_programCounter + 2);
    }

    // Composite src
    constexpr uint16_t srcBig() const {
        return (static_cast<uint16_t>(src2()) << 8) + src();
    }

    // ---------------------- Memory modes -------------------------------------
    [[nodiscard]] constexpr uint8_t &accumculator() {
        return _accumulator;
    }

    [[nodiscard]] constexpr uint8_t &immediate() {
        _dummy = src();
        return _dummy;
    }

    // Zero page absolute
    [[nodiscard]] constexpr uint8_t &zeroPage() {
        return ram(src());
    }

    [[nodiscard]] constexpr uint8_t &zeroPageX() {
        return ramBig(static_cast<uint16_t>(src()) + X());
    }

    [[nodiscard]] constexpr uint8_t &zeroPageY() {
        return ramBig(static_cast<uint16_t>(src()) + Y());
    }

    [[nodiscard]] constexpr uint8_t &absolute() {
        return ram(src(), src2());
    }

    [[nodiscard]] constexpr uint8_t &absoluteX() {
        return ram(Conversion(src(), src2()).u16 + X());
    }

    [[nodiscard]] constexpr uint8_t &absoluteY() {
        return ram(Conversion(src(), src2()).u16 + Y());
    }

    [[nodiscard]] constexpr uint8_t &implied() {
        return _dummy;
    }

    // Difference from immediate is that the value is treated as a signed number
    [[nodiscard]] constexpr uint8_t &relative() {
        _dummy = src();
        return _dummy; // Todo: Check if this is correct
    }

    // Pre-calculated indirect
    [[nodiscard]] constexpr uint8_t &indirectX() {
        auto tmpSrc = srcBig();
        auto tmp1 = ramBig(tmpSrc);
        auto tmp2 = ramBig(tmpSrc + 1);

        auto postAddress = Conversion(tmp1, tmp2).u16 + Y();

        return ram(postAddress);
    }

    // Post calculated indirect
    [[nodiscard]] constexpr uint8_t &indirectY() {
        auto tmpSrc = srcBig() + X();
        auto tmp1 = ramBig(tmpSrc);
        auto tmp2 = ramBig(tmpSrc + 1);

        return ram(tmp1, tmp2);
    }

    // Only used for the JMP instruction
    // JMP (Operand)
    // Also known as absolute indirect
    [[nodiscard]] constexpr uint8_t &absoluteIndirect() {
        auto tmpSrc = srcBig();
        auto tmp1 = ramBig(tmpSrc);
        auto tmp2 = ramBig(tmpSrc + 1);

        return ram(tmp1, tmp2);
    }

    // Not used for the nes
    [[nodiscard]] constexpr uint8_t &indirect() {
        return ramBig(srcBig());
    }

private:
    // Registers
    uint8_t _dummy = 0; // When need to return a reference
    uint8_t _accumulator = 0;
    uint8_t _xIndex = 0;
    uint8_t _yIndex = 0;
    uint8_t _status = 1 << 5;
    uint16_t _programCounter = 0;
    uint8_t _stackPointer = 0;

    std::array<uint8_t, 1024 * 2> _ram = {};
};
