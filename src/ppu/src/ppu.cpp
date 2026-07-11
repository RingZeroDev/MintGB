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

    dots = dots % 70224;
    ly = dots / 456;

    switch (stat & 0b11) {
        case 0: {
            if (dots % 456 < 252) {
                if (ly >= 144) {
                    stat &= 0b11111100;
                    stat |= 1;
                    interrupt.issueInterrupt(InterruptSource::VBlank);
                    frameComplete = true;
                } else {
                    stat &= 0b11111100;
                    stat |= 2;
                    oamScan();
                }
            } 
        } break;
        case 1: {
            if (ly < 144) {
                stat &= 0b11111100;
                stat |= 2;
                oamScan();
            }
        } break;
        case 2: {
            if (dots % 456 > 80) {
                stat &= 0b11111100;
                stat |= 3;
                renderLine();
            }
        } break;
        case 3: {
            if (dots % 456 > 369) {
                stat &= 0b11111100;
                stat |= 0;
            }
        } break;
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

std::array<uint32_t, 160 * 144>& PPU::getFramebuffer() {
    return frameBuffer;
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

PPUState PPU::getState() const {
    return PPUState {
        .lcdc = lcdc,
        .ly = ly,
        .stat = stat,
        .scy = scy, .scx = scx,
        .bgp = bgp,
        .obp0 = obp0, 
        .obp1 = obp1, 
        .wy = wy, .wx = wx, 
        .dma = dma 
    };
}

void PPU::setState(const PPUState& state) {
    lcdc = state.lcdc;
    ly = state.ly;
    stat = state.stat;
    scy = state.scy;
    scx = state.scx;
    bgp = state.bgp;
    obp0 = state.obp0;
    obp1 = state.obp1;
    wy = state.wy;
    wx = state.wx;
    dma = state.dma;
}