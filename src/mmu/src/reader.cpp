#include "reader.hpp"

CartReader::CartReader(Cartridge& cart) : cart(cart) {}

uint8_t CartReader::read(uint16_t addr) {
    return cart.read(addr);
}