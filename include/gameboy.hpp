#pragma once

#include "cartridge/cartridge.hpp"
#include "cpu/cpu.hpp"
#include "ppu/ppu.hpp"
#include "mmu/mmu.hpp"
#include "hardware/interrupt.hpp"
#include "hardware/timer.hpp"
#include "hardware/joypad.hpp"

#include <utility>

class Gameboy {
    private:
        Cartridge* cart;
        InterruptSystem interrupt;
        Joypad joypad {};
        Timer timer { interrupt };
        MMU mmu { ppu, interrupt, joypad, timer };
        PPU ppu { mmu, interrupt };
        CPU cpu { mmu, interrupt };

    public:
        void insertCartridge(Cartridge* cart);

        void reset();
        void stepFrame();

        void stepInstruction();
        void stepInstructions(unsigned int amount);

        CPUState getCPUState();
        void setCPUState(CPUState state);

        TimerState getTimerState();
        void setTimerState(TimerState state);

        JoypadInput getInput();
        void setInput(JoypadInput input);
        uint8_t getJoypad();
        void setJoypad(uint8_t value);

        std::pair<uint8_t, uint8_t> getInterruptState();
        void setInterruptState(std::pair<uint8_t, uint8_t> state);
        void setInterruptCallback(InterruptCallback newCallback);

        const std::array<uint8_t, 0x2000>& getVRAM() const;
        const std::array<uint8_t, 0x2000>& getWRAM() const;
        const std::array<uint8_t, 160>& getOAM() const;
        const std::array<uint8_t, 127>& getHRAM() const;

        std::array<uint8_t, 0x2000>& getVRAM();
        std::array<uint8_t, 0x2000>& getWRAM();
        std::array<uint8_t, 160>& getOAM(); 
        std::array<uint8_t, 127>& getHRAM(); 
};