#pragma once

#include "gameboy.hpp"
#include "disassembler/disassembler.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_memory_editor.h"

class Debugger {
    private:
        Gameboy* gb;

        CPUState cpuState;

        Disassembler dasm;
        MemoryEditor memEdit;

        void renderCPU();
        void renderDisassembly();
        void renderMemory(std::array<uint8_t, 0x2000>& vram, 
                          std::array<uint8_t, 0x2000>& wram, 
                          std::array<uint8_t, 160>& oam, 
                          std::array<uint8_t, 127>& hram
                         );

    public:
        void attachGameboy(Gameboy* newGB);
        void render();
};
