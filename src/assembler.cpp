
#include "assembler.h"

#include <regex>
#include <vector>

namespace {

struct Translation {
    Translation(std::string str, uint8_t op) : r(str), opCode(op) {
    }

    std::regex r;
    uint8_t opCode;
};

std::vector<Translation> translations = {
    {{R"_(ADC #\$(\w\w))_"}, 0x69},
};

} // namespace

std::string translateAssemblyLine(const std::string line) {
    std::smatch matches;
    std::string ret;
    for (auto &translation : translations) {
        if (std::regex_match(line, matches, translation.r)) {
            ret.reserve(matches.size() + 1);

            ret.push_back(translation.opCode);
            for (size_t i = 1; i < matches.size(); ++i) {
                auto str = matches[i].str();
                ret.push_back(std::stoi(str, 0, 16));
            }
            return ret;
        }
    }

    return {};
}

Assembler::Assembler(std::istream &stream) : _stream(stream) {
    for (std::string line; getline(stream, line);) {
        if (line.empty()) {
            continue;
        }

        //        auto raw = translateAssemblyLine();
    }
}
