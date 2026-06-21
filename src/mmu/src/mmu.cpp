#include "mmu.hpp" 

MMU::MMU(Cartridge& cart) : cart(cart) {}

uint8_t MMU::read(uint16_t addr) {
    if (addr >= 0x0000 && addr <= 0x7FFF) {
        return cart.read(addr);
    } else {
        throw std::runtime_error("Unimplemented!");
    }
}

void MMU::write(uint16_t addr, uint8_t value) {}

void MMU::cycle() {}