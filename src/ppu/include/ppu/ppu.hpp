#pragma once

#include "mmu/mmu.hpp"
#include "hardware/interrupt.hpp"

#include <cstdint>

class MMU;

class PPU {
    private:
        MMU& mmu;
        InterruptSystem& interrupt;

        uint8_t lcdc = 0x00; // LCD Control

        uint8_t ly = 0x00; // LCD Y coordinate [read only]
        uint8_t lyc = 0x00; // LY compare
        uint8_t stat = 0x00; // LCD status

        uint8_t scy = 0x00, scx = 0x00; // Background viewport Y postition, X position

        uint8_t bgp = 0x00; // BG palette data
        uint8_t obp0 = 0x00, obp1 = 0x00; // OBJ palette 0, 1 data

        uint8_t wy = 0x00, wx = 0x00; // Window Y position, X position plus 7

        uint8_t dma = 0x00; // DMA: OAM DMA source address and start

        bool frameComplete = false;
        uint8_t currentDMA = 0;
        unsigned int dots = 0;

    public:
        PPU(MMU& mmu, InterruptSystem& interrupt);

        void reset();
        void cycle(); 
        bool isFrameComplete();

        uint8_t readControl() const;
        void writeControl(uint8_t value);

        uint8_t readY() const;

        uint8_t readYCompare() const;
        void writeYCompare(uint8_t value);

        uint8_t readStatus() const;
        void writeStatus(uint8_t value);

        uint8_t startDMA();
        void writeDMA(uint8_t value);
};