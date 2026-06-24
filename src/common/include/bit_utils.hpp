#ifndef BIT_UTILS_HPP
#define BIT_UTILS_HPP

#include <concepts>
#include <cstddef>
#include <climits>
#include <cassert>
#include <cstdint>

namespace BitUtils {

template<std::integral T>
T set(T value, unsigned char bit) noexcept {
    assert(sizeof(T) * CHAR_BIT > bit);
    return value | (1u << bit);
}

template<std::integral T>
T reset(T value, unsigned char bit) noexcept {
    assert(sizeof(T) * CHAR_BIT > bit);
    return value & ~(1u << bit);
}

inline constexpr uint8_t lowNibbleMask = 0x0F;

inline uint8_t lowNibble(uint8_t value) noexcept { 
    return value & lowNibbleMask;
}

inline constexpr uint8_t highNibbleMask = 0xF0;
inline constexpr uint8_t highNibbleShift = 4;

inline uint8_t highNibble(uint8_t value) noexcept {
    return (value & highNibbleMask) >> highNibbleShift;
}

inline constexpr uint16_t lowByteMask = 0x00FF;

inline uint8_t lowByte(uint16_t value) noexcept {
    return value & lowByteMask;
}

inline constexpr uint16_t highByteMask = 0xFF00;
inline constexpr uint8_t highByteShift = 8;

inline uint8_t highByte(uint16_t value) noexcept {
    return (value & highByteMask) >> highByteShift;
}

inline uint16_t concat(uint8_t high, uint8_t low) noexcept {
    return (static_cast<uint16_t>(high) << highByteShift) | low;
}

}

#endif