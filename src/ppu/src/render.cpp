#include "ppu.hpp"

void PPU::oamScan() {
    uint8_t spriteCount = 0;

    for (int i = 0; i < 40; i++) {
        uint8_t spriteOffset = i * 4;
        
        uint8_t ypos = mmu.oam[spriteOffset + 0];
        uint8_t xpos = mmu.oam[spriteOffset + 1];

        if (ly >= ypos - 16 && ly <= ypos - 8) {
            spriteCount++;

            spriteIndices[spriteCount - 1] = i;

            if (spriteCount >= 10) return;
        } 
    }
}

void PPU::renderLine() {
    constexpr std::array<uint32_t, 4> COLORS = {{ 0xFFFFFFFF, 0xFFD3D3D3, 0xFFA9A9A9, 0xFF000000 }};

    uint8_t row = ly % 8;
    for (int i = 0; i < 20; i++) {
        uint8_t tile = mmu.vram[0x1800 + (ly / 8) * 32 + i];

        uint8_t backgroundLow = mmu.vram[tile * 16 + row * 2 + 0];
        uint8_t backgroundHigh = mmu.vram[tile * 16 + row * 2 + 1];

         for (int pixel = 0; pixel < 8; pixel++) {
            uint8_t bit = 7 - pixel;
            uint8_t lsb = (backgroundLow >> bit) & 1;
            uint8_t msb = (backgroundHigh >> bit) & 1;
            uint8_t index = (msb << 1) | lsb;

            uint8_t pixelX = i * 8 + pixel;
            
            frameBuffer[ly * 160 + pixelX] = COLORS[index];
        }
    }
}