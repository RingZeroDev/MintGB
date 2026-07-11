#pragma once

#include "gameboy.hpp"
#include "disassembler/disassembler.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_memory_editor.h"

#include <SDL3/SDL.h> 

class Debugger {
    private:
        Gameboy* gb;
        SDL_Renderer& renderer;

        Disassembler dasm;
        MemoryEditor memEdit;
        
        CPUState cpuState;

        std::array<uint32_t, 128 * 128> tilesData{};
        SDL_Texture* tilesTexture;

        SDL_Texture* spritesTexture;
        SDL_Texture* tilemapTexture;

        void updateTiles();
        void updateSprites();
        void updateTilemap();

        void renderCPU();
        void renderDisassembly();
        void renderMemory();
        void renderInterrupts();
        void renderPPU();
        void renderTiles();
        void renderSprites();
        void renderTilemap();

    public:
        Debugger(SDL_Renderer& renderer);
        ~Debugger();

        void attachGameboy(Gameboy* newGB);
        void render();
};
