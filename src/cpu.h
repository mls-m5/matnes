#pragma once

// http://nesdev.com/6502.txt

#include <array>
#include <cstdint>

namespace matnes {
namespace cpu {

//// Convert between 8 and 16 bit integers

constexpr uint8_t getHigh(uint16_t value) {
    return value >> 8;
}

constexpr uint8_t getLow(uint8_t value) {
    return value;
}

constexpr uint16_t merge(uint8_t a, uint8_t b) {
    return (static_cast<uint16_t>(b) << 8) | a;
}

constexpr bool isDifferentPage(uint16_t a, uint16_t b) {
    return (a >> 8) != (b >> 8);
}

class NotImplementedError : public std::logic_error {
    using logic_error::logic_error;
};

class Cpu {
public:
    constexpr Cpu() = default;

    //! Setup the right registers and values
    constexpr void start() {
        disableInterupts(true);
        decimalFlag(false);
        PCL(ramBig(0xfffc));
        PCL(ramBig(0xfffd));
    }

    void tick();

    // Internal data

    constexpr bool statusFlag(uint8_t index) const {
        return _status & (1 << index);
    }

    //! Set a status flag to a spcific value
    constexpr void statusFlag(uint8_t index, bool value) {
        uint8_t mask = ~(1 << index);
        _status = (_status & mask) | value << index;
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

    [[nodiscard]] constexpr auto PC() const {
        return _programCounter;
    }

    constexpr void PC(uint8_t value) {
        _programCounter = value;
    }

    constexpr void PCH(uint8_t value) {
        _programCounter = (_programCounter & 0xff) | value;
    }

    constexpr void PCL(uint8_t value) {
        _programCounter = (_programCounter & 0xff00) | (value << 8);
    }

    constexpr uint8_t pull() {
        // Todo check if order is correct;
        auto value = ramBig(_stackPointer);
        ++_stackPointer;
        return value;
    }

    constexpr uint16_t pullBig() {
        uint8_t value = pull();
        return value | (pull() << 8);
    }

    constexpr void push(uint8_t value) {
        // Todo: Check if order (decreesing and saving) is correct
        --_stackPointer;
        ramBig(_stackPointer) = value;
    }

    constexpr void pushBig(uint16_t value) {
        push(value >> 8);
        push(value & 0xff);
    }

    //! S is for stack
    [[nodiscard]] constexpr uint8_t S() {
        return _stackPointer;
    }

    constexpr void S(uint8_t value) {
        _stackPointer = value;
    }

    //! P is for processor status
    [[nodiscard]] constexpr uint8_t status() {
        return _status;
    }

    constexpr void status(uint8_t value) {
        _status = value;
    }

    // Status flags ----------------------------

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

    //! This flag does not really exists but sometimes it needs to be set before
    //! pushing the status to the stack
    constexpr auto bFlag(bool value) {
        statusFlag(4, value);
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
        //        return _ram.at(_programCounter + 1);
        return _src1;
    }

    constexpr void src(uint8_t value) {
        _src1 = value;
    }

    constexpr void src2(uint8_t value) {
        _src2 = value;
    }

    constexpr void srcBig(uint16_t value) {
        _src1 = getLow(value);
        _src2 = getHigh(value);
    }

    // Argument 2
    [[nodiscard]] constexpr uint8_t src2() const {
        //        return _ram.at(_programCounter + 2);
        return _src2;
    }

    // Composite src
    constexpr uint16_t srcBig() const {
        return (static_cast<uint16_t>(src2()) << 8) + src();
    }

    // Set sign and zero bits
    constexpr uint8_t updateStatus(uint8_t value) {
        updateSign(value);
        updateZero(!value);
        return value;
    }

    constexpr uint8_t updateOverflow(uint8_t value) {
        overflowFlag(value & 0x80);
        return value;
    }

    //! Note that the argument is int8_t (signed)
    constexpr uint16_t relativeAddress(int8_t value) {
        return _programCounter + value;
    }

    //! Function used by B.. functions
    constexpr void branch(uint8_t offset) {
        ++_extraClockCycles;
        auto newAddress = relativeAddress(offset);
        if (isDifferentPage(_programCounter, newAddress)) {
            ++_extraClockCycles;
        }
        _programCounter = newAddress;
    }

    // ---------------------- Instructions -------------------------------------

    constexpr void ADC(uint8_t &memory) {
        uint8_t tmp = static_cast<uint8_t>(A()) + memory + carry();
        A(updateSign(updateStatus(tmp)));

        // Negative plus positive cannot overflow
        overflowFlag(((tmp ^ A()) & 0x80) && ((tmp ^ memory) & 0x80));
        carry(tmp > 0xff);
    }

    constexpr void AND(uint8_t &memory) {
        A(updateStatus(A() & memory));
    }

    constexpr void ASL(uint8_t &memory) {
        carry(memory & (1 << 7));
        memory <<= 1;
        updateStatus(memory);
    }

    constexpr void BCC(uint8_t &memory) {
        if (!carry()) {
            branch(memory);
        }
    }

    constexpr void BCS(uint8_t &memory) {
        if (carry()) {
            branch(memory);
        }
    }

    constexpr void BEQ(uint8_t &memory) {
        if (zero()) {
            branch(memory);
        }
    }

    constexpr void BIT(uint8_t &memory) {
        updateOverflow(memory);
        updateStatus(memory);
    }

    constexpr void BMI(uint8_t &memory) {
        if (signFlag()) {
            branch(memory);
        }
    }

    constexpr void BNE(uint8_t &memory) {
        if (!zero()) {
            branch(memory);
        }
    }

    constexpr void BPL(uint8_t &memory) {
        if (!signFlag()) {
            branch(memory);
        }
    }

    constexpr void BRK(uint8_t &) {
        ++_programCounter;
        pushBig(_programCounter);
        bFlag(true);
        push(status());
        disableInterupts(true);
        _programCounter = merge(ramBig(0xFFFE), ramBig(0xFFFF));
    }

    constexpr void BVC(uint8_t &memory) {
        if (!overflowFlag()) {
            branch(memory);
        }
    }

    constexpr void BVS(uint8_t &memory) {
        if (overflowFlag()) {
            branch(memory);
        }
    }

    constexpr void CLC(uint8_t &) {
        carry(false);
    }

    constexpr void CLD(uint8_t &) {
        decimalFlag(false);
    }

    constexpr void CLI(uint8_t &) {
        disableInterupts(false);
    }

    constexpr void CLV(uint8_t &) {
        overflowFlag(false);
    }

    constexpr void CMP(uint8_t &memory) {
        auto tmp = static_cast<uint16_t>(A()) - memory;
        updateStatus(tmp);
        carry(tmp > 0xff);
    }

    constexpr void CPX(uint8_t &memory) {
        auto tmp = static_cast<uint16_t>(X()) - memory;
        updateStatus(tmp);
        carry(tmp > 0xff);
    }

    constexpr void CPY(uint8_t &memory) {
        auto tmp = static_cast<uint16_t>(Y()) - memory;
        updateStatus(tmp);
        carry(tmp > 0xff);
    }

    constexpr void DEC(uint8_t &memory) {
        --memory;
    }

    constexpr void DEX(uint8_t &) {
        X(X() - 1);
    }

    constexpr void DEY(uint8_t &) {
        Y(Y() - 1);
    }

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
        pushBig(_programCounter);
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
        bFlag(true);
        push(status());
    }

