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

class NotImplementedError : public std::logic_error {
    using logic_error::logic_error;
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

    constexpr void X(uint8_t value) {
        _xIndex = value;
    }

    [[nodiscard]] constexpr auto Y() const {
        return _yIndex;
    }

    constexpr void Y(uint8_t value) {
        _yIndex = value;
    }

    [[nodiscard]] constexpr auto stack() const {
        return _stackPointer;
    }

    constexpr void stack(uint8_t value) {
        _stackPointer = value;
    }

    constexpr uint8_t pull() {
        // Todo check if order is correct;
        auto value = ramBig(_stackPointer);
        ++_stackPointer;
        return value;
    }

    constexpr void push(uint8_t value) {
        // Todo: Check if order (decreesing and saving) is correct
        --_stackPointer;
        ramBig(_stackPointer) = value;
    }

    // Status flags ----------------------------

    [[nodiscard]] constexpr uint8_t S() {
        return _stackPointer;
    }

    constexpr void S(uint8_t value) {
        _stackPointer = value;
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

    constexpr auto updateZero(uint8_t value) {
        zero(value);
        return value;
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

    constexpr auto updateSign(uint8_t value) {
        signFlag(value & (1 << 7));
        return value;
    }

    // -----------------------

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

    // Set sign and zero bits
    constexpr uint8_t updateStatus(uint8_t value) {
        updateSign(value);
        updateZero(value);
        return value;
    }

    // ---------------------- Instructions -------------------------------------

    constexpr void ADC(uint8_t &) {
    }

    constexpr void AND(uint8_t &) {
    }

    constexpr void ASL(uint8_t &) {
    }

    // continue here.. uppward

    constexpr void EOR(uint8_t &memory) {
        A(updateStatus(A() ^ memory));
    }

    constexpr void INC(uint8_t &memory) {
        memory = updateStatus((memory + 1));
    }

    constexpr void INX(uint8_t &) {
        X(updateStatus(X() + 1));
    }

    constexpr void INY(uint8_t &) {
        Y(updateStatus(Y() + 1));
    }

    constexpr void JMP(uint8_t &) {
        _programCounter = srcBig();
    }

    constexpr void JSR(uint8_t &) {
        --_programCounter;
        push(_programCounter >> 8);
        push(_programCounter & 0xff);
        _programCounter = srcBig();
    }

    constexpr void LDA(uint8_t &memory) {
        A(updateStatus(memory));
    }

    constexpr void LDX(uint8_t &memory) {
        X(updateStatus(memory));
    }

    constexpr void LDY(uint8_t &memory) {
        Y(updateStatus(memory));
    }

    constexpr void LSR(uint8_t &memory) {
        carry(memory | 1);
        memory >>= 1;
        updateStatus(memory);
    }

    constexpr void NOP(uint8_t &) {
        // Doing nothing for a while...
    }

    constexpr void ORA(uint8_t &value) {
        A(updateStatus(A() | value));
    }

    constexpr void PHA(uint8_t &) {
        push(A());
    }

    constexpr void PHP(uint8_t &) {
        push(S());
    }

    constexpr void PLA(uint8_t &) {
        A(pull());
    }

    constexpr void PLP(uint8_t &) {
        S(pull());
    }

    constexpr void ROL(uint8_t &memory) {
        auto value = (memory << 1) + carry();
        carry(value & 0x100);
        value &= 0xff;
        updateStatus(value);
        memory = value;
    }

    constexpr void ROR(uint8_t &memory) {
        auto value = memory + carry() * 0x100;
        carry(value & 1);
        value >>= 1;
        updateStatus(value);
        memory = value;
    }

    constexpr void RTI(uint8_t &) {
        S(pull());
        uint8_t value = pull();
        value += pull() * 0x100;
        _programCounter = value;
    }

    constexpr void RTS(uint8_t &) {
        uint16_t value = pull();
        value += pull() * 0x100 + 1;
        _programCounter = value;
    }

    constexpr void SBC(uint8_t &value) {
        // Subtract with carry
        auto tmp = static_cast<uint16_t>(A()) - value - carry();
        updateStatus(tmp);
        overflowFlag(((A() ^ tmp) & 0x80) && ((A() ^ tmp) & 0x80));
        carry(!(tmp < 0x100)); // Todo: Check so that this is correct
    }

    // set flags

    constexpr void SEC(uint8_t &) {
        carry(true);
    }

    constexpr void SED(uint8_t &) {
        decimalFlag(true);
    }

    constexpr void SEI(uint8_t &) {
        disableInterupts(true);
    }

    // Store to memory

    constexpr void STA(uint8_t &memory) {
        memory = A();
    }

    constexpr void STX(uint8_t &memory) {
        memory = X();
    }

    constexpr void STY(uint8_t &memory) {
        memory = Y();
    }

    // Translate instructions

    constexpr void TAX(uint8_t &) {
        X(updateStatus(A()));
    }

    constexpr void TAY(uint8_t &) {
        Y(updateStatus(A()));
    }

    constexpr void TSX(uint8_t &) {
        X(updateStatus(S()));
    }

    constexpr void TXA(uint8_t &) {
        A(updateStatus(X()));
    }

    constexpr void TXS(uint8_t &) {
        S(X());
    }

    constexpr void TYA(uint8_t &) {
        A(updateStatus(Y()));
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
    uint16_t _stackPointer = 0;

    std::array<uint8_t, 1024 * 2> _ram = {};
};
