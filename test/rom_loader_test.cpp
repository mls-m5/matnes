#include <iostream>
#include <loadnes.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "to few arguments, please specify rom file\n";
        return 1;
    }

    auto data = matnes::cartrige::loadRom(argv[1]);

    std::cout << std::string(data.header.startBytes, data.header.startBytes + 4)
              << std::endl;

    std::cout << "rom size " << data.header.getProgRomSize() << std::endl;

    return 0;
}
