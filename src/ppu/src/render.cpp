#include "ppu.hpp"

#include <algorithm>

struct Pixel {
    uint8_t color;
    uint8_t palette;
    uint8_t priority;
};

class FIFO {
    private:
        std::array<Pixel, 16> pixels {};
        uint8_t head = 0;
        uint8_t tail = 0;

    public:
        void push(const Pixel& spr) {
            pixels[tail] = spr;
            tail = (tail + 1) % 16;

        }

        const Pixel& pop() {
            const Pixel& pixel = pixels[head];
            head = (head + 1) % 16;
            return pixel;
        }
};

void PPU::oamScan() {
    spriteCount = 0;

    for (int i = 0; i < 40; i++) {
        uint8_t spriteOffset = i * 4;
        
        uint8_t ypos = mmu.oam[spriteOffset + 0];
        uint8_t xpos = mmu.oam[spriteOffset + 1];
        uint8_t tile = mmu.oam[spriteOffset + 2];
        uint8_t attr = mmu.oam[spriteOffset + 3];

        if (ly >= static_cast<uint16_t>(ypos - 16) && ly <= static_cast<uint16_t>(ypos - 8)) {
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
        return a.x < b.x;
    });
}

void PPU::pushTile(FIFO& fifo, uint8_t index, uint8_t row) {
    uint8_t low = mmu.vram[index * 16 + row * 2 + 0];
    uint8_t high = mmu.vram[index * 16 + row * 2 + 1];

    for (int pixel = 0; pixel < 8; pixel++) {
        uint8_t bit = 7 - pixel;
        uint8_t lsb = (low >> bit) & 1;
        uint8_t msb = (high >> bit) & 1;
        uint8_t color = (msb << 1) | lsb;
    
        fifo.push(Pixel {
            .color = color,
            .palette = 0,
            .priority = 0
        });
    }
}

std::array<uint32_t, 4> COLORS = {{ 0xFFFFFFFF, 0xFFD3D3D3, 0xFFA9A9A9, 0xFF000000 }};

void PPU::renderLine() {
    FIFO backgroundPixels {};
    FIFO objectPixels {};

    uint8_t currentSprite = 0;
    uint8_t nextX = sprites[currentSprite].x - 8;

    uint8_t row = ly % 8;
    for (int i = 0; i < 20; i++) {
        const uint8_t startX = i * 8;
        const uint8_t endX = i * 8 + 7;
        
        uint8_t tile = mmu.vram[0x1800 + (ly / 8) * 32 + i];
        pushTile(backgroundPixels, tile, row);

        if (spriteCount > 0 && nextX >= startX && nextX <= endX) {
            for (int i = 0; i < (nextX - startX); i++) {
                objectPixels.push(Pixel {
                    .color = 0
                });
            }
            const Sprite& spr = sprites[currentSprite++];
            pushTile(objectPixels, spr.tile, row);
            nextX = sprites[currentSprite].x - 8;  
        } else {
            for (int i = 0; i < 8; i++) {
                objectPixels.push(Pixel {
                    .color = 0
                });
            }
        }

        for (int pixel = 0; pixel < 8; pixel++) {
            const Pixel& backgroundPixel = backgroundPixels.pop();
            const Pixel& objectPixel = objectPixels.pop();

            if (objectPixel.color == 0) {
                frameBuffer[ly * 160 + i * 8 + pixel] = COLORS[backgroundPixel.color];
            } else {
                frameBuffer[ly * 160 + i * 8 + pixel] = COLORS[objectPixel.color];
            }
        }
    }
}