#include "disassembler.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>

int main() {
    std::vector<uint8_t> buffer(1000);

    std::ifstream f("C:\\Users\\tpmac\\MintGB\\MintGB\\roms\\tetris.gb", std::ios_base::binary);
    if (f) {
        f.read(reinterpret_cast<char*>(buffer.data()), 1000);
    }

    Disassembler dasm(buffer.data()); 
    auto ins = dasm.disassemble();
    for (std::string str : ins) {
        std::cout << str << "\n";
    }
    
    return EXIT_SUCCESS;
}