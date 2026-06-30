#pragma once

#include <cstdint>

class PPU {
    private:
        uint8_t lcdc = 0x00; // LCD Control

        uint8_t ly = 0x00; // LCD Y coordinate [read only]
        uint8_t lyc = 0x00; // LY compare
        uint8_t stat = 0x00; // LCD status

        uint8_t scy = 0x00, scx = 0x00; // Background viewport Y postition, X position

        uint8_t bgp = 0x00; // BG palette data
        uint8_t obp0 = 0x00, obp1 = 0x00; // OBJ palette 0, 1 data

        uint8_t wy = 0x00, wx = 0x00; // Window Y position, X position plus 7

        unsigned int dots = 0;

    public:
        void cycle(); 

        uint8_t readLY() const;
};