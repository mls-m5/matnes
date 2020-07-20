#pragma once

#include "bus.h"
#include "matgui/paint.h"
#include "matgui/view.h"

namespace matnes {

class Ppu : public matnes::IBusComponent, public matgui::View {
public:
    Ppu();
    ~Ppu() noexcept override;

    // Try to write to component, return true if successfull
    virtual bool write(uint16_t address, uint8_t value) override;

    // Try to read a byte return empty if not in range
    virtual std::optional<uint8_t> read(uint16_t address) override;

    void draw() override;

private:
    constexpr bool inRange(uint16_t address) {
        return (address >= 0x2000 && address <= 0x2007) ||
               (address >= 0x4000 && address <= 0x4017);
    }

private:
    matgui::Paint _testPaint;

    uint8_t spriteMemoryData() {
        return 0;
    }

    void spriteMemoryData(uint8_t value) {
        _spriteMemoryAddress = value;
    }

    uint8_t ppuMemoryData() {
        return 0;
    }

    void ppuMemoryData(uint8_t) {
    }

    uint8_t _register1 = 0; // PPUCTRL 2000
    uint8_t _register2 = 0; // PPUMASK 2001
    uint8_t _status = 0;    // PPUSTATUS 2002

    uint8_t _spriteMemoryAddress = 0; // OAMADDR
    uint8_t _ppuMemoryAddress = 0;    // PPUADDR
    uint8_t _backgroundScroll = 0;    // PPUSCROLL
};

} // namespace matnes
