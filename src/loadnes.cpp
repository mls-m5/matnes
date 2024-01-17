
#include "loadnes.h"
#include <bit>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace matnes {
namespace cartrige {

RomData loadRom(std::istream &stream) {
    RomData data;

    stream.read(reinterpret_cast<char *>(&data.header),
                sizeof(RomData::Header));

    data.progRomData.resize(data.header.getProgRomSize() * romUnits);

    stream.read(data.progRomData.data(), data.progRomData.size());

    return data;
}

RomData loadRom(std::filesystem::path path) {
    std::fstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(path.string() + " could not be opened");
    }
    return loadRom(file);
}

bool saveRom(const RomData &rom, std::filesystem::path path) {
    auto file = std::ofstream{path};
    if (!file.is_open()) {
        throw std::runtime_error{path.string() + " could not be saved"};
    }
    return saveRom(rom, file);
}

bool saveRom(const RomData &rom, std::ostream &stream) {
    auto header = rom.header;
    header.romSize = rom.progRomData.size() / romUnits;

    stream.write(std::bit_cast<char *>(&header), sizeof(header));
    stream.write(rom.progRomData.data(), rom.progRomData.size());

    return true;
}

} // namespace cartrige
} // namespace matnes
