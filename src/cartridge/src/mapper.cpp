#include "mapper.hpp"

ROMOnly::ROMOnly(std::vector<uint8_t> rom) : rom(std::move(rom)) {}

uint8_t ROMOnly::read(uint16_t addr) {
    return rom[addr];
}