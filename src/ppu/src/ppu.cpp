#include "ppu.hpp"

constexpr unsigned int DOTS_PER_LINE = 456;

PPU::PPU(InterruptSystem& interrupt) : interrupt(interrupt) {}

void PPU::cycle() {
    dots += 4;

    if (dots >= DOTS_PER_LINE) {
        dots -= DOTS_PER_LINE;
        ly++;
        if (ly >= 154) ly -= 154;
        if (ly == 144) interrupt.issueInterrupt(InterruptSource::VBlank); 
    }
}

uint8_t PPU::readLY() const {
    return ly;
}