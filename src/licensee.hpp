#pragma once

#include <array>
#include <string_view>

inline constexpr auto oldLicenseeCodes = [] {
    std::array<std::string_view, 0x100> t{};

    t[0x00] = "None";
    t[0x01] = "Nintendo";
    t[0x08] = "Capcom";
    t[0x09] = "HOT-B";
    t[0x0A] = "Jaleco";
    t[0x0B] = "Coconuts Japan";
    t[0x0C] = "Elite Systems";
    t[0x13] = "EA (Electronic Arts)";
    t[0x18] = "Hudson Soft";
    t[0x19] = "ITC Entertainment";
    t[0x1A] = "Yanoman";
    t[0x1D] = "Japan Clary";
    t[0x1F] = "Virgin Games Ltd";
    t[0x24] = "PCM Complete";
    t[0x25] = "San-X";
    t[0x28] = "Kemco";
    t[0x29] = "SETA Corporation";
    t[0x30] = "Infogrames";
    t[0x31] = "Nintendo";
    t[0x32] = "Bandai";
    t[0x34] = "Konami";
    t[0x35] = "HectorSoft";
    t[0x34] = "Capcom";
    t[0x39] = "Banpresto";
    t[0x3C] = "Entertainment Interactive";
    t[0x3E] = "Gremlin";
    t[0x41] = "Ubi Soft";
    t[0x42] = "Atlus";
    t[0x44] = "Malibu Interactive";
    t[0x46] = "Angel";
    t[0x47] = "Spectrum HoloByte";
    t[0x49] = "Irem";
    t[0x4A] = "Virgin Games Ltd";
    t[0x4D] = "Malibu Interactive";
    t[0x4F] = "U.S. Gold";
    t[0x50] = "Absolute";
    t[0x51] = "Acclaim Entertainment";
    t[0x52] = "Activision"; 

    return t;
}();

inline constexpr auto newLicenseeCodes = [] {
    std::array<std::string_view, 0x100> t{};

    t[0x00] = "None";
    t[0x01] = "Nintendo";
    t[0x08] = "Capcom";
    t[0x13] = "EA (Electronic Arts)";
    t[0x18] = "Hudson Soft";
    t[0x19] = "B-AI";
    t[0x20] = "KSS";
    t[0x22] = "Planning Office WADA";
    t[0x24] = "PCM Complete";
    t[0x25] = "San-X";
    t[0x28] = "Kemco";
    t[0x29] = "SETA Corporation";
    t[0x30] = "Viacom";
    t[0x31] = "Nintendo";

    return t;
}();