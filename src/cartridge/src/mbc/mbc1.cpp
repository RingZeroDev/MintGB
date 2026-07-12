#include "mbc/mbc1.hpp"

#include <stdexcept>

MBC1::MBC1(std::vector<uint8_t> rom, std::vector<uint8_t> ram) : rom(std::move(rom)), ram(std::move(ram)) {}

uint8_t MBC1::read(uint16_t addr) {
    if (addr <= 0x3FFF) {
        return rom[addr];
    } else if (addr <= 0x7FFF) {
        const size_t eAddr = romBank * 0x4000 - 0x4000 + addr; 
        return rom[eAddr];
    } else if (addr <= 0xBFFF) {
        const size_t eAddr = ramBank * 0x2000 - 0xA000 + addr;
        return ram[eAddr];
    } else {
        throw std::runtime_error("Exceeded viable cartridge memory range.");
    }
}

void MBC1::write(uint16_t addr, uint8_t value) {
    if (addr <= 0x1FFF) {
        ramEnable = (value & 0b1111) == 0xA;
    } else if (addr <= 0x3FFF) {
        romBank = value & 0b11111;
        if (romBank == 0) romBank = 1;
    } else if (addr <= 0x5FFF) {
        ramBank = value & 0b11;
    } else if (addr <= 0x7FFF) {
        bankingMode = value & 1;
    } else if (addr <= 0xBFFF) {
        const size_t eAddr = ramBank * 0x2000 - 0xA000 + addr;
        ram[eAddr] = value;
    }
}