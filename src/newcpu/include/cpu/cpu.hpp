#ifndef CPU_HPP
#define CPU_HPP

#include "bit_utils.hpp"

#include <cstdint>

namespace MintGB {

/**
 * @brief A class meant to be defined by CPU Testing programs. 
 * 
 * CPUTest is a friend of CPU, giving full access to its members and member functions.
 * CPUTest does not have a default implementation. The class itself must be defined. 
 * @headerfile cpu.hpp
 */
class CPUTests;

class CPU {
    public:
    friend class CPUTests;

    private:
    uint8_t a, f = 0x00;
    uint8_t b, c = 0x00;
    uint8_t d, e = 0x00;
    uint8_t h, l = 0x00;

    uint16_t pc = 0x0000;
    uint16_t sp = 0x0000;

    uint16_t af() { return BitUtils::concat(a, f); }
    void af(uint16_t value) { a = BitUtils::highByte(value); f = BitUtils::lowByte(value & BitUtils::highNibbleMask); }

    uint16_t bc() { return BitUtils::concat(b, c); }
    void bc(uint16_t value) { b = BitUtils::highByte(value); c = BitUtils::lowByte(value); }

    uint16_t de() { return BitUtils::concat(d, e); }
    void de(uint16_t value) { d = BitUtils::highByte(value); e = BitUtils::lowByte(value); }

    uint16_t hl() { return BitUtils::concat(h, l); }
    void hl(uint16_t value) { h = BitUtils::highByte(value); l = BitUtils::lowByte(value); }
};

}

#endif