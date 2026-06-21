#include "disassembler.hpp"
#include "cartridge/cartridge.hpp"
#include "mmu/mmu.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>

int main() {
    Cartridge cart("C:\\Users\\tpmac\\MintGB\\MintGB\\roms\\tetris.gb");
    MMU mmu { cart };

    Disassembler dasm(mmu); 
    auto ins = dasm.disassemble();
    for (std::string str : ins) {
        std::cout << str << "\n";
    }
    
    return EXIT_SUCCESS;
}