#pragma once

#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <string>

namespace matnes {
namespace cartrige {

constexpr size_t romUnits = 16 * 1024;
constexpr size_t chRomUnits = 8 * 1024;

struct RomData {
    // Structure for INES 2.0
    struct Header {
        uint8_t startBytes[4] = {};
        // Rom size is in 16KB units
        uint8_t romSize = 0; // 4
        // chRomSize is in 8KB units
        uint8_t chRomSize = 0; // 5
        uint8_t flags6 = 0;
        uint8_t flags7 = 0;
        uint8_t flags8 = 0;  // Mapper/submopper
        uint8_t flags9 = 0;  // Prog-rom/chr-rom size MSB
        uint8_t flags10 = 0; // Prog+RAM/EEPROM size

        uint8_t padding[15 - 11] = {};

        constexpr size_t getProgRomSize() {
            auto num = (static_cast<size_t>(flags9) << 8) + romSize;

            return num * 1024 * 16;
        }
    };

    Header header;
    std::string progRomData;
};

RomData loadRom(std::istream &stream);
RomData loadRom(std::filesystem::path);

bool saveRom(const RomData &rom, std::ostream &stream);
bool saveRom(const RomData &rom, std::filesystem::path);

} // namespace cartrige
} // namespace matnes
