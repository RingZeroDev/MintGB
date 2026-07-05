#include "ppu.hpp"

constexpr unsigned int DOTS_PER_LINE = 456;

PPU::PPU(MMU& mmu, InterruptSystem& interrupt) : mmu(mmu), interrupt(interrupt) {}

void PPU::reset() {
    lcdc = 0x00;

    ly = 0x00; 
    lyc = 0x00; 
    stat = 0x00;

    scy = 0x00, scx = 0x00;

    bgp = 0x00;
    obp0 = 0x00, obp1 = 0x00; 

    wy = 0x00, wx = 0x00; 

    dma = 0x00; 

    frameComplete = false;
    currentDMA = 0;
    dots = 0;
}

void PPU::cycle() {
    dots += 4;

    if (dots >= DOTS_PER_LINE) {
        dots -= DOTS_PER_LINE;
        ly++;
        if (ly >= 154) ly -= 154;
        if (ly == 144) { 
            interrupt.issueInterrupt(InterruptSource::VBlank);
            frameComplete = true;
        } 
    }

    if (currentDMA < 160) {
        mmu.oam[currentDMA] = mmu.read((dma << 8) | currentDMA);
        currentDMA++;
    }
}

bool PPU::isFrameComplete() {
    bool complete = frameComplete;
    frameComplete = false;
    return complete;
}

uint8_t PPU::readControl() const {
    return lcdc;
}

void PPU::writeControl(uint8_t value) {
    lcdc = value;
}

uint8_t PPU::readY() const {
    return ly;
}

uint8_t PPU::readYCompare() const {
    return lyc;
}

void PPU::writeYCompare(uint8_t value) {
    lyc = value;
}

uint8_t PPU::readStatus() const {
    return stat;
}

void PPU::writeStatus(uint8_t value) {
    stat = value & 0b11111000;
}

uint8_t PPU::startDMA() {
    return dma;
}

void PPU::writeDMA(uint8_t value) {
    dma = value;
    currentDMA = 0;
}