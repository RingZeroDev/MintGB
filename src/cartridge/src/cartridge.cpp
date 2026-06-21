#include "cartridge.hpp"

#include <format>

Cartridge::Cartridge(const char* path) {
    size_t fileSize = std::filesystem::file_size(path);
    
    buffer = std::vector<uint8_t>(fileSize);

    std::ifstream f(path, std::ios_base::binary);
    if (f) {
        f.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    }

    title = std::string_view(reinterpret_cast<const char*>(&buffer[0x0134]), 0x0143 - 0x0134);
    licensee = getOldLicensee(buffer[0x014B]);

    uint8_t romCode = buffer[0x0148];
    romSize = 32768 * (1 << romCode);

    uint8_t ramCode = buffer[0x0149];
    versionNumber = buffer[0x014C];
    
    headerChecksum = buffer[0x014D];
    globalChecksum = buffer[0x014E] << 8 | buffer[0x014F];
}

uint8_t Cartridge::read(uint16_t addr) {
    return buffer[addr];
} 

