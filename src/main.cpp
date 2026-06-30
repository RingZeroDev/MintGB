#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_memory_editor.h"

#include <SDL3/SDL.h> 
#include <SDL3/SDL_main.h>

#include "cpu/cpu.hpp"
#include "mmu/mmu.hpp"
#include "ppu/ppu.hpp"
#include "cartridge/cartridge.hpp"
#include "hardware/interrupt.hpp"
#include "disassembler/disassembler.hpp"

#include <memory>

struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiIO& io;
    SDL_Texture* tilesTexture;
    SDL_Texture* tilemapTexture;
    Cartridge cart { "C:\\Users\\tpmac\\MintGB\\MintGB\\roms\\tetris.gb" };
    InterruptSystem interrupt;
    PPU ppu { interrupt };
    MMU mmu { cart, ppu, interrupt };
    CPU cpu { mmu, interrupt };
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("MintGB", (int)(1280 * mainScale), (int)(800 * mainScale), windowFlags, &window, &renderer)) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderVSync(renderer, 1);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    SDL_Texture* tilesTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        128,
        128
    );

    SDL_Texture* tilemapTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        256,
        256
    );

    AppState* state = new AppState {
        window,
        renderer,
        io,
        tilesTexture,
        tilemapTexture
    };

    state->interrupt.writeIE(static_cast<uint8_t>(InterruptSource::VBlank));

    *appstate = static_cast<void*>(state);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    ImGui_ImplSDL3_ProcessEvent(event);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    AppState* state {
        static_cast<AppState*>(appstate)
    };

    if (SDL_GetWindowFlags(state->window) & SDL_WINDOW_MINIMIZED) {
        SDL_Delay(10);
        return SDL_APP_CONTINUE;
    }

    const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);
    static std::array<uint32_t, 128 * 128> tilesData{};
    const std::array<SDL_Color, 4> TILES_PALLETE = {{ 
        SDL_Color { 255, 255, 255, 255 }, 
        SDL_Color { 211, 211, 211, 255 }, 
        SDL_Color { 169, 169, 169, 255 },
        SDL_Color { 0, 0, 0, 255 } 
    }};  
    for (int tile = 0; tile < 256; tile++) {
        for (int row = 0; row < 8; row++) {
            uint8_t lowByte = state->mmu.vram[tile * 16 + row * 2];
            uint8_t highByte = state->mmu.vram[tile * 16 + row * 2 + 1];

            for (int pixel = 0; pixel < 8; pixel++) {
                uint8_t bit = 7 - pixel;
                uint8_t lsb = (lowByte >> bit) & 1;
                uint8_t msb = (highByte >> bit) & 1;
                uint8_t index = (msb << 1) | lsb;

                uint8_t tileX = tile % 16;
                uint8_t tileY = tile / 16;
                uint8_t pixelX = tileX * 8 + pixel;
                uint8_t pixelY = tileY * 8 + row;
                SDL_Color color = TILES_PALLETE[index];
                tilesData[pixelY * 128 + pixelX] = SDL_MapRGBA(format, nullptr, color.r, color.g, color.b, color.a);
            }
        }
    }
    SDL_UpdateTexture(state->tilesTexture, nullptr, static_cast<void*>(tilesData.data()), 128 * sizeof(uint32_t));

    SDL_SetRenderTarget(state->renderer, state->tilemapTexture);
    for (int i = 0; i < 1024; i++) {
        uint8_t index = state->mmu.vram[0x1800 + i];
        uint8_t tileStartX = index % 16 * 8; 
        uint8_t tileStartY = index / 16 * 8;

        uint8_t mapStartX = i % 32 * 8;
        uint8_t mapStartY = i / 32 * 8; 

        SDL_FRect src = SDL_FRect { 
            static_cast<float>(tileStartX), 
            static_cast<float>(tileStartY), 
            8, 
            8 
        };
        SDL_FRect dst = SDL_FRect {
            static_cast<float>(mapStartX),
            static_cast<float>(mapStartY),
            8,
            8
        };
        SDL_RenderTexture(
            state->renderer, 
            state->tilesTexture, 
            &src,
            &dst
        );
    }
    SDL_SetRenderTarget(state->renderer, nullptr);

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    CPUState cpuState = state->cpu.state();
    {
        ImGui::Begin("CPU Monitor");                         

        if (ImGui::BeginTable("Registers", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX)) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("PC");
            ImGui::TableNextColumn();
            ImGui::Text("%04X", cpuState.pc);

            ImGui::TableNextColumn();
            ImGui::Text("SP");
            ImGui::TableNextColumn();
            ImGui::Text("%04X", cpuState.sp);

            ImGui::TableNextColumn();
            ImGui::Text("AF");
            ImGui::TableNextColumn();
            ImGui::Text("%04X", cpuState.af);
            
            ImGui::TableNextColumn();
            ImGui::Text("BC");
            ImGui::TableNextColumn();
            ImGui::Text("%04X", cpuState.bc);

            ImGui::TableNextColumn();
            ImGui::Text("DE");
            ImGui::TableNextColumn();
            ImGui::Text("%04X", cpuState.de);

            ImGui::TableNextColumn();
            ImGui::Text("HL");
            ImGui::TableNextColumn();
            ImGui::Text("%04X", cpuState.hl);

            ImGui::EndTable();
        }

        ImGui::End();
    }

    static unsigned int stepAmount = 1;
    {
        ImGui::Begin("Control Panel");

        if (ImGui::ArrowButton("step", ImGuiDir_Right)) {
            for (int i = 0; i < stepAmount; i++) {
                state->cpu.step();
            }
        }


        ImGui::InputScalar("Step Amount", ImGuiDataType_U32, &stepAmount, nullptr, nullptr, "%d", ImGuiInputTextFlags_CharsDecimal);

        ImGui::End();
    }

    {
        ImGui::Begin("Disassembly");                         

        Disassembler dasm(state->mmu);
        dasm.setCurrentAddr(cpuState.pc);

        std::vector<std::string> ins = dasm.disassemble(20);
        for (int i = 0; i < ins.size(); i++) {
            if (i == 0) {
                ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), ins[i].c_str());
            } else {
                ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), ins[i].c_str());
            }
        }

        ImGui::End();
    }

    static MemoryEditor memEdit;
    {
        ImGui::Begin("Memory");

        if (ImGui::BeginTabBar("Memory", ImGuiTabBarFlags_Reorderable)) {
            if (ImGui::BeginTabItem("VRAM")) {
                memEdit.DrawContents(state->mmu.vram.data(), 0x2000, 0x8000);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("WRAM")) {
                memEdit.DrawContents(state->mmu.wram.data(), 0x2000, 0xC000);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("OAM")) {
                memEdit.DrawContents(state->mmu.oam.data(), 160, 0xFE00);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("HRAM")) {
                memEdit.DrawContents(state->mmu.hram.data(), 127, 0xFF80);
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
        ImGui::End();
    }

    {
        ImGui::Begin("Interrupts");
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("IME: {}", cpuState.ime).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("IME Pending: {}", cpuState.imePending).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("IE: {:02X}", state->interrupt.readIE()).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("IF: {:02X}", state->interrupt.readIF()).c_str());

        if (ImGui::Button("Raise VBlank")) {
            state->interrupt.issueInterrupt(InterruptSource::VBlank);
        }

        ImGui::End();
    }

    {
        ImGui::Begin("PPU");

        ImGui::Text("LY: %04X", state->ppu.readLY());

        ImGui::End();
    }

    {
        ImGui::Begin("Tile Viewer");

        constexpr float tilesAspect = 1;
        ImVec2 size = ImGui::GetContentRegionAvail();
        
        if (size.x / size.y > tilesAspect) {
            size.x = size.y * tilesAspect;
        } else {
            size.y = size.x / tilesAspect;
        }

        ImGui::Image(
            (ImTextureID)(intptr_t)(state->tilesTexture),
            size
        );

        ImGui::End();
    }

    {
        ImGui::Begin("Tilemap Viewer");

        constexpr float tilemapAspect = 1;
        ImVec2 size = ImGui::GetContentRegionAvail();
        
        if (size.x / size.y > tilemapAspect) {
            size.x = size.y * tilemapAspect;
        } else {
            size.y = size.x / tilemapAspect;
        }

        ImGui::Image(
            (ImTextureID)(intptr_t)(state->tilemapTexture),
            size
        );

        ImGui::End();
    }

    ImGui::Render();
    SDL_SetRenderScale(state->renderer, state->io.DisplayFramebufferScale.x, state->io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColorFloat(state->renderer, 0.45f, 0.55f, 0.60f, 1.00f);
    SDL_RenderClear(state->renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), state->renderer);
    SDL_RenderPresent(state->renderer);
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AppState* state {
        static_cast<AppState*>(appstate)
    };

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(state->tilesTexture);
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    
    delete static_cast<AppState*>(appstate);
}