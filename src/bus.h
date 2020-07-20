#pragma once

#include <array>
#include <cstdint>
#include <optional>

namespace matnes {

class IBusComponent {
public:
    virtual ~IBusComponent() = default;

    // Try to write to component, return true if successfull
    virtual bool write(uint16_t address, uint8_t value) = 0;

    // Try to read a byte return empty if not in range
    virtual std::optional<uint8_t> read(uint16_t address) = 0;
};

//! A bus from the cpu to memory etc.
//! The 2k register is built in to make the raw buss constexpr
//! for testing
class Bus {
    constexpr Bus() = default;
    constexpr Bus(const Bus &) = default;
    constexpr Bus &operator=(const Bus &) = default;

    constexpr void write(uint16_t address, uint8_t value) {
        if (address < _ram.size() * 4) {
            // Ram is mirrored 4 times
            _ram.at(address % _ram.size());
        }
        else {
            for (size_t i = 0; i < _nConnections; ++i) {
                if (_components[i]->write(address, value)) {
                    return;
                }
            }
        }
    }

    constexpr uint8_t read(uint16_t address) {
        if (address < _ram.size() * 4) {
            // Ram is mirrored 4 times
            return _ram.at(address % _ram.size());
        }
        else {
            for (size_t i = 0; i < _nConnections; ++i) {
                if (auto data = _components[i]->read(address)) {
                    return *data;
                }
            }
        }

        return 0;
    }

private:
    std::array<uint8_t, 1024 * 2> _ram = {};
    std::array<IBusComponent *, 16> _components = {};
    size_t _nConnections = 0;
};

} // namespace matnes
