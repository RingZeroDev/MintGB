#ifndef BIT_UTILS_HPP
#define BIT_UTILS_HPP

#include <concepts>
#include <cstddef>
#include <climits>
#include <cassert>
#include <cstdint>

namespace BitUtils { 

inline constexpr uint8_t maxNibble = 0x0F;
inline constexpr uint8_t maxByte = 0xFF;
inline constexpr uint16_t maxWord = 0xFFFF;

template<std::integral T>
constexpr T set(T value, unsigned char bit) noexcept {
    assert(sizeof(T) * CHAR_BIT > bit);
    return value | (1u << bit);
}

template<std::integral T>
constexpr T set(T value, unsigned char bit, bool cond) noexcept {
    assert(sizeof(T) * CHAR_BIT > bit);
    return value & ~(1u << bit) | (static_cast<uint8_t>(cond) << bit);
}

template<std::integral T>
constexpr T reset(T value, unsigned char bit) noexcept {
    assert(sizeof(T) * CHAR_BIT > bit);
    return value & ~(1u << bit);
}

template<std::integral T>
constexpr bool test(T value, unsigned char bit) noexcept {
    assert(sizeof(T) * CHAR_BIT > bit);
    return static_cast<bool>(value & (1u << bit)); 
}

inline constexpr uint8_t lowNibbleMask = 0x0F;

constexpr inline uint8_t lowNibble(uint8_t value) noexcept { 
    return value & lowNibbleMask;
}

inline constexpr uint8_t highNibbleMask = 0xF0;
inline constexpr uint8_t highNibbleShift = 4;

constexpr inline uint8_t highNibble(uint8_t value) noexcept {
    return (value & highNibbleMask) >> highNibbleShift;
}

inline constexpr uint16_t lowByteMask = 0x00FF;

constexpr inline uint8_t lowByte(uint16_t value) noexcept {
    return value & lowByteMask;
}

inline constexpr uint16_t highByteMask = 0xFF00;
inline constexpr uint8_t highByteShift = 8;

constexpr inline uint8_t highByte(uint16_t value) noexcept {
    return (value & highByteMask) >> highByteShift;
}

constexpr inline uint16_t concat(uint8_t high, uint8_t low) noexcept {
    return (static_cast<uint16_t>(high) << highByteShift) | low;
}

constexpr inline uint8_t concatByte(uint8_t high, uint8_t low) noexcept {
    return (high << highNibbleShift) | low;
}

template<std::integral T>
constexpr bool msb(T value) noexcept {
    constexpr unsigned int bits = sizeof(T) * CHAR_BIT;
    return value >> (bits - 1);
}

template<std::integral T>
constexpr bool lsb(T value) noexcept {
    return value & 1;
}

}

#endif