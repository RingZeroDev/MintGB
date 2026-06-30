#include "ppu.hpp"

constexpr unsigned int DOTS_PER_FRAME = 456 * 154;

void PPU::cycle() {
    dots += 4;

    if (dots >= DOTS_PER_FRAME) {
        dots -= DOTS_PER_FRAME;
    }
}

uint8_t PPU::readLY() const {
    return dots / 456;
}