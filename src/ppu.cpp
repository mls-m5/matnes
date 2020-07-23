#include "ppu.h"

#include "matgui/draw.h"

// Registers described
// https://en.wikibooks.org/wiki/NES_Programming
// https://en.wikibooks.org/wiki/NES_Programming/Memory_Map

namespace matnes {

//! Character rom = "pattern memory" ie sprites (mostly)
//! Name table memory"Vram" is much about layout
//! and palette memory is much about... well palette
//! Memory layout of the ppu
//! 0x0-0x1FFF Chrrom
//! 0x2000-0x3FFF Name table memory
//! 0x3F00-0x3FFF Palette memory
//!
//! A tile is 8x8 pixels two planes
//! Low significant bit plane and most significant bit plane
//! Bit value of 0 use to be transparent
//!
//! Pallettes
//! 0x3f00 (background pallette)
//! 0x3f01 Pallette 1
//! 0x3f05 Pellette 2 etc
//!
//! The screen is 256x240 but the scanlines is 341x261
//! Most of the changes to the ppu is done during the vertical blank period
//! The ppu can also trigger interupts to the cpu
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
        address = address - 0x2000;
        address = address % 8;
        switch (address) {
        case 0:
            _ctrl = value;
            return true;
        case 1:
            _mask = value;
            return true;
        case 3:
            _spriteMemoryAddress = value;
            return true;
        case 4:
            spriteMemoryData(value);
            return true;
        case 5:
            _backgroundScroll = value;
            return true;
        case 6:
            _ppuMemoryAddress = value;
            return true;
        case 7:
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
        // Memory is mirrored
        address -= 0x2000;
        address = address % 8;
        switch (address) {
        case 2:
            return _status;
        case 4:
            return spriteMemoryData();
        case 7:
            return ppuMemoryData();
        }
    }
    return {};
}

void Ppu::draw() {
    _testPaint.drawLine(0, 0, width(), height());
}

} // namespace matnes
