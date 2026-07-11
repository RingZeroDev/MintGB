#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <format>

struct CartridgeMetadata {
    std::string_view title;
    std::string_view manufacturerCode;
    std::string_view licensee;
    std::string_view type;
    size_t romSize;
    size_t ramSize;
    uint8_t versionNumber;
    uint8_t headerChecksum;
    uint16_t globalChecksum;
};

class Cartridge {
    private:
        std::vector<uint8_t> buffer;

        std::string_view title;
        std::string_view manufacturerCode;
        std::string_view licensee;
        std::string_view type;
        size_t romSize;
        size_t ramSize;
        uint8_t versionNumber;
        uint8_t headerChecksum;
        uint16_t globalChecksum;

        static std::string_view getCartridgeType(uint8_t code);
        static std::string_view getOldLicensee(uint8_t code);
        static std::string_view getNewLicensee(uint8_t code);

    public:
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
        Cartridge(const char *path);

        CartridgeMetadata getMetadata() const;
        
        uint8_t calculateHeaderChecksum() const;
        uint16_t calculateGlobalChecksum() const;
};