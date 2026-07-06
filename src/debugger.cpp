#include "debugger.hpp"

#include <stdexcept>

void Debugger::attachGameboy(Gameboy* newGB) {
    gb = newGB;
    dasm.attachBus(&gb->getBus());
}

void Debugger::renderCPU() {
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

void Debugger::renderMemory(
    std::array<uint8_t, 0x2000>& vram,
    std::array<uint8_t, 0x2000>& wram,
    std::array<uint8_t, 160>& oam,
    std::array<uint8_t, 127>& hram
) {
    {
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
}

void Debugger::render() {
    if (gb == nullptr) {
        throw std::runtime_error("Debugger must have a Gameboy attached");
    }

    cpuState = gb->getCPUState();

    std::array<uint8_t, 0x2000>& vram = gb->getVRAM(); 
    std::array<uint8_t, 0x2000>& wram = gb->getWRAM();
    std::array<uint8_t, 160>& oam = gb->getOAM();
    std::array<uint8_t, 127>& hram = gb->getHRAM();

    renderCPU();
    renderDisassembly();
    renderMemory(vram, wram, oam, hram);
}