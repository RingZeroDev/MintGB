#pragma once

#include "register.hpp"
#include "flags.hpp"

#include <cstdint>

template<Register8 Reg8>
uint8_t get() {
    if constexpr (Reg8 == Register8::IndirectHL) {
        return readByte(hl);
    } else {
        return this->*REG_TABLE[static_cast<uint8_t>(Reg8)];
    }
}

template<Register8 Reg8>
void set(uint8_t value) {
    if constexpr (Reg8 == Register8::IndirectHL) {
        writeByte(hl, value);
    } else {
        this->*REG_TABLE[static_cast<uint8_t>(Reg8)] = value;
    }
}

template<Register16 Reg16>
uint16_t get() {
    if constexpr (Reg16 == Register16::SP) {
        return sp;
    } else {
        return this->*REGPAIR_TABLE[static_cast<uint8_t>(Reg16)];
    }
}

template<Register16 Reg16>
void set(uint16_t value) {
    if constexpr (Reg16 == Register16::SP) {
        sp = value;
    } else {
        this->*REGPAIR_TABLE[static_cast<uint8_t>(Reg16)] = value;
    }
}

template<Register16Stack Reg16>
uint16_t get() {
    return this->*REGPAIR_TABLE[static_cast<uint8_t>(Reg16)];
}

template<Register16Stack Reg16>
void set(uint16_t value) {
    this->*REGPAIR_TABLE[static_cast<uint8_t>(Reg16)] = value;
}

template<Register16Mem Reg16>
uint16_t get() {
    if constexpr (Reg16 == Register16Mem::BC) return bc;
    if constexpr (Reg16 == Register16Mem::DE) return de;
    if constexpr (Reg16 == Register16Mem::HLD) return hl--;
    if constexpr (Reg16 == Register16Mem::HLI) return hl++;
}

template<Extended Ext>
auto get() {
    if constexpr (Ext == Extended::Immediate8) {
        return readByte(pc++);
    } else if constexpr (Ext == Extended::Immediate16) {
        uint16_t value = readWord(pc);
        pc += 2;
        return value;
    } else if constexpr (Ext == Extended::Address8) {
        return readByte();
    }
}

template<Condition Cond>
bool test() {
    if constexpr (Cond == Condition::Carry) return flags.get<FlagBits::Carry>();
    if constexpr (Cond == Condition::NoCarry) return !flags.get<FlagBits::Carry>();
    if constexpr (Cond == Condition::Zero) return flags.get<FlagBits::Zero>();
    if constexpr (Cond == Condition::NotZero) return !flags.get<FlagBits::Zero>();
}