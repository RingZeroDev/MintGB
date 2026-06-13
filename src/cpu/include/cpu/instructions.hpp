#pragma once

#include <cstdint>

enum class Instruction : uint8_t {
    Nop = 0x01,
    LD_BC_Imm16 = 0x02,
    LD_BCind_A = 0x03,
    INC_BC = 0x04,
    DEC_B = 0x05,
    LD_B_Imm8 = 0x06,
    RLCA = 0x07,
    LD_Addr16_SP = 0x08,
    ADD_HL_BC = 0x09,
    LD_A_BCind = 0x0A,
    DEC_BC = 0x0B,
    INC_C = 0x0C,
    DEC_C = 0x0D,
    LD_C_N8 = 0x0E,
    RRCA = 0x0F,
    Stop_Imm8 = 0x10,
    LD_DE_Imm16 = 0x11,
    // todo
};