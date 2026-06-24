#ifndef CPU_HPP
#define CPU_HPP

#include "bit_utils.hpp"

#include <cstdint>

namespace MintGB {

/**
 * @brief An SM83 CPU emulator core.
 * 
 * CPU executes gameboy instructions. 
 * 
 * @headerfile cpu.hpp
 */
class CPU {
    public:
    explicit CPU() = default;

    /**
     * @brief A class meant to be defined by CPU Testing programs. 
     * 
     * CPU::Tests is a friend of CPU, giving full access to its members and member functions.
     * CPU::Tests does not have a default implementation. The class itself must be defined. 
     * @headerfile cpu.hpp
     */
    class Tests;
    friend class Tests;

    struct State;

    enum class Flags : uint8_t {
        Zero = 7,
        Subtract = 6,
        HalfCarry = 5,
        Carry = 4
    };

    /**
     * @brief Retrieve the state of the CPU
     * @headerfile cpu.hpp
     */
    State state() const;

    /**
     * @brief Set the state of the CPU
     * @headerfile cpu.hpp
     */
    void state(State cpuState);

    private:
    uint8_t a = 0x00, f = 0x00;
    uint8_t b = 0x00, c = 0x00;
    uint8_t d = 0x00, e = 0x00;
    uint8_t h = 0x00, l = 0x00;

    uint16_t pc = 0x0000;
    uint16_t sp = 0x0000;

    bool ime;
    bool ei;
    bool halted;

    uint16_t af() const { return BitUtils::concat(a, f); }
    void af(uint16_t value) { a = BitUtils::highByte(value); f = BitUtils::lowByte(value & BitUtils::highNibbleMask); }

    uint16_t bc() const { return BitUtils::concat(b, c); }
    void bc(uint16_t value) { b = BitUtils::highByte(value); c = BitUtils::lowByte(value); }

    uint16_t de() const { return BitUtils::concat(d, e); }
    void de(uint16_t value) { d = BitUtils::highByte(value); e = BitUtils::lowByte(value); }

    uint16_t hl() const { return BitUtils::concat(h, l); }
    void hl(uint16_t value) { h = BitUtils::highByte(value); l = BitUtils::lowByte(value); }

    bool zf() const { return BitUtils::test(f, static_cast<unsigned char>(Flags::Zero)); }
    void zf(bool cond) { f = BitUtils::set(f, static_cast<unsigned char>(Flags::Zero), cond); }

    bool nf() const { return BitUtils::test(f, static_cast<unsigned char>(Flags::Subtract)); }
    void nf(bool cond) { f = BitUtils::set(f, static_cast<unsigned char>(Flags::Subtract), cond); }

    bool hf() const { return BitUtils::test(f, static_cast<unsigned char>(Flags::HalfCarry)); }
    void hf(bool cond) { f = BitUtils::set(f, static_cast<unsigned char>(Flags::HalfCarry), cond); }

    bool cf() const { return BitUtils::test(f, static_cast<unsigned char>(Flags::Carry)); }
    void cf(bool cond) { f = BitUtils::set(f, static_cast<unsigned char>(Flags::Carry), cond); }
};

/**
 * @brief A struct meant to represent the state of the CPU
 * 
 * CPU::State contains a copy of the state of the CPU registers and flags.
 * @headerfile cpu.hpp
 */
struct CPU::State {
    uint16_t af = 0x0000;
    uint16_t bc = 0x0000;
    uint16_t de = 0x0000;
    uint16_t hl = 0x0000;
    
    uint16_t pc = 0x0000;
    uint16_t sp = 0x0000;

    bool ime = false;
    bool ei = false;
    bool halted = false;

    bool operator==(const CPU::State&) const = default;
};   

}

#endif