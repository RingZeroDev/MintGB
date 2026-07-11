#include "cartridge/cartridge.hpp"

#include <iostream>
#include <format>

int main(int argc, char* argv[]) {
    Cartridge cart { argv[1] };
    CartridgeMetadata meta = cart.getMetadata(); 

    std::cout << std::format("Title: {}", meta.title) << std::endl;
    std::cout << std::format("Version Number: {:d}", meta.versionNumber) << std::endl;
    std::cout << std::format("Manufacturer Code: {}", meta.manufacturerCode) << std::endl;
    std::cout << std::format("Licensee: {}", meta.licensee) << std::endl;
    std::cout << std::format("Type: {}", meta.type) << std::endl;
    std::cout << std::format("ROM Size: {:d}", meta.romSize) << std::endl;
    std::cout << std::format("RAM Size: {:d}", meta.ramSize) << std::endl;
    std::cout << std::format("Header Checksum: {:02X}", meta.headerChecksum) << std::endl;
    std::cout << std::format("Global Checksum: {:04X}", meta.globalChecksum) << std::endl;
    std::cout << std::format("Calculated Header Checksum: {:02X}", cart.calculateHeaderChecksum()) << std::endl;
    std::cout << std::format("Calculated Global Checksum: {:02X}", cart.calculateGlobalChecksum()) << std::endl;

    return EXIT_SUCCESS;
}