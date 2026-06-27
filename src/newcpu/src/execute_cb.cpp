#include "cpu.hpp"
#include "cb_instruction.hpp"

#include <stdexcept>
#include <format>

namespace MintGB {

void CPU::executeCB(CBInstruction ins) {
    switch (ins) {
        // bit u3, r8
        case CBInstruction::BIT_0_A: bit(0, a); break;
        case CBInstruction::BIT_0_B: bit(0, b); break;
        case CBInstruction::BIT_0_C: bit(0, c); break;
        case CBInstruction::BIT_0_D: bit(0, d); break;
        case CBInstruction::BIT_0_E: bit(0, e); break;
        case CBInstruction::BIT_0_H: bit(0, h); break;
        case CBInstruction::BIT_0_L: bit(0, l); break;

        case CBInstruction::BIT_1_A: bit(1, a); break;
        case CBInstruction::BIT_1_B: bit(1, b); break;
        case CBInstruction::BIT_1_C: bit(1, c); break;
        case CBInstruction::BIT_1_D: bit(1, d); break;
        case CBInstruction::BIT_1_E: bit(1, e); break;
        case CBInstruction::BIT_1_H: bit(1, h); break;
        case CBInstruction::BIT_1_L: bit(1, l); break;

        case CBInstruction::BIT_2_A: bit(2, a); break;
        case CBInstruction::BIT_2_B: bit(2, b); break;
        case CBInstruction::BIT_2_C: bit(2, c); break;
        case CBInstruction::BIT_2_D: bit(2, d); break;
        case CBInstruction::BIT_2_E: bit(2, e); break;
        case CBInstruction::BIT_2_H: bit(2, h); break;
        case CBInstruction::BIT_2_L: bit(2, l); break;

        case CBInstruction::BIT_3_A: bit(3, a); break;
        case CBInstruction::BIT_3_B: bit(3, b); break;
        case CBInstruction::BIT_3_C: bit(3, c); break;
        case CBInstruction::BIT_3_D: bit(3, d); break;
        case CBInstruction::BIT_3_E: bit(3, e); break;
        case CBInstruction::BIT_3_H: bit(3, h); break;
        case CBInstruction::BIT_3_L: bit(3, l); break;

        case CBInstruction::BIT_4_A: bit(4, a); break;
        case CBInstruction::BIT_4_B: bit(4, b); break;
        case CBInstruction::BIT_4_C: bit(4, c); break;
        case CBInstruction::BIT_4_D: bit(4, d); break;
        case CBInstruction::BIT_4_E: bit(4, e); break;
        case CBInstruction::BIT_4_H: bit(4, h); break;
        case CBInstruction::BIT_4_L: bit(4, l); break;

        case CBInstruction::BIT_5_A: bit(5, a); break;
        case CBInstruction::BIT_5_B: bit(5, b); break;
        case CBInstruction::BIT_5_C: bit(5, c); break;
        case CBInstruction::BIT_5_D: bit(5, d); break;
        case CBInstruction::BIT_5_E: bit(5, e); break;
        case CBInstruction::BIT_5_H: bit(5, h); break;
        case CBInstruction::BIT_5_L: bit(5, l); break;

        case CBInstruction::BIT_6_A: bit(6, a); break;
        case CBInstruction::BIT_6_B: bit(6, b); break;
        case CBInstruction::BIT_6_C: bit(6, c); break;
        case CBInstruction::BIT_6_D: bit(6, d); break;
        case CBInstruction::BIT_6_E: bit(6, e); break;
        case CBInstruction::BIT_6_H: bit(6, h); break;
        case CBInstruction::BIT_6_L: bit(6, l); break;

        case CBInstruction::BIT_7_A: bit(7, a); break;
        case CBInstruction::BIT_7_B: bit(7, b); break;
        case CBInstruction::BIT_7_C: bit(7, c); break;
        case CBInstruction::BIT_7_D: bit(7, d); break;
        case CBInstruction::BIT_7_E: bit(7, e); break;
        case CBInstruction::BIT_7_H: bit(7, h); break;
        case CBInstruction::BIT_7_L: bit(7, l); break;

        // bit u3, [hl]
        case CBInstruction::BIT_0_IndirectHL: bit(0, readByte(hl())); break;
        case CBInstruction::BIT_1_IndirectHL: bit(1, readByte(hl())); break;
        case CBInstruction::BIT_2_IndirectHL: bit(2, readByte(hl())); break;
        case CBInstruction::BIT_3_IndirectHL: bit(3, readByte(hl())); break;       
        case CBInstruction::BIT_4_IndirectHL: bit(4, readByte(hl())); break;
        case CBInstruction::BIT_5_IndirectHL: bit(5, readByte(hl())); break;
        case CBInstruction::BIT_6_IndirectHL: bit(6, readByte(hl())); break;
        case CBInstruction::BIT_7_IndirectHL: bit(7, readByte(hl())); break;

        // res u3, r8
        case CBInstruction::RES_0_A: res(0, a); break;
        case CBInstruction::RES_0_B: res(0, b); break;
        case CBInstruction::RES_0_C: res(0, c); break;
        case CBInstruction::RES_0_D: res(0, d); break;
        case CBInstruction::RES_0_E: res(0, e); break;
        case CBInstruction::RES_0_H: res(0, h); break;
        case CBInstruction::RES_0_L: res(0, l); break;

        case CBInstruction::RES_1_A: res(1, a); break;
        case CBInstruction::RES_1_B: res(1, b); break;
        case CBInstruction::RES_1_C: res(1, c); break;
        case CBInstruction::RES_1_D: res(1, d); break;
        case CBInstruction::RES_1_E: res(1, e); break;
        case CBInstruction::RES_1_H: res(1, h); break;
        case CBInstruction::RES_1_L: res(1, l); break;

        case CBInstruction::RES_2_A: res(2, a); break;
        case CBInstruction::RES_2_B: res(2, b); break;
        case CBInstruction::RES_2_C: res(2, c); break;
        case CBInstruction::RES_2_D: res(2, d); break;
        case CBInstruction::RES_2_E: res(2, e); break;
        case CBInstruction::RES_2_H: res(2, h); break;
        case CBInstruction::RES_2_L: res(2, l); break;

        case CBInstruction::RES_3_A: res(3, a); break;
        case CBInstruction::RES_3_B: res(3, b); break;
        case CBInstruction::RES_3_C: res(3, c); break;
        case CBInstruction::RES_3_D: res(3, d); break;
        case CBInstruction::RES_3_E: res(3, e); break;
        case CBInstruction::RES_3_H: res(3, h); break;
        case CBInstruction::RES_3_L: res(3, l); break;

        case CBInstruction::RES_4_A: res(4, a); break;
        case CBInstruction::RES_4_B: res(4, b); break;
        case CBInstruction::RES_4_C: res(4, c); break;
        case CBInstruction::RES_4_D: res(4, d); break;
        case CBInstruction::RES_4_E: res(4, e); break;
        case CBInstruction::RES_4_H: res(4, h); break;
        case CBInstruction::RES_4_L: res(4, l); break;

        case CBInstruction::RES_5_A: res(5, a); break;
        case CBInstruction::RES_5_B: res(5, b); break;
        case CBInstruction::RES_5_C: res(5, c); break;
        case CBInstruction::RES_5_D: res(5, d); break;
        case CBInstruction::RES_5_E: res(5, e); break;
        case CBInstruction::RES_5_H: res(5, h); break;
        case CBInstruction::RES_5_L: res(5, l); break;

        case CBInstruction::RES_6_A: res(6, a); break;
        case CBInstruction::RES_6_B: res(6, b); break;
        case CBInstruction::RES_6_C: res(6, c); break;
        case CBInstruction::RES_6_D: res(6, d); break;
        case CBInstruction::RES_6_E: res(6, e); break;
        case CBInstruction::RES_6_H: res(6, h); break;
        case CBInstruction::RES_6_L: res(6, l); break;

        case CBInstruction::RES_7_A: res(7, a); break;
        case CBInstruction::RES_7_B: res(7, b); break;
        case CBInstruction::RES_7_C: res(7, c); break;
        case CBInstruction::RES_7_D: res(7, d); break;
        case CBInstruction::RES_7_E: res(7, e); break;
        case CBInstruction::RES_7_H: res(7, h); break;
        case CBInstruction::RES_7_L: res(7, l); break;

        // res u3, [hl]
        case CBInstruction::RES_0_IndirectHL: {
            uint8_t value = readByte(hl());
            res(0, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::RES_1_IndirectHL: {
            uint8_t value = readByte(hl());
            res(1, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::RES_2_IndirectHL: {
            uint8_t value = readByte(hl());
            res(2, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::RES_3_IndirectHL: {
            uint8_t value = readByte(hl());
            res(3, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::RES_4_IndirectHL: {
            uint8_t value = readByte(hl());
            res(4, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::RES_5_IndirectHL: {
            uint8_t value = readByte(hl());
            res(5, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::RES_6_IndirectHL: {
            uint8_t value = readByte(hl());
            res(6, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::RES_7_IndirectHL: {
            uint8_t value = readByte(hl());
            res(7, value);
            writeByte(hl(), value);
        } break;

        // set u3, r8
        case CBInstruction::SET_0_A: set(0, a); break;
        case CBInstruction::SET_0_B: set(0, b); break;
        case CBInstruction::SET_0_C: set(0, c); break;
        case CBInstruction::SET_0_D: set(0, d); break;
        case CBInstruction::SET_0_E: set(0, e); break;
        case CBInstruction::SET_0_H: set(0, h); break;
        case CBInstruction::SET_0_L: set(0, l); break;

        case CBInstruction::SET_1_A: set(1, a); break;
        case CBInstruction::SET_1_B: set(1, b); break;
        case CBInstruction::SET_1_C: set(1, c); break;
        case CBInstruction::SET_1_D: set(1, d); break;
        case CBInstruction::SET_1_E: set(1, e); break;
        case CBInstruction::SET_1_H: set(1, h); break;
        case CBInstruction::SET_1_L: set(1, l); break;

        case CBInstruction::SET_2_A: set(2, a); break;
        case CBInstruction::SET_2_B: set(2, b); break;
        case CBInstruction::SET_2_C: set(2, c); break;
        case CBInstruction::SET_2_D: set(2, d); break;
        case CBInstruction::SET_2_E: set(2, e); break;
        case CBInstruction::SET_2_H: set(2, h); break;
        case CBInstruction::SET_2_L: set(2, l); break;

        case CBInstruction::SET_3_A: set(3, a); break;
        case CBInstruction::SET_3_B: set(3, b); break;
        case CBInstruction::SET_3_C: set(3, c); break;
        case CBInstruction::SET_3_D: set(3, d); break;
        case CBInstruction::SET_3_E: set(3, e); break;
        case CBInstruction::SET_3_H: set(3, h); break;
        case CBInstruction::SET_3_L: set(3, l); break;

        case CBInstruction::SET_4_A: set(4, a); break;
        case CBInstruction::SET_4_B: set(4, b); break;
        case CBInstruction::SET_4_C: set(4, c); break;
        case CBInstruction::SET_4_D: set(4, d); break;
        case CBInstruction::SET_4_E: set(4, e); break;
        case CBInstruction::SET_4_H: set(4, h); break;
        case CBInstruction::SET_4_L: set(4, l); break;

        case CBInstruction::SET_5_A: set(5, a); break;
        case CBInstruction::SET_5_B: set(5, b); break;
        case CBInstruction::SET_5_C: set(5, c); break;
        case CBInstruction::SET_5_D: set(5, d); break;
        case CBInstruction::SET_5_E: set(5, e); break;
        case CBInstruction::SET_5_H: set(5, h); break;
        case CBInstruction::SET_5_L: set(5, l); break;

        case CBInstruction::SET_6_A: set(6, a); break;
        case CBInstruction::SET_6_B: set(6, b); break;
        case CBInstruction::SET_6_C: set(6, c); break;
        case CBInstruction::SET_6_D: set(6, d); break;
        case CBInstruction::SET_6_E: set(6, e); break;
        case CBInstruction::SET_6_H: set(6, h); break;
        case CBInstruction::SET_6_L: set(6, l); break;

        case CBInstruction::SET_7_A: set(7, a); break;
        case CBInstruction::SET_7_B: set(7, b); break;
        case CBInstruction::SET_7_C: set(7, c); break;
        case CBInstruction::SET_7_D: set(7, d); break;
        case CBInstruction::SET_7_E: set(7, e); break;
        case CBInstruction::SET_7_H: set(7, h); break;
        case CBInstruction::SET_7_L: set(7, l); break;

        // set u3, [hl]
        case CBInstruction::SET_0_IndirectHL: {
            uint8_t value = readByte(hl());
            set(0, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::SET_1_IndirectHL: {
            uint8_t value = readByte(hl());
            set(1, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::SET_2_IndirectHL: {
            uint8_t value = readByte(hl());
            set(2, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::SET_3_IndirectHL: {
            uint8_t value = readByte(hl());
            set(3, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::SET_4_IndirectHL: {
            uint8_t value = readByte(hl());
            set(4, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::SET_5_IndirectHL: {
            uint8_t value = readByte(hl());
            set(5, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::SET_6_IndirectHL: {
            uint8_t value = readByte(hl());
            set(6, value);
            writeByte(hl(), value);
        } break;
        case CBInstruction::SET_7_IndirectHL: {
            uint8_t value = readByte(hl());
            set(7, value);
            writeByte(hl(), value);
        } break;

        // rl r8
        case CBInstruction::RL_A: rl(a); break;
        case CBInstruction::RL_B: rl(b); break;
        case CBInstruction::RL_C: rl(c); break;
        case CBInstruction::RL_D: rl(d); break;
        case CBInstruction::RL_E: rl(e); break;
        case CBInstruction::RL_H: rl(h); break;
        case CBInstruction::RL_L: rl(l); break;

        // rl [hl]
        case CBInstruction::RL_IndirectHL: {
            uint8_t value = readByte(hl());
            rl(value);
            writeByte(hl(), value);
        } break;

        // rlc r8
        case CBInstruction::RLC_A: rlc(a); break;
        case CBInstruction::RLC_B: rlc(b); break;
        case CBInstruction::RLC_C: rlc(c); break;
        case CBInstruction::RLC_D: rlc(d); break;
        case CBInstruction::RLC_E: rlc(e); break;
        case CBInstruction::RLC_H: rlc(h); break;
        case CBInstruction::RLC_L: rlc(l); break;

        // rlc [hl]
        case CBInstruction::RLC_IndirectHL: {
            uint8_t value = readByte(hl());
            rlc(value);
            writeByte(hl(), value);
        } break;

        // rr r8
        case CBInstruction::RR_A: rr(a); break;
        case CBInstruction::RR_B: rr(b); break;
        case CBInstruction::RR_C: rr(c); break;
        case CBInstruction::RR_D: rr(d); break;
        case CBInstruction::RR_E: rr(e); break;
        case CBInstruction::RR_H: rr(h); break;
        case CBInstruction::RR_L: rr(l); break;

        // rr [hl]
        case CBInstruction::RR_IndirectHL: {
            uint8_t value = readByte(hl());
            rr(value);
            writeByte(hl(), value);
        } break;

        // rrc r8
        case CBInstruction::RRC_A: rrc(a); break;
        case CBInstruction::RRC_B: rrc(b); break;
        case CBInstruction::RRC_C: rrc(c); break;
        case CBInstruction::RRC_D: rrc(d); break;
        case CBInstruction::RRC_E: rrc(e); break;
        case CBInstruction::RRC_H: rrc(h); break;
        case CBInstruction::RRC_L: rrc(l); break;

        // rrc [hl]
        case CBInstruction::RRC_IndirectHL: {
            uint8_t value = readByte(hl());
            rrc(value);
            writeByte(hl(), value);
        } break;

        // sla r8
        case CBInstruction::SLA_A: sla(a); break;
        case CBInstruction::SLA_B: sla(b); break;
        case CBInstruction::SLA_C: sla(c); break;
        case CBInstruction::SLA_D: sla(d); break;
        case CBInstruction::SLA_E: sla(e); break;
        case CBInstruction::SLA_H: sla(h); break;
        case CBInstruction::SLA_L: sla(l); break;

        // sla [hl]
        case CBInstruction::SLA_IndirectHL: {
            uint8_t value = readByte(hl());
            sla(value);
            writeByte(hl(), value);
        } break;

        // sra r8
        case CBInstruction::SRA_A: sra(a); break;
        case CBInstruction::SRA_B: sra(b); break;
        case CBInstruction::SRA_C: sra(c); break;
        case CBInstruction::SRA_D: sra(d); break;
        case CBInstruction::SRA_E: sra(e); break;
        case CBInstruction::SRA_H: sra(h); break;
        case CBInstruction::SRA_L: sra(l); break;

        // sra [hl]
        case CBInstruction::SRA_IndirectHL: {
            uint8_t value = readByte(hl());
            sra(value);
            writeByte(hl(), value);
        } break;

        // srl r8
        case CBInstruction::SRL_A: sla(a); break;
        case CBInstruction::SRL_B: sla(b); break;
        case CBInstruction::SRL_C: sla(c); break;
        case CBInstruction::SRL_D: sla(d); break;
        case CBInstruction::SRL_E: sla(e); break;
        case CBInstruction::SRL_H: sla(h); break;
        case CBInstruction::SRL_L: sla(l); break;

        // srl [hl]
        case CBInstruction::SRL_IndirectHL: {
            uint8_t value = readByte(hl());
            srl(value);
            writeByte(hl(), value);
        } break;

        // swap r8
        case CBInstruction::SWAP_A: swap(a); break;
        case CBInstruction::SWAP_B: swap(b); break;
        case CBInstruction::SWAP_C: swap(c); break;
        case CBInstruction::SWAP_D: swap(d); break;
        case CBInstruction::SWAP_E: swap(e); break;
        case CBInstruction::SWAP_H: swap(h); break;
        case CBInstruction::SWAP_L: swap(l); break;

        // swap [hl]
        case CBInstruction::SWAP_IndirectHL: {
            uint8_t value = readByte(hl());
            swap(value);
            writeByte(hl(), value);
        } break; 

        default:
            throw std::runtime_error(std::format("Illegal CB opcode reached: {:02X}", static_cast<uint8_t>(ins)));
    }
}

}