
#include "assembler.h"
#include "instruction-mode.h"
#include <iostream>
#include <regex>
#include <vector>

using namespace matnes::cpu;

namespace matnes {

namespace {

struct Translation {
    Translation(std::string str, MemoryMode mode) : r(str), mode(mode) {
    }

    std::regex r;
    MemoryMode mode;
};

std::vector<Translation> translations = {
    {{R"_((\w\w\w) #\$(\w\w))_"}, Immediate},
    {{R"_((\w\w\w) \$(\w\w)(\w\w))_"}, Absolute},
    {{R"_((\w\w\w) \$(\w\w))_"}, ZeroPage},
    {{R"_((\w\w\w) \$(\w\w)(\w\w),X)_"}, AbsoluteX},
    {{R"_((\w\w\w) \$(\w\w),X)_"}, ZeroPage},
    {{R"_((\w\w\w) \$(\w\w)(\w\w),Y)_"}, AbsoluteX},
    {{R"_((\w\w\w) \$(\w\w),Y)_"}, ZeroPage},
    {{R"_((\w\w\w) \(\$(\w\w),X\))_"}, _IndirectX_},
    {{R"_((\w\w\w) \(\$(\w\w)\),X)_"}, _Indirect_Y},
    {{R"_((\w\w\w))_"}, Implied},
    {{R"_((\w\w\w) A)_"}, Accumulator},
};

} // namespace

std::string translateAssemblyLine(const std::string line) {
    auto matches = std::smatch{};
    auto ret = std::string{};
    for (auto &translation : translations) {
        if (std::regex_match(line, matches, translation.r)) {
            ret.reserve(matches.size() + 1);

            if (matches.empty()) {
                throw std::invalid_argument("syntax error: " + line);
            }

            auto abbreviation = matches[1].str();

            Instruction instruction = getInstructionEnum(abbreviation);

            auto info =
                getInfoFromInstructionAndMode(instruction, translation.mode);

            ret.push_back(info.opCode);

            // Kolla copy-constructorn- det verkar inte kopieras tillbaka
            if (matches.size() - 2 !=
                static_cast<size_t>(info.instructionLength - 1)) {
                for (auto match : matches) {
                    std::cerr << "match: " << match.str() << std::endl;
                }
                throw std::invalid_argument(
                    "wrong number of arguments in: " + line + " got " +
                    std::to_string(matches.size() - 2) + " required " +
                    std::to_string(info.instructionLength + 1));
            }

            for (size_t i = 2; i < matches.size(); ++i) {
                auto str = matches[i].str();
                ret.push_back(std::stoi(str, 0, 16));
            }
            return ret;
        }
    }

    return {};
}

AssembledMemoryT assembleCode(std::istream &stream) {
    auto memory = AssembledMemoryT{};
    auto memoryAddress = size_t{0};

    for (std::string line; getline(stream, line);) {
        if (line.empty()) {
            continue;
        }

        auto raw = translateAssemblyLine(line);

        for (auto c : raw) {
            memory.at(memoryAddress) = c;
            ++memoryAddress;
        }
    }

    return memory;
}

} // namespace matnes
