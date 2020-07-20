#include "ppu.h"

#include "matgui/draw.h"

// Registers described
// https://en.wikibooks.org/wiki/NES_Programming

namespace matnes {

Ppu::Ppu() {
    _testPaint.line.color(1, 1, 1);
}

Ppu::~Ppu() noexcept {
}

bool Ppu::write(uint16_t address, uint8_t value) {
    //    PPU
    //    $2000 - write only - PPU Control Register 1
    //    $2001 - write only - PPU Control Register 2
    //    $2002 - read only - PPU Status Register
    //    Video control
    //    Sprites
    //    $2003 - write only - Sprite Memory Address
    //    $2004 - read / write - Sprite Memory Data
    //    $2005 - write only - Background Scroll
    //    $2006 - write only - PPU Memory Address - indexing into PPU memory
    //    location $2007 - read / write - PPU Memory Data - data to read from a
    //    PPU memory location or write to a PPU memory location
    if (inRange(address)) {
        switch (address) {
        case 0x2000:
            _register1 = value;
            return true;
        case 0x2001:
            _register2 = value;
            return true;
        case 0x2003:
            _spriteMemoryAddress = value;
            return true;
        case 0x2004:
            spriteMemoryData(value);
            return true;
        case 0x2005:
            _backgroundScroll = value;
            return true;
        case 0x2006:
            _ppuMemoryAddress = value;
            return true;
        case 0x2007:
            ppuMemoryData(value);
            return true;
        }

        return true;
    }
    else {
        return false;
    }
}

std::optional<uint8_t> Ppu::read(uint16_t address) {
    if (inRange(address)) {
        switch (address) {
        case 0x2002:
            return _status;
        case 0x2004:
            return spriteMemoryData();
        case 0x2007:
            return ppuMemoryData();
        }
    }
    return {};
}

void Ppu::draw() {
    _testPaint.drawLine(0, 0, width(), height());
}

} // namespace matnes
