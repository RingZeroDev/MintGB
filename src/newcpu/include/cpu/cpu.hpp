#ifndef CPU_HPP
#define CPU_HPP

#include "bit_utils.hpp"
#include "instruction.hpp"
#include "cb_instruction.hpp"

#ifdef TEST_CPU
#include "mmu/test_mmu.hpp"
#else
#include "mmu/mmu.hpp"
#endif

#include <cstdint>

namespace MintGB {

/**
 * @brief An SM83 CPU emulator core.
 * 
 * CPU executes gameboy instructions. 
 */
class CPU {
    public:
    explicit CPU(MMU* mmu) : mmu(mmu) {}

    /**
     * @brief A class meant to be defined by CPU Testing programs. 
     * 
     * CPU::Tests is a friend of CPU, giving full access to its members and member functions.
     * CPU::Tests does not have a default implementation. The class itself must be defined. 
     */
    class Tests;
    friend class Tests;

    struct State;

    /**
     * @brief Retrieves the state of the CPU
     */
    State state() const;

    /**
     * @brief Set the state of the CPU
     */
    void state(State cpuState);

    private:
    enum class Flags : uint8_t {
        Zero = 7,
        Subtract = 6,
        HalfCarry = 5,
        Carry = 4
    };

    MMU* mmu = nullptr;

    uint8_t a = 0x00, f = 0x00;
    uint8_t b = 0x00, c = 0x00;
    uint8_t d = 0x00, e = 0x00;
    uint8_t h = 0x00, l = 0x00;

    uint16_t pc = 0x0000;
    uint16_t sp = 0x0000;

    bool ime = false;
    bool ei = false;
    bool halted = false;

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

    void execute(Instruction ins);
    void executeCB(CBInstruction ins);

    // Load, Store, Move

    uint8_t readByte(uint16_t addr) const;
    void writeByte(uint16_t addr, uint8_t value) const;
    void waitCycle() const;
    void waitCycle(uint8_t amount) const;
    uint16_t readWord(uint16_t addr) const;
    void writeWord(uint16_t addr, uint16_t value) const;
    uint8_t fetchByte();
    int8_t fetchRelative();
    uint16_t fetchWord();

    uint16_t pop();
    void push(uint16_t addr);

    // Arithmetic and Logic

    bool arithHalfCarry(uint8_t value, uint16_t res) const;
    bool arithHalfCarry(uint16_t value, uint32_t res) const;
    bool unaryHalfCarry(uint8_t reg, uint8_t compare) const;
    uint16_t relCarry(int8_t value, uint16_t res) const;

    void arithFlags(uint8_t value, uint16_t res, bool subtract);
    void arithFlags(uint16_t value, uint32_t res);
    void unaryFlags(uint8_t reg, uint8_t res, uint8_t compare, bool subtract);
    void logicFlags(uint8_t res, bool halfCarry);
    void relFlags(int8_t value, uint16_t res);

    void adc(uint8_t value);
    void add(uint8_t value);
    void cp(uint8_t value);
    void dec(uint8_t& reg);
    void inc(uint8_t& reg);
    void sbc(uint8_t value);
    void sub(uint8_t value);
    void and_(uint8_t value);
    void or_(uint8_t value);
    void xor_(uint8_t value);
    void daa();
    void add(uint16_t value);
    uint16_t spRel();

    // Rotate, Shift, Bit

    void shiftFlags(uint8_t sb, uint8_t value);
    void accShiftFlags(uint8_t sb);

    void rl(uint8_t& reg);
    void rla();
    void rlc(uint8_t& reg);
    void rlca();
    void rr(uint8_t& reg);
    void rra();
    void rrc(uint8_t& reg);
    void rrca();
    void sla(uint8_t& reg);
    void sra(uint8_t& reg);
    void srl(uint8_t& reg);
    void swap(uint8_t& reg);
    void bit(uint8_t u3, uint8_t value);
    void res(uint8_t u3, uint8_t& reg);
    void set(uint8_t u3, uint8_t& reg);

    // Control

    void call(uint16_t addr);
    void ret();
    void halt();
    void stop();
};

/**
 * @brief A struct meant to represent the state of the CPU
 * 
 * CPU::State contains a copy of the state of the CPU registers and flags.
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