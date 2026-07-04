#pragma once

#include "cartridge/cartridge.hpp"
#include "cpu/cpu.hpp"
#include "ppu/ppu.hpp"
#include "mmu/mmu.hpp"
#include "hardware/interrupt.hpp"
#include "hardware/timer.hpp"
#include "hardware/joypad.hpp"

class Gameboy {
    private:
        Cartridge* cart;
        InterruptSystem interrupt;
        Joypad joypad{};
        Timer timer{};
        MMU mmu { cart, ppu, interrupt, joypad, timer };
        PPU ppu { mmu, interrupt };
        CPU cpu { mmu, interrupt };

    public:
        Gameboy();

        void insertCartridge(Cartridge* cart);

        void reset();
        void stepFrame();

        void stepInstruction();
        void stepInstructions(unsigned int amount);

        CPUState getCPUState();
        void setCPUState(CPUState state);

        
};