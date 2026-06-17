#include "cpu.hpp"

void CPU::ld_r8_r8(uint8_t& r1, uint8_t& r2) {
    r1 = r2;
}

void CPU::ld_r8_n8(uint8_t& r) {
    r = fetchByte();
}

void CPU::ld_ind_hl_r8(uint8_t& r) {
    writeByte(hl, r);
}

void CPU::ld_ind_hl_n8() {
    uint8_t value = fetchByte();
    writeByte(hl, value);
}

void CPU::ld_r8_ind_hl(uint8_t& r) {
    r = readByte(hl);
}

void CPU::ld_ind_r16_a(RegisterPair& rp) {
    writeByte(rp, a);
}

void CPU::ld_a16_a() {
    uint16_t addr = fetchWord();
    writeByte(addr, a);
}

void CPU::ldh_a16_a() {
    uint8_t addr = fetchByte();
    writeByte(0xFF00 + addr, a);
}

void CPU::ldh_ind_c_a() {
    writeByte(0xFF00 + c, a);
}

void CPU::ld_a_ind_r16(RegisterPair& rp) {
    a = readByte(rp);
}

void CPU::ld_a_a16() {
    uint16_t addr = fetchWord();
    a = readByte(addr);
}

void CPU::ldh_a_a16() {
    uint8_t addr = fetchByte();
    a = readByte(0xFF00 + addr);
}

void CPU::ldh_a_ind_c() {
    a = readByte(0xFF00 + c);
}

void CPU::ld_ind_hli_a() {
    writeByte(hl++, a);
}

void CPU::ld_ind_hld_a() {
    writeByte(hl--, a);
}

void CPU::ld_a_ind_hli() {
    a = readByte(hl++);
}

void CPU::ld_a_ind_hld() {
    a = readByte(hl--);
}

