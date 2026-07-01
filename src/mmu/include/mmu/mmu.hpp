#pragma once

#include "bus.hpp"
#include "cartridge/cartridge.hpp"
#include "ppu/ppu.hpp"
#include "hardware/interrupt.hpp"
#include "hardware/joypad.hpp"
#include "hardware/timer.hpp"

class MMU final : public Bus {
    private:
        Cartridge& cart;
        PPU& ppu;
        InterruptSystem& interrupt;
        Joypad& joypad;
        Timer& timer;

        uint8_t readIO(uint8_t addr);
        void writeIO(uint8_t addr, uint8_t value);
    
    public:
        std::array<uint8_t, 0x2000> vram;
        std::array<uint8_t, 0x2000> wram;
        std::array<uint8_t, 160> oam;
        std::array<uint8_t, 127> hram;

        MMU(Cartridge& cart, PPU& ppu, InterruptSystem& interrupt, Joypad& joypad, Timer& timer);

        uint8_t read(uint16_t addr) override;
        void write(uint16_t addr, uint8_t value) override;
        void cycle() override;
};