    constexpr void PLA(uint8_t &) {
        A(pull());
    }

    constexpr void PLP(uint8_t &) {
        status(pull());
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
        status(pull());
        _programCounter = pullBig();
    }

    constexpr void RTS(uint8_t &) {
        _programCounter = pullBig() + 1;
    }

    constexpr void SBC(uint8_t &value) {
        // Subtract with carry
        auto tmp = static_cast<uint16_t>(A()) - value - carry();
        updateStatus(tmp);
        overflowFlag(((A() ^ tmp) & 0x80) && ((A() ^ tmp) & 0x80));
        carry(!(tmp < 0x100)); // Todo: Check so that this is correct
        A(tmp);
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
        return ram(merge(src(), src2()) + X());
    }

    [[nodiscard]] constexpr uint8_t &absoluteY() {
        return ram(merge(src(), src2()) + Y());
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

        auto postAddress = merge(tmp1, tmp2) + Y();

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

    // Use to send in to functions that requires constexpr references
    [[nodiscard]] constexpr uint8_t &dummy() {
        return _dummy;
    }

    constexpr void dummy(uint8_t value) {
        _dummy = value;
    }

    constexpr uint8_t extraClockCycles() const {
        return _extraClockCycles;
    }

private:
    // startup state:
    // https://wiki.nesdev.com/w/index.php/CPU_power_up_state#cite_note-reset-stack-push-3

    // Registers
    uint8_t _dummy = 0; // When need to return a reference
    uint8_t _accumulator = 0;
    uint8_t _xIndex = 0;
    uint8_t _yIndex = 0;
    uint8_t _status = 1 << 5; // also called P
    uint16_t _programCounter = 0;
    uint16_t _stackPointer = 0xfd;
    uint8_t _src1 = 0;
    uint8_t _src2 = 0;

    uint8_t _extraClockCycles = 0; // If instruction demands more time

    std::array<uint8_t, 1024 * 2> _ram = {};
};

} // namespace cpu
} // namespace matnes
