#pragma once

#include "mbc/mbc1.hpp"

#include <cstdint>
#include <vector>
#include <variant>

class ROMOnly {
    private:
        std::vector<uint8_t> rom;

    public:
        ROMOnly() = default;
        ROMOnly(std::vector<uint8_t> rom);

        uint8_t read(uint16_t addr);
};

using Mapper = std::variant<
    ROMOnly,
    MBC1
>;