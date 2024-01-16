#pragma once

#include "bus.h"
#include <array>

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

    constexpr void drawLine(ScreenMemory &screen, int scanLineY) const {
        auto localY = static_cast<int>(scanLineY) - y;
        if (localY < 0) {
            return;
        }
        if (localY >= 8) {
            return;
        }
        auto end = std::min(ScreenMemory::width, x + size);
        for (size_t x = 0; x < end; ++x) {
            screen.at(x, localY) = {
                255, 255, 255, 255}; // TODO: Make some real implementation here
                                     // with the real image data
        }
    }
};

/// The part of the memory that contains sprite information
struct PpuOam {
    std::array<Sprite, 64> sprites;
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
    bool write(uint16_t address, uint8_t value) override;

    // Try to read a byte return empty if not in range
    std::optional<uint8_t> read(uint16_t address) override;

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
            sprite.drawLine(_screenMemory, y);
            ++count;
            if (count > maxNumSprites) {
                break;
            }
        }
    }

    constexpr bool inRange(uint16_t address) {
        return address >= 0x2000 && address < 4000;
        //        return (address >= 0x2000 && address <= 0x2007) ||
        //               (address >= 0x4000 && address <= 0x4017);
    }

    constexpr uint8_t spriteMemoryData() {
        return 0;
    }

    constexpr void spriteMemoryData(uint8_t value) {
        _spriteMemoryAddress = value;
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
    uint8_t _ppuMemoryAddress = 0;    // PPUADDR

    PpuOam _oam;
    ScreenMemory _screenMemory;

    friend class PpuView;
};

} // namespace matnes
