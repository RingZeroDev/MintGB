#include "sm83.hpp"

#define INS(opcode, operation, ...) case opcode: operation(__VA_ARGS__); break

void SM83::executeCB(uint8_t opcode) {
    switch (opcode) {
        // rlc r8
        INS(0x07, rlc_r8, a);
        INS(0x00, rlc_r8, b);
        INS(0x01, rlc_r8, c);
        INS(0x02, rlc_r8, d);
        INS(0x03, rlc_r8, e);
        INS(0x04, rlc_r8, h);
        INS(0x05, rlc_r8, l);

        // rlc [hl]
        INS(0x06, rlc_ind_hl);

        // rrc r8
        INS(0x0F, rrc_r8, a);
        INS(0x08, rrc_r8, b);
        INS(0x09, rrc_r8, c);
        INS(0x0A, rrc_r8, d);
        INS(0x0B, rrc_r8, e);
        INS(0x0C, rrc_r8, h);
        INS(0x0D, rrc_r8, l);

        // rrc [hl]
        INS(0x0E, rrc_ind_hl);

        // rl r8 
        INS(0x17, rl_r8, a);
        INS(0x10, rl_r8, b);
        INS(0x11, rl_r8, c);
        INS(0x12, rl_r8, d);
        INS(0x13, rl_r8, e);
        INS(0x14, rl_r8, h);
        INS(0x15, rl_r8, l);

        // rl [hl]
        INS(0x16, rl_ind_hl);

        // rr r8
        INS(0x1F, rr_r8, a);
        INS(0x18, rr_r8, b);
        INS(0x19, rr_r8, c);
        INS(0x1A, rr_r8, d);
        INS(0x1B, rr_r8, e);
        INS(0x1C, rr_r8, h);
        INS(0x1D, rr_r8, l);

        // rr [hl]
        INS(0x1E, rr_ind_hl);

        // sla r8
        INS(0x27, sla_r8, a);
        INS(0x20, sla_r8, b);
        INS(0x21, sla_r8, c);
        INS(0x22, sla_r8, d);
        INS(0x23, sla_r8, e);
        INS(0x24, sla_r8, h);
        INS(0x25, sla_r8, l);

        // sla [hl]
        INS(0x26, sla_ind_hl);

        // sra r8
        INS(0x2F, sra_r8, a);
        INS(0x28, sra_r8, b);
        INS(0x29, sra_r8, c);
        INS(0x2A, sra_r8, d);
        INS(0x2B, sra_r8, e);
        INS(0x2C, sra_r8, h);
        INS(0x2D, sra_r8, l);

        // sra [hl]
        INS(0x2E, sra_ind_hl);

        // swap r8
        INS(0x37, swap_r8, a);
        INS(0x30, swap_r8, b);
        INS(0x31, swap_r8, c);
        INS(0x32, swap_r8, d);
        INS(0x33, swap_r8, e);
        INS(0x34, swap_r8, h);
        INS(0x35, swap_r8, l);

        // swap [hl]
        INS(0x36, swap_ind_hl);

        // srl r8
        INS(0x3F, srl_r8, a);
        INS(0x38, srl_r8, b);
        INS(0x39, srl_r8, c);
        INS(0x3A, srl_r8, d);
        INS(0x3B, srl_r8, e);
        INS(0x3C, srl_r8, h);
        INS(0x3D, srl_r8, l);

        // srl [hl]
        INS(0x3E, srl_ind_hl);

        // bit u3, r8
        INS(0x47, bit_u3_r8, 0, a);
        INS(0x40, bit_u3_r8, 0, b);
        INS(0x41, bit_u3_r8, 0, c);
        INS(0x42, bit_u3_r8, 0, d);
        INS(0x43, bit_u3_r8, 0, e);
        INS(0x44, bit_u3_r8, 0, h);
        INS(0x45, bit_u3_r8, 0, l);

        INS(0x4F, bit_u3_r8, 1, a);
        INS(0x48, bit_u3_r8, 1, b);
        INS(0x49, bit_u3_r8, 1, c);
        INS(0x4A, bit_u3_r8, 1, d);
        INS(0x4B, bit_u3_r8, 1, e);
        INS(0x4C, bit_u3_r8, 1, h);
        INS(0x4D, bit_u3_r8, 1, l);

        INS(0x57, bit_u3_r8, 2, a);
        INS(0x50, bit_u3_r8, 2, b);
        INS(0x51, bit_u3_r8, 2, c);
        INS(0x52, bit_u3_r8, 2, d);
        INS(0x53, bit_u3_r8, 2, e);
        INS(0x54, bit_u3_r8, 2, h);
        INS(0x55, bit_u3_r8, 2, l);

        INS(0x5F, bit_u3_r8, 3, a);
        INS(0x58, bit_u3_r8, 3, b);
        INS(0x59, bit_u3_r8, 3, c);
        INS(0x5A, bit_u3_r8, 3, d);
        INS(0x5B, bit_u3_r8, 3, e);
        INS(0x5C, bit_u3_r8, 3, h);
        INS(0x5D, bit_u3_r8, 3, l);

        INS(0x67, bit_u3_r8, 4, a);
        INS(0x60, bit_u3_r8, 4, b);
        INS(0x61, bit_u3_r8, 4, c);
        INS(0x62, bit_u3_r8, 4, d);
        INS(0x63, bit_u3_r8, 4, e);
        INS(0x64, bit_u3_r8, 4, h);
        INS(0x65, bit_u3_r8, 4, l);

        INS(0x6F, bit_u3_r8, 5, a);
        INS(0x68, bit_u3_r8, 5, b);
        INS(0x69, bit_u3_r8, 5, c);
        INS(0x6A, bit_u3_r8, 5, d);
        INS(0x6B, bit_u3_r8, 5, e);
        INS(0x6C, bit_u3_r8, 5, h);
        INS(0x6D, bit_u3_r8, 5, l);

        INS(0x77, bit_u3_r8, 6, a);
        INS(0x70, bit_u3_r8, 6, b);
        INS(0x71, bit_u3_r8, 6, c);
        INS(0x72, bit_u3_r8, 6, d);
        INS(0x73, bit_u3_r8, 6, e);
        INS(0x74, bit_u3_r8, 6, h);
        INS(0x75, bit_u3_r8, 6, l);

        INS(0x7F, bit_u3_r8, 7, a);
        INS(0x78, bit_u3_r8, 7, b);
        INS(0x79, bit_u3_r8, 7, c);
        INS(0x7A, bit_u3_r8, 7, d);
        INS(0x7B, bit_u3_r8, 7, e);
        INS(0x7C, bit_u3_r8, 7, h);
        INS(0x7D, bit_u3_r8, 7, l);

        // bit u3, [hl]
        INS(0x46, bit_u3_ind_hl, 0);
        INS(0x4E, bit_u3_ind_hl, 1);
        INS(0x56, bit_u3_ind_hl, 2);
        INS(0x5E, bit_u3_ind_hl, 3);
        INS(0x66, bit_u3_ind_hl, 4);
        INS(0x6E, bit_u3_ind_hl, 5);
        INS(0x76, bit_u3_ind_hl, 6);
        INS(0x7E, bit_u3_ind_hl, 7);

         // res u3, r8
        INS(0x87, res_u3_r8, 0, a);
        INS(0x80, res_u3_r8, 0, b);
        INS(0x81, res_u3_r8, 0, c);
        INS(0x82, res_u3_r8, 0, d);
        INS(0x83, res_u3_r8, 0, e);
        INS(0x84, res_u3_r8, 0, h);
        INS(0x85, res_u3_r8, 0, l);

        INS(0x8F, res_u3_r8, 1, a);
        INS(0x88, res_u3_r8, 1, b);
        INS(0x89, res_u3_r8, 1, c);
        INS(0x8A, res_u3_r8, 1, d);
        INS(0x8B, res_u3_r8, 1, e);
        INS(0x8C, res_u3_r8, 1, h);
        INS(0x8D, res_u3_r8, 1, l);

        INS(0x97, res_u3_r8, 2, a);
        INS(0x90, res_u3_r8, 2, b);
        INS(0x91, res_u3_r8, 2, c);
        INS(0x92, res_u3_r8, 2, d);
        INS(0x93, res_u3_r8, 2, e);
        INS(0x94, res_u3_r8, 2, h);
        INS(0x95, res_u3_r8, 2, l);

        INS(0x9F, res_u3_r8, 3, a);
        INS(0x98, res_u3_r8, 3, b);
        INS(0x99, res_u3_r8, 3, c);
        INS(0x9A, res_u3_r8, 3, d);
        INS(0x9B, res_u3_r8, 3, e);
        INS(0x9C, res_u3_r8, 3, h);
        INS(0x9D, res_u3_r8, 3, l);

        INS(0xA7, res_u3_r8, 4, a);
        INS(0xA0, res_u3_r8, 4, b);
        INS(0xA1, res_u3_r8, 4, c);
        INS(0xA2, res_u3_r8, 4, d);
        INS(0xA3, res_u3_r8, 4, e);
        INS(0xA4, res_u3_r8, 4, h);
        INS(0xA5, res_u3_r8, 4, l);

        INS(0xAF, res_u3_r8, 5, a);
        INS(0xA8, res_u3_r8, 5, b);
        INS(0xA9, res_u3_r8, 5, c);
        INS(0xAA, res_u3_r8, 5, d);
        INS(0xAB, res_u3_r8, 5, e);
        INS(0xAC, res_u3_r8, 5, h);
        INS(0xAD, res_u3_r8, 5, l);

        INS(0xB7, res_u3_r8, 6, a);
        INS(0xB0, res_u3_r8, 6, b);
        INS(0xB1, res_u3_r8, 6, c);
        INS(0xB2, res_u3_r8, 6, d);
        INS(0xB3, res_u3_r8, 6, e);
        INS(0xB4, res_u3_r8, 6, h);
        INS(0xB5, res_u3_r8, 6, l);

        INS(0xBF, res_u3_r8, 7, a);
        INS(0xB8, res_u3_r8, 7, b);
        INS(0xB9, res_u3_r8, 7, c);
        INS(0xBA, res_u3_r8, 7, d);
        INS(0xBB, res_u3_r8, 7, e);
        INS(0xBC, res_u3_r8, 7, h);
        INS(0xBD, res_u3_r8, 7, l);

        // res u3, [hl]
        INS(0x86, res_u3_ind_hl, 0);
        INS(0x8E, res_u3_ind_hl, 1);
        INS(0x96, res_u3_ind_hl, 2);
        INS(0x9E, res_u3_ind_hl, 3);
        INS(0xA6, res_u3_ind_hl, 4);
        INS(0xAE, res_u3_ind_hl, 5);
        INS(0xB6, res_u3_ind_hl, 6);
        INS(0xBE, res_u3_ind_hl, 7);

        // set u3, r8
        INS(0xC7, set_u3_r8, 0, a);
        INS(0xC0, set_u3_r8, 0, b);
        INS(0xC1, set_u3_r8, 0, c);
        INS(0xC2, set_u3_r8, 0, d);
        INS(0xC3, set_u3_r8, 0, e);
        INS(0xC4, set_u3_r8, 0, h);
        INS(0xC5, set_u3_r8, 0, l);

        INS(0xCF, set_u3_r8, 1, a);
        INS(0xC8, set_u3_r8, 1, b);
        INS(0xC9, set_u3_r8, 1, c);
        INS(0xCA, set_u3_r8, 1, d);
        INS(0xCB, set_u3_r8, 1, e);
        INS(0xCC, set_u3_r8, 1, h);
        INS(0xCD, set_u3_r8, 1, l);

        INS(0xD7, set_u3_r8, 2, a);
        INS(0xD0, set_u3_r8, 2, b);
        INS(0xD1, set_u3_r8, 2, c);
        INS(0xD2, set_u3_r8, 2, d);
        INS(0xD3, set_u3_r8, 2, e);
        INS(0xD4, set_u3_r8, 2, h);
        INS(0xD5, set_u3_r8, 2, l);

        INS(0xDF, set_u3_r8, 3, a);
        INS(0xD8, set_u3_r8, 3, b);
        INS(0xD9, set_u3_r8, 3, c);
        INS(0xDA, set_u3_r8, 3, d);
        INS(0xDB, set_u3_r8, 3, e);
        INS(0xDC, set_u3_r8, 3, h);
        INS(0xDD, set_u3_r8, 3, l);

        INS(0xE7, set_u3_r8, 4, a);
        INS(0xE0, set_u3_r8, 4, b);
        INS(0xE1, set_u3_r8, 4, c);
        INS(0xE2, set_u3_r8, 4, d);
        INS(0xE3, set_u3_r8, 4, e);
        INS(0xE4, set_u3_r8, 4, h);
        INS(0xE5, set_u3_r8, 4, l);

        INS(0xEF, set_u3_r8, 5, a);
        INS(0xE8, set_u3_r8, 5, b);
        INS(0xE9, set_u3_r8, 5, c);
        INS(0xEA, set_u3_r8, 5, d);
        INS(0xEB, set_u3_r8, 5, e);
        INS(0xEC, set_u3_r8, 5, h);
        INS(0xED, set_u3_r8, 5, l);

        INS(0xF7, set_u3_r8, 6, a);
        INS(0xF0, set_u3_r8, 6, b);
        INS(0xF1, set_u3_r8, 6, c);
        INS(0xF2, set_u3_r8, 6, d);
        INS(0xF3, set_u3_r8, 6, e);
        INS(0xF4, set_u3_r8, 6, h);
        INS(0xF5, set_u3_r8, 6, l);

        INS(0xFF, set_u3_r8, 7, a);
        INS(0xF8, set_u3_r8, 7, b);
        INS(0xF9, set_u3_r8, 7, c);
        INS(0xFA, set_u3_r8, 7, d);
        INS(0xFB, set_u3_r8, 7, e);
        INS(0xFC, set_u3_r8, 7, h);
        INS(0xFD, set_u3_r8, 7, l);

        // set u3, [hl]
        INS(0xC6, set_u3_ind_hl, 0);
        INS(0xCE, set_u3_ind_hl, 1);
        INS(0xD6, set_u3_ind_hl, 2);
        INS(0xDE, set_u3_ind_hl, 3);
        INS(0xE6, set_u3_ind_hl, 4);
        INS(0xEE, set_u3_ind_hl, 5);
        INS(0xF6, set_u3_ind_hl, 6);
        INS(0xFE, set_u3_ind_hl, 7);
    }
}