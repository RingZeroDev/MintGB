#include "disassembler.hpp"
#include "cartridge/cartridge.hpp"
#include "mmu/reader.hpp"
#include "ppu/ppu.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>

int main() {
    Cartridge cart("C:\\Users\\tpmac\\MintGB\\MintGB\\roms\\tetris.gb");
    CartReader reader { cart };

    Disassembler dasm;
    dasm.attachBus(&reader);

    auto ins = dasm.disassemble();
    for (std::string str : ins) {
        std::cout << str << "\n";
    }
    
    return EXIT_SUCCESS;
}