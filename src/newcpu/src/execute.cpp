#include "cpu.hpp"

#include "instruction.hpp"

namespace MintGB {

#define OP(name, operation, ...) case Instruction::name: operation(__VA_ARGS__); break

void CPU::execute(Instruction ins) {
    switch (ins) {
        // ld r8, r8
        OP(LD_A_A, ld_r8_r8, a, a);
        OP(LD_A_B, ld_r8_r8, a, b);
        OP(LD_A_C, ld_r8_r8, a, c);
        OP(LD_A_D, ld_r8_r8, a, d);
        OP(LD_A_E, ld_r8_r8, a, e);
        OP(LD_A_H, ld_r8_r8, a, h);
        OP(LD_A_L, ld_r8_r8, a, l);
        
        OP(LD_B_A, ld_r8_r8, b, a);
        OP(LD_B_B, ld_r8_r8, b, b);
        OP(LD_B_C, ld_r8_r8, b, c);
        OP(LD_B_D, ld_r8_r8, b, d);
        OP(LD_B_E, ld_r8_r8, b, e);
        OP(LD_B_H, ld_r8_r8, b, h);
        OP(LD_B_L, ld_r8_r8, b, l);

        OP(LD_C_A, ld_r8_r8, c, a);
        OP(LD_C_B, ld_r8_r8, c, b);
        OP(LD_C_C, ld_r8_r8, c, c);
        OP(LD_C_D, ld_r8_r8, c, d);
        OP(LD_C_E, ld_r8_r8, c, e);
        OP(LD_C_H, ld_r8_r8, c, h);
        OP(LD_C_L, ld_r8_r8, c, l);

        OP(LD_D_A, ld_r8_r8, d, a);
        OP(LD_D_B, ld_r8_r8, d, b);
        OP(LD_D_C, ld_r8_r8, d, c);
        OP(LD_D_D, ld_r8_r8, d, d);
        OP(LD_D_E, ld_r8_r8, d, e);
        OP(LD_D_H, ld_r8_r8, d, h);
        OP(LD_D_L, ld_r8_r8, d, l);
        
        OP(LD_E_A, ld_r8_r8, e, a);
        OP(LD_E_B, ld_r8_r8, e, b);
        OP(LD_E_C, ld_r8_r8, e, c);
        OP(LD_E_D, ld_r8_r8, e, d);
        OP(LD_E_E, ld_r8_r8, e, e);
        OP(LD_E_H, ld_r8_r8, e, h);
        OP(LD_E_L, ld_r8_r8, e, l);

        OP(LD_H_A, ld_r8_r8, h, a);
        OP(LD_H_B, ld_r8_r8, h, b);
        OP(LD_H_C, ld_r8_r8, h, c);
        OP(LD_H_D, ld_r8_r8, h, d);
        OP(LD_H_E, ld_r8_r8, h, e);
        OP(LD_H_H, ld_r8_r8, h, h);
        OP(LD_H_L, ld_r8_r8, h, l);

        OP(LD_L_A, ld_r8_r8, l, a);
        OP(LD_L_B, ld_r8_r8, l, b);
        OP(LD_L_C, ld_r8_r8, l, c);
        OP(LD_L_D, ld_r8_r8, l, d);
        OP(LD_L_E, ld_r8_r8, l, e);
        OP(LD_L_H, ld_r8_r8, l, h);
        OP(LD_L_L, ld_r8_r8, l, l);

        // ld r8, n8
        OP(LD_A_Imm8, ld_r8_n8, a);
        OP(LD_B_Imm8, ld_r8_n8, b);
        OP(LD_C_Imm8, ld_r8_n8, c);
        OP(LD_D_Imm8, ld_r8_n8, d);
        OP(LD_E_Imm8, ld_r8_n8, e);
        OP(LD_H_Imm8, ld_r8_n8, h);
        OP(LD_L_Imm8, ld_r8_n8, l);

        // ld r16, n16
        OP(LD_BC_Imm16, ld_r16_n16, b, c);
        OP(LD_DE_Imm16, ld_r16_n16, d, e);
        OP(LD_HL_Imm16, ld_r16_n16, h, l);

        // ld [hl], r8
        OP(LD_IndirectHL_A, ld_ind_hl_r8, a);
        OP(LD_IndirectHL_B, ld_ind_hl_r8, b);
        OP(LD_IndirectHL_C, ld_ind_hl_r8, c);
        OP(LD_IndirectHL_D, ld_ind_hl_r8, d);
        OP(LD_IndirectHL_E, ld_ind_hl_r8, e);
        OP(LD_IndirectHL_H, ld_ind_hl_r8, h);
        OP(LD_IndirectHL_L, ld_ind_hl_r8, l);

        // ld [hl], n8
        OP(LD_IndirectHL_Imm8, ld_ind_hl_n8);

        // ld r8, [hl]
        OP(LD_A_IndirectHL, ld_r8_ind_hl, a);
        OP(LD_B_IndirectHL, ld_r8_ind_hl, b);
        OP(LD_C_IndirectHL, ld_r8_ind_hl, c);
        OP(LD_D_IndirectHL, ld_r8_ind_hl, d);
        OP(LD_E_IndirectHL, ld_r8_ind_hl, e);
        OP(LD_H_IndirectHL, ld_r8_ind_hl, h);
        OP(LD_L_IndirectHL, ld_r8_ind_hl, l);

        // ld [r16], a
        OP(LD_IndirectBC_A, ld_ind_r16_a, bc());
        OP(LD_IndirectDE_A, ld_ind_r16_a, de());

        // ld [a16], a
        OP(LD_IndirectAddr16_A, ld_ind_a16_a);

        // ldh [a16], a
        OP(LDH_IndirectAddr8_A, ldh_ind_a16_a);

        // ldh [c], a
        OP(LD_IndirectC_A, ldh_ind_c_a);

        // ld a, [r16]
        OP(LD_A_IndirectBC, ld_a_ind_r16, bc());
        OP(LD_A_IndirectDE, ld_a_ind_r16, de());

        // ld a, [a16]
        OP(LD_A_IndirectAddr16, ld_a_ind_a16);

        // ldh a, [a16]
        OP(LDH_A_IndirectAddr8, ldh_a_ind_a16);

        // ldh a, [c]
        OP(LD_A_IndirectC, ldh_a_ind_c);

        // ld [hli], a
        OP(LD_IndirectHLI_A, ld_ind_hli_a);

        // ld [hld], a
        OP(LD_IndirectHLD_A, ld_ind_hld_a);

        // ld a, [hli]
        OP(LD_A_IndirectHLI, ld_a_ind_hli);

        // ld a, [hld]
        OP(LD_A_IndirectHLD, ld_a_ind_hld);
    }
}

void CPU::ld_r8_r8(uint8_t& dst, uint8_t src) {
    dst = src;
}

void CPU::ld_r8_n8(uint8_t& dst) {
    dst = fetchByte();
}

void CPU::ld_r16_n16(uint8_t& high, uint8_t& low) {
    low = fetchByte();
    high = fetchByte();
}

void CPU::ld_ind_hl_r8(uint8_t src) {
    writeByte(hl(), src);
}

void CPU::ld_ind_hl_n8() {
    uint8_t value = fetchByte();
    writeByte(hl(), value);
}

void CPU::ld_r8_ind_hl(uint8_t& dst) {
    dst = readByte(hl());
}

void CPU::ld_ind_r16_a(uint16_t addr) {
    writeByte(addr, a);
}

void CPU::ld_ind_a16_a() {
    uint16_t addr = fetchWord();
    writeByte(addr, a);
}

static constexpr uint16_t highPage = 0xFF00;

void CPU::ldh_ind_a16_a() {
    uint8_t addr = fetchByte();
    writeByte(highPage + addr, a);
}

void CPU::ldh_ind_c_a() {
    writeByte(highPage + c, a);
}

void CPU::ld_a_ind_r16(uint16_t addr) {
    a = readByte(addr);
}

void CPU::ld_a_ind_a16() {
    uint16_t addr = fetchWord();
    a = readByte(addr);
}

void CPU::ldh_a_ind_a16() {
    uint8_t addr = fetchByte();
    a = readByte(highPage + addr);
}

void CPU::ldh_a_ind_c() {
    a = readByte(highPage + c);
}

void CPU::ld_ind_hli_a() {
    uint16_t addr = hl();
    writeByte(addr, a);
    hl(addr + 1);
}

void CPU::ld_ind_hld_a() {
    uint16_t addr = hl();
    writeByte(addr, a);
    hl(addr - 1);
}

void CPU::ld_a_ind_hli() {
    uint16_t addr = hl();
    a = readByte(addr);
    hl(addr + 1);
}

void CPU::ld_a_ind_hld() {
    uint16_t addr = hl();
    a = readByte(addr);
    hl(addr - 1);
}

}
