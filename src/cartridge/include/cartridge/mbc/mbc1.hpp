#pragma once

#include <cstdint>
#include <vector>

class MBC1 {
    private:
        std::vector<uint8_t> rom;
        std::vector<uint8_t> ram;

        bool ramEnable = false;
        bool bankingMode = false;

        uint8_t romBank = 0x00;
        uint8_t ramBank = 0x00;

    public:
        MBC1(std::vector<uint8_t> rom, std::vector<uint8_t> ram);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
};