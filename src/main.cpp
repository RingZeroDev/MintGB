#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h> 
#include <SDL3/SDL_main.h>

#include "cpu/cpu.hpp"
#include "mmu/mmu.hpp"
#include "cartridge/cartridge.hpp"
#include "disassembler/disassembler.hpp"

#include <memory>

struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiIO& io;
    Cartridge cart { "C:\\Users\\tpmac\\MintGB\\MintGB\\roms\\tetris.gb" };
    MMU mmu { cart };
    CPU cpu { mmu };
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

    AppState* state = new AppState {
        window,
        renderer,
        io
    };

    for (int i = 0; i < 20; i++) {
        state->cpu.step();
    }

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

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    CPUState cpuState = state->cpu.state();
    {
        ImGui::Begin("CPU Monitor");                         

        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("A: {:02X}", cpuState.a).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("B: {:02X}", cpuState.b).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("C: {:02X}", cpuState.c).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("D: {:02X}", cpuState.d).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("E: {:02X}", cpuState.e).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("H: {:02X}", cpuState.h).c_str());
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("L: {:02X}", cpuState.l).c_str());             
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), std::format("F: {:02X}", cpuState.f).c_str());

        if (ImGui::ArrowButton("step", ImGuiDir_Right)) {
            state->cpu.step();
        }

        if (ImGui::ArrowButton("step100", ImGuiDir_Right)) {
            for (int i = 0; i < 100; i++) {
                state->cpu.step();
            }
        }

        ImGui::End();
    }

    {
        ImGui::Begin("Disassembly");                         

        Disassembler dasm(state->mmu);
        dasm.setCurrentAddr(cpuState.pc+10);

        std::vector<std::string> ins = dasm.disassemble(20);
        for (int i = 0; i < ins.size(); i++) {
            if (i == 10) {
                ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), ins[i].c_str());
            } else {
                ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), ins[i].c_str());
            }
        }

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

    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    
    delete static_cast<AppState*>(appstate);
}