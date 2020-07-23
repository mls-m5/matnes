
#include "loadnes.h"
#include <fstream>

namespace matnes {
namespace cartrige {

RomData loadRom(std::istream &stream) {
    RomData data;

    stream.read(reinterpret_cast<char *>(&data.header),
                sizeof(RomData::Header));

    data.progRomData.resize(data.header.getProgRomSize());

    stream.read(data.progRomData.data(), data.progRomData.size());

    return data;
}

RomData loadRom(const std::string &filename) {
    std::fstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error(filename + " could not be opened");
    }
    return loadRom(file);
}

} // namespace cartrige
} // namespace matnes
