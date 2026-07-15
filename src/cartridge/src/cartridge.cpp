#include "cartridge.hpp"

#include <format>
#include <type_traits>

std::string_view Cartridge::getCartridgeType(uint8_t code) {
    switch (code) {
        case 0x00: return "ROM ONLY";
        case 0x01: return "MBC1";
        case 0x02: return "MBC1+RAM";
        case 0x03: return "MBC1+RAM+BATTERY";
        case 0x05: return "MBC2";
        case 0x06: return "MBC2+BATTERY";
        case 0x08: return "ROM+RAM";
        case 0x09: return "ROM+RAM+BATTERY";
        case 0x0B: return "MMM01";
        case 0x0C: return "MMM01+RAM";
        case 0x0F: return "MMM01+RAM+BATTERY";
        case 0x10: return "MBC3+TIMER+BATTERY";
        case 0x11: return "MBC3";
        case 0x12: return "MBC3+RAM";
        case 0x13: return "MBC3+RAM+BATTERY";
        case 0x19: return "MBC5";
        case 0x1A: return "MBC5+RAM";
        case 0x1B: return "MBC5+RAM+BATTERY";
        case 0x1C: return "MBC5+RUMBLE";
        case 0x1D: return "MBC5+RUMBLE+RAM";
        case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
        case 0x20: return "MBC6";
        case 0x22: return "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
        case 0xFC: return "POCKET CAMERA";
        case 0xFD: return "BANDAI TAMA5";
        case 0xFE: return "HuC3";
        case 0xFF: return "HuC1+RAM+BATTERY";

        default: return "Unrecognized";
    }
}

Cartridge::Cartridge(const char* path) {
    size_t fileSize = std::filesystem::file_size(path);
    
    buffer = std::vector<uint8_t>(fileSize);

    std::ifstream f(path, std::ios_base::binary);
    if (f) {
        f.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    }

    title = std::string_view(reinterpret_cast<const char*>(&buffer[0x0134]), 0x0143 - 0x0134);
    licensee = getOldLicensee(buffer[0x014B]);

    type = getCartridgeType(buffer[0x0147]);

    uint8_t romCode = buffer[0x0148];
    romSize = 32768 * (1 << romCode);

    uint8_t ramCode = buffer[0x0149];
    versionNumber = buffer[0x014C];
    
    headerChecksum = buffer[0x014D];
    globalChecksum = buffer[0x014E] << 8 | buffer[0x014F];

    mbc.emplace<MBC1>(std::move(buffer), std::vector<uint8_t>(ramSize));
}

uint8_t Cartridge::read(uint16_t addr) {
    return std::visit([addr](auto&& mapper) {
        return mapper.read(addr);
    }, mbc);
} 

void Cartridge::write(uint16_t addr, uint8_t value) {
    return std::visit([addr, value](auto&& mapper) {
        using T = std::decay_t<decltype(mapper)>;
        
        if constexpr (!std::is_same_v<T, ROMOnly>) {
            mapper.write(addr, value);
        }
    }, mbc);
}

CartridgeMetadata Cartridge::getMetadata() const {
    return CartridgeMetadata {
        .title = title,
        .manufacturerCode = manufacturerCode,
        .licensee = licensee,
        .type = type,
        .romSize = romSize,
        .ramSize = ramSize,
        .versionNumber = versionNumber,
        .headerChecksum = headerChecksum,
        .globalChecksum = globalChecksum
    };
}

uint8_t Cartridge::calculateHeaderChecksum() const {
    uint8_t checksum = 0x00;

    for (uint16_t addr = 0x0134; addr <= 0x014C; addr++) {
        checksum = checksum - buffer[addr] - 1;
    }

    return checksum;
}

uint16_t Cartridge::calculateGlobalChecksum() const {
    uint16_t checksum = 0x0000;

    for (size_t addr = 0x0000; addr < buffer.size(); addr++) {
        // Skip the two global checksum bytes
        if (addr == 0x014E || addr == 0x014F) continue;

        checksum += buffer[addr];
    }

    return checksum;
}

