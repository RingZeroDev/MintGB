#include "ppu.hpp"

constexpr unsigned int DOTS_PER_LINE = 456;

PPU::PPU(MMU& mmu, InterruptSystem& interrupt) : mmu(mmu), interrupt(interrupt) {}

void PPU::cycle() {
    dots += 4;

    if (dots >= DOTS_PER_LINE) {
        dots -= DOTS_PER_LINE;
        ly++;
        if (ly >= 154) ly -= 154;
        if (ly == 144) interrupt.issueInterrupt(InterruptSource::VBlank); 
    }

    if (currentDMA < 160) {
        mmu.oam[currentDMA] = mmu.read((dma << 8) | currentDMA);
        currentDMA++;
    }
}

uint8_t PPU::readLY() const {
    return ly;
}

uint8_t PPU::readDMA() {
    return dma;
}

void PPU::writeDMA(uint8_t value) {
    dma = value;
    currentDMA = 0;
}