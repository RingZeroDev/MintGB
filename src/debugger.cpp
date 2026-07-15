#include "debugger.hpp"

#include <stdexcept>

Debugger::Debugger(SDL_Renderer& renderer) : renderer(renderer) {
    tilesTexture = SDL_CreateTexture(
        &renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        128,
        128
    );

    spritesTexture = SDL_CreateTexture(
        &renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        256,
        256
    );

    tilemapTexture = SDL_CreateTexture(
        &renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        256,
        256
    );
}

Debugger::~Debugger() {
    SDL_DestroyTexture(tilesTexture);
    SDL_DestroyTexture(spritesTexture);
    SDL_DestroyTexture(tilemapTexture);
}

void Debugger::attachGameboy(Gameboy* newGB) {
    gb = newGB;
    dasm.attachBus(&gb->getBus());
}

void Debugger::renderCPU() {
    ImGui::Begin("CPU Monitor");                         

    if (ImGui::BeginTable("Registers", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoHostExtendX)) {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::Text("PC");
        ImGui::TableNextColumn();
        ImGui::InputScalar("##Program Counter", ImGuiDataType_U16, &cpuState.pc, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

        ImGui::TableNextColumn();
        ImGui::Text("SP");
        ImGui::TableNextColumn();
        ImGui::InputScalar("##Stack Pointer", ImGuiDataType_U16, &cpuState.sp, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

        ImGui::TableNextColumn();
        ImGui::Text("AF");
        ImGui::TableNextColumn();
        ImGui::InputScalar("##AF Register", ImGuiDataType_U16, &cpuState.af, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);
        
        ImGui::TableNextColumn();
        ImGui::Text("BC");
        ImGui::TableNextColumn();
        ImGui::InputScalar("##BC Register", ImGuiDataType_U16, &cpuState.bc, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

        ImGui::TableNextColumn();
        ImGui::Text("DE");
        ImGui::TableNextColumn();
        ImGui::InputScalar("##DE Register", ImGuiDataType_U16, &cpuState.de, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

        ImGui::TableNextColumn();
        ImGui::Text("HL");
        ImGui::TableNextColumn();
        ImGui::InputScalar("##HL Register", ImGuiDataType_U16, &cpuState.hl, nullptr, nullptr, "%04X", ImGuiInputTextFlags_CharsHexadecimal);

        ImGui::EndTable();
    }

    ImGui::End();
}

void Debugger::renderDisassembly() {
    ImGui::Begin("Disassembly");                         

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

void Debugger::renderMemory() {
    std::array<uint8_t, 0x2000>& vram = gb->getVRAM(); 
    std::array<uint8_t, 0x2000>& wram = gb->getWRAM();
    std::array<uint8_t, 160>& oam = gb->getOAM();
    std::array<uint8_t, 127>& hram = gb->getHRAM();

    ImGui::Begin("Memory");

    if (ImGui::BeginTabBar("Memory", ImGuiTabBarFlags_Reorderable)) {
        if (ImGui::BeginTabItem("VRAM")) {
            memEdit.DrawContents(vram.data(), 0x2000, 0x8000);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("WRAM")) {
            memEdit.DrawContents(wram.data(), 0x2000, 0xC000);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("OAM")) {
            memEdit.DrawContents(oam.data(), 160, 0xFE00);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("HRAM")) {
            memEdit.DrawContents(hram.data(), 127, 0xFF80);
            ImGui::EndTabItem();
        }
    }
    ImGui::EndTabBar();
    
    ImGui::End();
}

void Debugger::renderInterrupts() {
    std::pair<uint8_t, uint8_t> interruptState = gb->getInterruptState();

    ImGui::Begin("Interrupts");
        
    ImGui::Checkbox("IME", &cpuState.ime);
    ImGui::Checkbox("IME Pending", &cpuState.imePending);
    ImGui::InputScalar("IE", ImGuiDataType_U8, &interruptState.first, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("IF", ImGuiDataType_U8, &interruptState.second, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::End();
}

void Debugger::renderPPU() {
    PPUState ppuState = gb->getPPUState();
    
    ImGui::Begin("PPU");

    ImGui::InputScalar("LCDC", ImGuiDataType_U8, &ppuState.lcdc, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("STAT", ImGuiDataType_U8, &ppuState.stat, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("LY", ImGuiDataType_U8, &ppuState.ly, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("LYC", ImGuiDataType_U8, &ppuState.lyc, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("OAM", ImGuiDataType_U8, &ppuState.dma, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputScalar("SCX", ImGuiDataType_U8, &ppuState.scx, nullptr, nullptr, "%02X", ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::End();
}

void Debugger::updateTiles() {
    const std::array<uint8_t, 0x2000>& vram = gb->getVRAM();

    const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);
    const std::array<SDL_Color, 4> TILES_PALLETE = {{ 
        SDL_Color { 255, 255, 255, 255 }, 
        SDL_Color { 211, 211, 211, 255 }, 
        SDL_Color { 169, 169, 169, 255 },
        SDL_Color { 0, 0, 0, 255 } 
    }};  
    for (int tile = 0; tile < 256; tile++) {
        for (int row = 0; row < 8; row++) {
            uint8_t lowByte = vram[tile * 16 + row * 2];
            uint8_t highByte = vram[tile * 16 + row * 2 + 1];

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
    SDL_UpdateTexture(tilesTexture, nullptr, static_cast<void*>(tilesData.data()), 128 * sizeof(uint32_t));
}

void Debugger::updateSprites() {
    const std::array<uint8_t, 160>& oam = gb->getOAM();

    SDL_SetRenderTarget(&renderer, spritesTexture);
    SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255);
    SDL_RenderClear(&renderer);
    for (int i = 0; i < 40; i++) {
        uint8_t ypos = oam[i * 4];
        uint8_t xpos = oam[i * 4 + 1];
        uint8_t tile = oam[i * 4 + 2];
        uint8_t attr = oam[i * 4 + 3];

        uint8_t tileStartX = tile % 16 * 8; 
        uint8_t tileStartY = tile / 16 * 8;

        if (ypos > 0) {
            SDL_FRect src = SDL_FRect { 
                static_cast<float>(tileStartX), 
                static_cast<float>(tileStartY), 
                8, 
                8 
            };
            SDL_FRect dst = SDL_FRect {
                static_cast<float>(xpos - 8),
                static_cast<float>(ypos - 16),
                8,
                8
            };
            SDL_RenderTexture(
                &renderer,
                tilesTexture,
                &src,
                &dst
            );
        }
    }
    SDL_SetRenderTarget(&renderer, nullptr);
}

void Debugger::updateTilemap() {
    const std::array<uint8_t, 0x2000>& vram = gb->getVRAM(); 
    const std::array<uint8_t, 160>& oam = gb->getOAM();

    SDL_SetRenderTarget(&renderer, tilemapTexture);
    for (int i = 0; i < 1024; i++) {
        uint8_t index = vram[0x1800 + i];
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
            &renderer, 
            tilesTexture, 
            &src,
            &dst
        );
    }
    SDL_SetRenderTarget(&renderer, nullptr);
}

void Debugger::renderTiles() {
    ImGui::Begin("Tile Viewer");

    constexpr float tilesAspect = 1;
    ImVec2 size = ImGui::GetContentRegionAvail();
    
    if (size.x / size.y > tilesAspect) {
        size.x = size.y * tilesAspect;
    } else {
        size.y = size.x / tilesAspect;
    }

    ImGui::Image(
        (ImTextureID)(intptr_t)(tilesTexture),
        size
    );

    ImGui::End();
}

void Debugger::renderSprites() {
    ImGui::Begin("Sprite Viewer");

    constexpr float spriteAspect = 1;
    ImVec2 size = ImGui::GetContentRegionAvail();
    
    if (size.x / size.y > spriteAspect) {
        size.x = size.y * spriteAspect;
    } else {
        size.y = size.x / spriteAspect;
    }

    ImGui::Image(
        (ImTextureID)(intptr_t)(spritesTexture),
        size
    );

    ImGui::End();
}

void Debugger::renderTilemap() {
    ImGui::Begin("Tilemap Viewer");

    constexpr float tilemapAspect = 1;
    ImVec2 size = ImGui::GetContentRegionAvail();
    
    if (size.x / size.y > tilemapAspect) {
        size.x = size.y * tilemapAspect;
    } else {
        size.y = size.x / tilemapAspect;
    }

    ImGui::Image(
        (ImTextureID)(intptr_t)(tilemapTexture),
        size
    );

    ImGui::End();
}

void Debugger::renderConverter() {
    ImGui::Begin("Hex Converter");

    ImGui::InputScalar("##Hex Value", ImGuiDataType_U32, &hexValue, nullptr, nullptr, "%X");
    ImGui::Text("Decimal: %d", hexValue);
    ImGui::Text("Octal: %o", hexValue);
    ImGui::Text("Binary: %b", hexValue);

    ImGui::End();
}

void Debugger::render() {
    if (gb == nullptr) {
        throw std::runtime_error("Debugger must have a Gameboy attached");
    }

    cpuState = gb->getCPUState();
    updateTiles();
    updateSprites();
    updateTilemap();

    renderCPU();
    renderDisassembly();
    renderMemory();
    renderInterrupts();
    renderPPU();
    renderTiles();
    renderSprites();
    renderTilemap();
    renderConverter();
}