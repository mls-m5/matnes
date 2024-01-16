#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

namespace matnes {
namespace cartrige {

struct RomData {
    // Structure for INES 2.0
    struct Header {
        uint8_t startBytes[4] = {};
        uint8_t romSize = 0;   // 4
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
RomData loadRom(const std::string &filename);

} // namespace cartrige
} // namespace matnes
