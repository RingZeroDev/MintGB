#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <format>

#include "licensee.hpp"

enum class CGBSupport : uint8_t {
    
};

enum class CartridgeType : uint8_t {
    RomOnly = 0x00,
    MCB_1 
};

class Cartridge {
    std::string_view title;
    std::string_view manufacturerCode;
    std::string_view licensee;
    size_t romSize;
    size_t ramSize;
    uint8_t versionNumber;
    uint8_t headerChecksum;
    uint16_t globalChecksum;

    public:
        Cartridge(const char* path) {
            size_t fileSize = std::filesystem::file_size(path);
            
            std::vector<uint8_t> buffer(fileSize);

            std::ifstream f(path, std::ios_base::binary);
            if (f) {
                f.read(reinterpret_cast<char*>(buffer.data()), fileSize);
            }

            title = std::string_view(reinterpret_cast<const char*>(&buffer[0x0134]), 0x0143 - 0x0134);
            licensee = oldLicenseeCodes[buffer[0x014B]];

            uint8_t romCode = buffer[0x0148];
            romSize = 32768 * (1 << romCode);

            uint8_t ramCode = buffer[0x0149];
            versionNumber = buffer[0x014C];
            
            headerChecksum = buffer[0x014D];
            globalChecksum = buffer[0x014E] << 8 | buffer[0x014F];

            std::cout << title << std::endl;
            std::cout << licensee << std::endl;
            std::cout << romSize << std::endl;
            std::cout << std::format("Version: {}", versionNumber) << std::endl;
            std::cout << std::format("Header Checksum: {:02X}", headerChecksum) << std::endl;
            std::cout << std::format("Global Checksum: {:04X}", globalChecksum) << std::endl;
        }
};