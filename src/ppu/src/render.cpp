#include "ppu.hpp"

#include <algorithm>

void PPU::oamScan() {
    spriteCount = 0;

    for (int i = 0; i < 40; i++) {
        uint8_t spriteOffset = i * 4;
        
        uint8_t ypos = mmu.oam[spriteOffset + 0];
        uint8_t xpos = mmu.oam[spriteOffset + 1];
        uint8_t tile = mmu.oam[spriteOffset + 2];
        uint8_t attr = mmu.oam[spriteOffset + 3];

        // make sure to check if first = is needed
        if (ly >= static_cast<uint16_t>(ypos - 16) && ly < static_cast<uint16_t>(ypos - 8)) {
            if (spriteCount < 10) {
                sprites[spriteCount++] = Sprite {
                    .tile = tile,
                    .attr = attr,
                    .x = xpos,
                    .y = ypos
                };
            } else {
                break;
            }
        } 
    }

    std::stable_sort(sprites.begin(), sprites.begin() + spriteCount, [](const Sprite& a, const Sprite& b) {
        return a.x < b.x; // reverse order
    });
}

constexpr std::array<uint32_t, 4> COLORS = {{ 0xFFFFFFFF, 0xFFD3D3D3, 0xFFA9A9A9, 0xFF000000 }};

void PPU::pushBackgroundTile(uint8_t tile, uint8_t index, uint8_t row) {
    const uint8_t low = mmu.vram[index * 16 + row * 2 + 0];
    const uint8_t high = mmu.vram[index * 16 + row * 2 + 1];

    for (int pixel = 0; pixel < 8; pixel++) {
        const uint8_t bit = 7 - pixel;
        const uint8_t lsb = (low >> bit) & 1;
        const uint8_t msb = (high >> bit) & 1;
        const uint8_t color = (msb << 1) | lsb;
    
        frameBuffer[ly * 160 + tile * 8 + pixel] = COLORS[color];
    }
}

void PPU::pushObject(const Sprite& spr, uint8_t row) {
    const uint8_t low = mmu.vram[spr.tile * 16 + row * 2 + 0];
    const uint8_t high = mmu.vram[spr.tile * 16 + row * 2 + 1];

    for (int pixel = 0; pixel < 8; pixel++) {
        const uint8_t bit = 7 - pixel;
        const uint8_t lsb = (low >> bit) & 1;
        const uint8_t msb = (high >> bit) & 1;
        const uint8_t color = (msb << 1) | lsb;
    
        if (color > 0) {
            const uint8_t eX = spr.x - 8;
            frameBuffer[ly * 160 + eX + pixel] = COLORS[color];
        }
    }
}

void PPU::renderLine() {
    const uint8_t row = ly % 8;
    
    for (int tile = 0; tile < 20; tile++) {
        const uint8_t index = mmu.vram[0x1800 + (ly / 8) * 32 + tile];
        pushBackgroundTile(tile, index, row);
    }

    for (int sprite = 0; sprite < spriteCount; sprite++) {
        const Sprite& spr = sprites[sprite];
        pushObject(spr, row);
    }
}