#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_memory_editor.h"

#include <SDL3/SDL.h> 
#include <SDL3/SDL_main.h>

#include "gameboy.hpp"
#include "debugger.hpp"

#include <memory>

struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiIO& io;
    SDL_Texture* tilemapTexture;
    SDL_Texture* spritesTexture;
    SDL_Texture* frameBuffer;
    Cartridge cart { "C:\\Users\\tpmac\\MintGB\\MintGB\\roms\\tetris.gb" };
    Gameboy gb;
    Debugger db;
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

    SDL_Texture* tilemapTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        256,
        256
    );

    SDL_Texture* spritesTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        256,
        256
    );

    SDL_Texture* frameBuffer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        160,
        144
    );

    AppState* state = new AppState {
        window,
        renderer,
        io,
        tilemapTexture,
        spritesTexture,
        frameBuffer
    };

    state->gb.insertCartridge(&state->cart);
    state->db.attachGameboy(&state->gb);

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

    SDL_UpdateTexture(state->frameBuffer, nullptr, static_cast<void*>(state->gb.getFramebuffer().data()), 160 * sizeof(uint32_t));

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    CPUState cpuState = state->gb.getCPUState();
    state->db.render();

    static bool running = false;
    static unsigned int stepAmount = 1;
    {
        ImGui::Begin("Control Panel");

        if (ImGui::ArrowButton("stepFrame", ImGuiDir_Up)) {
            state->gb.stepFrame();
        }

        if (ImGui::ArrowButton("step", ImGuiDir_Right)) {
            state->gb.stepInstructions(stepAmount);
        }

        if (ImGui::Button("Run")) {
            running = !running;
        }

        if (running) {
            state->gb.stepFrame();
        }


        ImGui::InputScalar("Step Amount", ImGuiDataType_U32, &stepAmount, nullptr, nullptr, "%d", ImGuiInputTextFlags_CharsDecimal);

        ImGui::End();
    }

    {
        ImGui::Begin("Render Output");

        constexpr float tilesAspect = 1;
        ImVec2 size = ImGui::GetContentRegionAvail();
        
        if (size.x / size.y > tilesAspect) {
            size.x = size.y * tilesAspect;
        } else {
            size.y = size.x / tilesAspect;
        }

        ImGui::Image(
            (ImTextureID)(intptr_t)(state->frameBuffer),
            size
        );

        ImGui::End();
    }

    static JoypadInput input{};

    const bool *keyStates = SDL_GetKeyboardState(nullptr);

    input.a = keyStates[SDL_SCANCODE_Z];
    input.b = keyStates[SDL_SCANCODE_X];
    input.select = keyStates[SDL_SCANCODE_TAB];
    input.start = keyStates[SDL_SCANCODE_RETURN];

    input.right = keyStates[SDL_SCANCODE_D];
    input.left = keyStates[SDL_SCANCODE_A];
    input.up = keyStates[SDL_SCANCODE_W];
    input.down = keyStates[SDL_SCANCODE_S];

    {
        ImGui::Begin("Input");

        ImGui::Text("JOYP: %02X", state->gb.getJoypad());

        ImGui::Checkbox("Up", &input.up);
        ImGui::Checkbox("Down", &input.down); 
        ImGui::Checkbox("Left", &input.left);
        ImGui::Checkbox("Right", &input.right);

        ImGui::Checkbox("A", &input.a);
        ImGui::Checkbox("B", &input.b);
        ImGui::Checkbox("Select", &input.select);
        ImGui::Checkbox("Start", &input.start);

        ImGui::End();
    }
    state->gb.setInput(input);

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
    SDL_DestroyTexture(state->spritesTexture);
    SDL_DestroyTexture(state->tilemapTexture);
    SDL_DestroyTexture(state->frameBuffer);

    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    
    delete static_cast<AppState*>(appstate);
}