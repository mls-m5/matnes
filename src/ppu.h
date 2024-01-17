#pragma once

#include "bus.h"
#include <array>
#include <bit>
#include <cstring>

// Registers described
// https://en.wikibooks.org/wiki/NES_Programming
// https://en.wikibooks.org/wiki/NES_Programming/Memory_Map

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

namespace matnes {

struct Pixel {
    // Check if this is how you should do it
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

struct ScreenMemory {
    static constexpr auto width = 256;
    static constexpr auto height = 250;

    std::array<Pixel, width * height> data;

    Pixel &at(uint8_t x, uint8_t y) {
        return data.at(y * width + x);
    }

    void clearLine(uint8_t y) {
        const auto offset = y * height;
        for (size_t x = 0; x < width; ++x) {
            data.at(x + offset) = {};
        }
    }
};

/// https://www.nesdev.org/wiki/PPU_OAM
struct Sprite {
    static constexpr uint8_t size = 8;

    uint8_t y = 0;
    uint8_t index = 0;
    uint8_t attribute = 0;
    uint8_t x = 0;

    /// @returns true if the sprite was draws or false if not drawn
    constexpr bool drawLine(ScreenMemory &screen, int scanLineY) const {
        auto localY = static_cast<int>(scanLineY) - y;
        if (localY < 0) {
            return false;
        }
        if (localY >= 8) {
            return false;
        }
        auto end = std::min(ScreenMemory::width, x + size);
        for (size_t i = 0; i < 8; ++i) {
            auto screenX = x + i;
            if (screenX >= end) {
                break;
            }
            screen.at(screenX, localY) = {
                255, 255, 255, 255}; // TODO: Make some real implementation here
                                     // with the real image data
        }

        return true;
    }
};

/// The part of the memory that contains sprite information
/// "Object Attribute Memory"
struct PpuOam {
    std::array<Sprite, 64> sprites = {};

    constexpr void write(uint8_t address, uint8_t value) {
        std::bit_cast<uint8_t *>(sprites.data())[address] = value;
    }
};

class Ppu : public matnes::IBusComponent {
public:
    constexpr Ppu() = default;

    /// Making move operators default (when in practice move is copy) is only to
    /// not accidental create copies
    Ppu(const Ppu &) = delete;
    Ppu(Ppu &&) = default;
    Ppu &operator=(const Ppu &) = delete;
    Ppu &operator=(Ppu &&) = default;

    ~Ppu() noexcept override {
    }

    // Try to write to component, return true if successfull
    constexpr bool write(uint16_t address, uint8_t value) override {
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
        //    location $2007 - read / write - PPU Memory Data - data to read
        //    from a PPU memory location or write to a PPU memory location

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

    // Try to read a byte return empty if not in range
    constexpr std::optional<uint8_t> read(uint16_t address) override {
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

    /// TODO: This is a test function, should be replaced with single line
    /// rendering later
    void drawAll() {
        for (uint8_t y = 0; y < _screenMemory.height; ++y) {
            drawLine(y);
        }
    }

private:
    constexpr void drawLine(uint8_t y) {
        constexpr int maxNumSprites = 8;
        auto count = 0;
        for (auto &sprite : _oam.sprites) {
            count += sprite.drawLine(_screenMemory, y);
            if (count > maxNumSprites) {
                break;
            }
        }
    }

    constexpr bool inRange(uint16_t address) {
        /// Note that 0x4017 is located on the cpu, but uploads data to the ppu
        return address >= 0x2000 && address < 0x4000;
    }

    constexpr uint8_t spriteMemoryData() {
        return 0;
    }

    constexpr void spriteMemoryData(uint8_t value) {
        _oam.write(_spriteMemoryAddress, value);
    }

    constexpr void ppuMemoryAddress(uint8_t value) {
        /// TODO: The addres is two bytes. Which byte is controlled by the
        /// w-register
    }

    constexpr uint8_t ppuMemoryData() {
        return 0;
    }

    constexpr void ppuMemoryData(uint8_t) {
    }

    uint8_t _ctrl = 0;             // PPUCTRL 2000
    uint8_t _mask = 0;             // PPUMASK 2001
    uint8_t _status = 0;           // PPUSTATUS 2002
    uint8_t _unused = 0;           //
    uint8_t _unused2 = 0;          //
    uint8_t _backgroundScroll = 0; // PPUSCROLL 2005

    uint8_t _spriteMemoryAddress = 0; // OAMADDR
    uint16_t _ppuMemoryAddress = 0;   // PPUADDR

    /// TODO: Also check internal registers:
    /// https://www.nesdev.org/wiki/PPU_registers#OAMDMA

    PpuOam _oam;
    ScreenMemory _screenMemory;

    friend class PpuView;
};

} // namespace matnes
