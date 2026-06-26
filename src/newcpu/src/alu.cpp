#include "cpu.hpp"

namespace MintGB {

inline bool CPU::arithHalfCarry(uint8_t value, uint16_t res) const {
    constexpr uint8_t halfCarryCompare = 0x10;
    return (a ^ value ^ res) & halfCarryCompare;
}

inline void CPU::arithFlags(uint8_t value, uint16_t res, bool subtract) {
    zf(BitUtils::lowByte(res) == 0);
    nf(subtract);
    hf(arithHalfCarry(value, res));
    cf(res > BitUtils::maxByte);
}

void CPU::adc(uint8_t value) {
    uint16_t res = a + value + cf();
    arithFlags(value, res, false);
    a = res;
}

void CPU::add(uint8_t value) {
    uint16_t res = a + value;
    arithFlags(value, res, false);
    a = res;
}

inline bool CPU::arithHalfCarry(uint16_t value, uint32_t res) const {
    constexpr uint16_t halfCarryCompare = 0x1000;
    return (hl() ^ value ^ res) & halfCarryCompare;
}

inline void CPU::arithFlags(uint16_t value, uint32_t res) {
    nf(false);
    hf(arithHalfCarry(value, res));
    cf(res > BitUtils::maxWord);
}

void CPU::add(uint16_t value) {
    uint32_t res = hl() + value;
    arithFlags(value, res);
    hl(res);
}

void CPU::sbc(uint8_t value) {
    uint16_t res = a - value - cf();
    arithFlags(value, res, true);
    a = res;
}

void CPU::sub(uint8_t value) {
    uint16_t res = a - value;
    arithFlags(value, res, true);
    a = res;
}

void CPU::cp(uint8_t value) {
    uint16_t res = a - value;
    arithFlags(value, res, true);
}

inline bool CPU::unaryHalfCarry(uint8_t reg, uint8_t compare) const {
    return (reg & BitUtils::maxNibble) == compare;
}

inline void CPU::unaryFlags(uint8_t reg, uint8_t res, uint8_t compare, bool subtract) {
    zf(res == 0);
    nf(subtract);
    hf(unaryHalfCarry(reg, compare));
}

void CPU::dec(uint8_t& reg) {
    uint8_t res = reg - 1;
    unaryFlags(reg, res, 0x00, true);
    reg = res;
}

void CPU::inc(uint8_t& reg) {
    uint8_t res = reg + 1;
    unaryFlags(reg, res, BitUtils::maxNibble, false);
    reg = res;
}

void CPU::inc(uint16_t& value) {
    value++;
}

void CPU::dec(uint16_t& value) {
    value--;
}

inline void CPU::logicFlags(uint8_t res, bool halfCarry) {
    zf(res == 0);
    nf(false);
    hf(halfCarry);
    cf(false);
}

void CPU::and_(uint8_t value) {
    uint8_t res = a & value;
    logicFlags(res, true);
    a = res;
}

void CPU::or_(uint8_t value) {
    uint8_t res = a | value;
    logicFlags(res, false);
    a = res;
}

void CPU::xor_(uint8_t value) {
    uint8_t res = a ^ value;
    logicFlags(res, false);
    a = res;
}

void CPU::daa() {
    constexpr uint8_t hfAdjust = 0x06;
    constexpr uint8_t cfAdjust = 0x60;

    constexpr uint8_t hfCompare = 0x09;
    constexpr uint8_t cfCompare = 0x99;

    uint8_t res = a;
    uint8_t adj = 0;

    // subtract
    if (nf()) {
        if (hf()) adj += hfAdjust;
        if (cf()) adj += cfAdjust;
        res -= adj;
    } else {
        // add
        if (hf() || (a & BitUtils::lowNibbleMask) > hfCompare) 
            adj += hfAdjust;
        if (cf() || a > cfCompare) {
            adj += cfAdjust;
            cf(true);
        } 
        res += adj;
    }

    zf(res == 0);
    hf(false);
    a = res;
}

void CPU::cpl() {
    a = ~a; 
    nf(true);
    hf(true);
}

void CPU::ccf() {
    cf(!cf());
    hf(false);
    nf(false);
}

void CPU::scf() {
    cf(true);
    hf(false);
    nf(false);
}

}