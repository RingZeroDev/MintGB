#include "cpu.hpp" 

#include <stdexcept>
#include <format>

#define INS(opcode, operation, ...) case opcode: operation(__VA_ARGS__); break

#define NC !carry
#define C carry
#define NZ !zero
#define Z zero 

void CPU::decode(uint8_t opcode) {
    switch (opcode) {
        // ld r8, r8
        INS(0x7F, ld_r8_r8, a, a);
        INS(0x78, ld_r8_r8, a, b);
        INS(0x79, ld_r8_r8, a, c);
        INS(0x7A, ld_r8_r8, a, d);
        INS(0x7B, ld_r8_r8, a, e);
        INS(0x7C, ld_r8_r8, a, h);
        INS(0x7D, ld_r8_r8, a, l);

        INS(0x47, ld_r8_r8, b, a); 
        INS(0x40, ld_r8_r8, b, b);
        INS(0x41, ld_r8_r8, b, c);
        INS(0x42, ld_r8_r8, b, d);
        INS(0x43, ld_r8_r8, b, e);
        INS(0x44, ld_r8_r8, b, h);
        INS(0x45, ld_r8_r8, b, l);

        INS(0x4F, ld_r8_r8, c, a); 
        INS(0x48, ld_r8_r8, c, b); 
        INS(0x49, ld_r8_r8, c, c); 
        INS(0x4A, ld_r8_r8, c, d); 
        INS(0x4B, ld_r8_r8, c, e); 
        INS(0x4C, ld_r8_r8, c, h); 
        INS(0x4D, ld_r8_r8, c, l); 

        INS(0x57, ld_r8_r8, d, a); 
        INS(0x50, ld_r8_r8, d, b); 
        INS(0x51, ld_r8_r8, d, c); 
        INS(0x52, ld_r8_r8, d, d); 
        INS(0x53, ld_r8_r8, d, e); 
        INS(0x54, ld_r8_r8, d, h); 
        INS(0x55, ld_r8_r8, d, l); 

        INS(0x5F, ld_r8_r8, e, a); 
        INS(0x58, ld_r8_r8, e, b); 
        INS(0x59, ld_r8_r8, e, c); 
        INS(0x5A, ld_r8_r8, e, d); 
        INS(0x5B, ld_r8_r8, e, e); 
        INS(0x5C, ld_r8_r8, e, h); 
        INS(0x5D, ld_r8_r8, e, l); 

        INS(0x67, ld_r8_r8, h, a); 
        INS(0x60, ld_r8_r8, h, b); 
        INS(0x61, ld_r8_r8, h, c); 
        INS(0x62, ld_r8_r8, h, d); 
        INS(0x63, ld_r8_r8, h, e); 
        INS(0x64, ld_r8_r8, h, h); 
        INS(0x65, ld_r8_r8, h, l); 

        INS(0x6F, ld_r8_r8, l, a); 
        INS(0x68, ld_r8_r8, l, b); 
        INS(0x69, ld_r8_r8, l, c); 
        INS(0x6A, ld_r8_r8, l, d); 
        INS(0x6B, ld_r8_r8, l, e); 
        INS(0x6C, ld_r8_r8, l, h); 
        INS(0x6D, ld_r8_r8, l, l); 

        // ld r8, n8
        INS(0x3E, ld_r8_n8, a);
        INS(0x06, ld_r8_n8, b); 
        INS(0x0E, ld_r8_n8, c);
        INS(0x16, ld_r8_n8, d); 
        INS(0x1E, ld_r8_n8, e); 
        INS(0x26, ld_r8_n8, h);
        INS(0x2E, ld_r8_n8, l);

        // ld r16, n16
        INS(0x01, ld_r16_n16, bc);
        INS(0x11, ld_r16_n16, de); 
        INS(0x21, ld_r16_n16, hl); 

        // ld sp, n16
        INS(0x31, ld_sp_n16);

        // ld [HL], r8
        INS(0x77, ld_ind_hl_r8, a);
        INS(0x70, ld_ind_hl_r8, b); 
        INS(0x71, ld_ind_hl_r8, c);
        INS(0x72, ld_ind_hl_r8, d);
        INS(0x73, ld_ind_hl_r8, e);
        INS(0x74, ld_ind_hl_r8, h);
        INS(0x75, ld_ind_hl_r8, l);

        // ld [hl], n8
        INS(0x36, ld_ind_hl_n8);

        // ld r8, [hl]
        INS(0x7E, ld_r8_ind_hl, a);
        INS(0x46, ld_r8_ind_hl, b);
        INS(0x4E, ld_r8_ind_hl, c);
        INS(0x56, ld_r8_ind_hl, d);
        INS(0x5E, ld_r8_ind_hl, e);
        INS(0x66, ld_r8_ind_hl, h);
        INS(0x6E, ld_r8_ind_hl, l);

        // ld [r16], a
        INS(0x02, ld_ind_r16_a, bc);
        INS(0x12, ld_ind_r16_a, de);
        
        // ld [n16], a
        INS(0xEA, ld_a16_a);

        // ldh [n16], a
        INS(0xE0, ldh_a16_a);

        // ldh [c], a
        INS(0xE2, ldh_ind_c_a);

        // ld a, [r16]
        INS(0x0A, ld_a_ind_r16, bc);
        INS(0x1A, ld_a_ind_r16, de); 

        // ld a, [n16]
        INS(0xFA, ld_a_a16);

        // ldh a, [n16]
        INS(0xF0, ldh_a_a16);

        // ldh a, [c]
        INS(0xF2, ldh_a_ind_c);

        // ld [hl+], a
        INS(0x22, ld_ind_hli_a);
        
        // ld [hl-], a
        INS(0x32, ld_ind_hld_a);

        // ld a, [hl+]
        INS(0x2A, ld_a_ind_hli);
        
        // ld a, [hl-]
        INS(0x3A, ld_a_ind_hld);

        // ld [n16], sp
        INS(0x08, ld_a16_sp);

        // adc a, r8
        INS(0x8F, adc_a_r8, a);
        INS(0x88, adc_a_r8, b);
        INS(0x89, adc_a_r8, c);
        INS(0x8A, adc_a_r8, d);
        INS(0x8B, adc_a_r8, e);
        INS(0x8C, adc_a_r8, h);
        INS(0x8D, adc_a_r8, l);

        // adc a, [hl]
        INS(0x8E, adc_a_ind_hl);

        // adc a, n8
        INS(0xCE, adc_a_n8);

        // add a, r8
        INS(0x87, add_a_r8, a);
        INS(0x80, add_a_r8, b);
        INS(0x81, add_a_r8, c);
        INS(0x82, add_a_r8, d);
        INS(0x83, add_a_r8, e);
        INS(0x84, add_a_r8, h);
        INS(0x85, add_a_r8, l);

        // add a, [hl]
        INS(0x86, add_a_ind_hl);

        // add a, n8
        INS(0xC6, add_a_n8);

        // cp a, r8
        INS(0xBF, cp_a_r8, a);
        INS(0xB8, cp_a_r8, b);
        INS(0xB9, cp_a_r8, c);
        INS(0xBA, cp_a_r8, d);
        INS(0xBB, cp_a_r8, e);
        INS(0xBC, cp_a_r8, h);
        INS(0xBD, cp_a_r8, l);

        // cp a, (hl)
        INS(0xBE, cp_a_ind_hl);

        // cp a, n8
        INS(0xFE, cp_a_n8);

        // dec r8
        INS(0x3D, dec_r8, a);
        INS(0x05, dec_r8, b);
        INS(0x0D, dec_r8, c);
        INS(0x15, dec_r8, d);
        INS(0x1D, dec_r8, e);
        INS(0x25, dec_r8, h);
        INS(0x2D, dec_r8, l);

        // dec [hl]
        INS(0x35, dec_ind_hl);

        // inc r8
        INS(0x3C, inc_r8, a);
        INS(0x04, inc_r8, b);
        INS(0x0C, inc_r8, c);
        INS(0x14, inc_r8, d);
        INS(0x1C, inc_r8, e);
        INS(0x24, inc_r8, h);
        INS(0x2C, inc_r8, l);

        // inc [hl]
        INS(0x34, inc_ind_hl);

        // sbc a, r8
        INS(0x9F, sbc_a_r8, a);
        INS(0x98, sbc_a_r8, b);
        INS(0x99, sbc_a_r8, c);
        INS(0x9A, sbc_a_r8, d);
        INS(0x9B, sbc_a_r8, e);
        INS(0x9C, sbc_a_r8, h);
        INS(0x9D, sbc_a_r8, l);

        // sbc a, [hl]
        INS(0x9E, sbc_a_ind_hl);

        // sbc a, n8
        INS(0xDE, sbc_a_n8);

        // sub a, r8
        INS(0x97, sub_a_r8, a);
        INS(0x90, sub_a_r8, b);
        INS(0x91, sub_a_r8, c);
        INS(0x92, sub_a_r8, d);
        INS(0x93, sub_a_r8, e);
        INS(0x94, sub_a_r8, h);
        INS(0x95, sub_a_r8, l);
        
        // sub a, [hl]
        INS(0x96, sub_a_ind_hl);

        // sub a, n8
        INS(0xD6, sub_a_n8);

        // add hl, r16
        INS(0x09, add_hl_r16, bc);
        INS(0x19, add_hl_r16, de);
        INS(0x29, add_hl_r16, hl);

        // add hl, sp
        INS(0x39, add_hl_sp);

        // dec r16
        INS(0x0B, dec_r16, bc);
        INS(0x1B, dec_r16, de);
        INS(0x2B, dec_r16, hl);
        
        // dec sp
        INS(0x3B, dec_sp);

        // inc r16
        INS(0x03, inc_r16, bc);
        INS(0x13, inc_r16, de);
        INS(0x23, inc_r16, hl);
        
        // inc sp
        INS(0x33, inc_sp);

        // and a, r8
        INS(0xA7, and_a_r8, a);
        INS(0xA0, and_a_r8, b);
        INS(0xA1, and_a_r8, c);
        INS(0xA2, and_a_r8, d);
        INS(0xA3, and_a_r8, e);
        INS(0xA4, and_a_r8, h);
        INS(0xA5, and_a_r8, l);
        
        // and a, [hl]
        INS(0xA6, and_a_ind_hl);

        // and a, n8
        INS(0xE6, and_a_n8);

        // cpl
        INS(0x2F, cpl);

        // or a, r8
        INS(0xB7, or_a_r8, a);
        INS(0xB0, or_a_r8, b);
        INS(0xB1, or_a_r8, c);
        INS(0xB2, or_a_r8, d);
        INS(0xB3, or_a_r8, e);
        INS(0xB4, or_a_r8, h);
        INS(0xB5, or_a_r8, l);
        
        // or a, [hl]
        INS(0xB6, or_a_ind_hl);

        // or a, n8
        INS(0xF6, or_a_n8);

        // xor a, r8
        INS(0xAF, xor_a_r8, a); 
        INS(0xA8, xor_a_r8, b);
        INS(0xA9, xor_a_r8, c);
        INS(0xAA, xor_a_r8, d);
        INS(0xAB, xor_a_r8, e);
        INS(0xAC, xor_a_r8, h);
        INS(0xAD, xor_a_r8, l);
        
        // xor a, [hl]
        INS(0xAE, xor_a_ind_hl);

        // xor a, n8
        INS(0xEE, xor_a_n8);

        // rla
        INS(0x17, rla);

        // rlca
        INS(0x07, rlca);

        // rra
        INS(0x1F, rra);

        // rrca
        INS(0x0F, rrca);

        // call n16
        INS(0xCD, call_a16);

        // call cc, n16
        INS(0xC4, call_cc_a16, NZ);
        INS(0xCC, call_cc_a16, Z);
        INS(0xD4, call_cc_a16, NC);
        INS(0xDC, call_cc_a16, C);

        // jp hl
        INS(0xE9, jp_hl);

        // jp n16
        INS(0xC3, jp_a16);

        // jp cc, n16
        INS(0xC2, jp_cc_a16, NZ);
        INS(0xCA, jp_cc_a16, Z);
        INS(0xD2, jp_cc_a16, NC);
        INS(0xDA, jp_cc_a16, C);

        // jr n16
        INS(0x18, jr_a16);

        // jr cc, n16
        INS(0x20, jr_cc_a16, NZ);
        INS(0x28, jr_cc_a16, Z);
        INS(0x30, jr_cc_a16, NC);
        INS(0x38, jr_cc_a16, C);

        // ret
        INS(0xC9, ret);

        // ret cc
        INS(0xC0, ret_cc, NZ);
        INS(0xC8, ret_cc, Z);
        INS(0xD0, ret_cc, NC);
        INS(0xD8, ret_cc, C);

        // reti
        INS(0xD9, reti);

        // rst vec
        INS(0xC7, rst_vec, 0x00);
        INS(0xCF, rst_vec, 0x08);
        INS(0xD7, rst_vec, 0x10);
        INS(0xDF, rst_vec, 0x18);
        INS(0xE7, rst_vec, 0x20);
        INS(0xEF, rst_vec, 0x28);
        INS(0xF7, rst_vec, 0x30);
        INS(0xFF, rst_vec, 0x38);

        // ccf
        INS(0x3F, ccf);

        // scf
        INS(0x37, scf);

        // add sp, e8
        INS(0xE8, add_sp_e8);

        // ld hl, sp+e8
        INS(0xF8, ld_hl_sp_e8);

        // ld sp, hl
        INS(0xF9, ld_sp_hl);

        // pop af
        INS(0xF1, pop_af);

        // pop r16
        INS(0xC1, pop_r16, bc);
        INS(0xD1, pop_r16, de);
        INS(0xE1, pop_r16, hl);

        // push af
        INS(0xF5, push_af);

        // push r16
        INS(0xC5, push_r16, bc);
        INS(0xD5, push_r16, de);
        INS(0xE5, push_r16, hl);

        // di
        INS(0xF3, di);

        // ei
        INS(0xFB, ei);

        // halt
        INS(0x76, halt);

        // daa
        INS(0x27, daa);

        // nop
        INS(0x00, nop);

        // stop
        INS(0x10, stop);

        case 0xCB:
        case 0xD3: 
        case 0xDB: 
        case 0xDD:
        case 0xE3: 
        case 0xE4: 
        case 0xEB:
        case 0xEC: 
        case 0xED:
        case 0xF4: 
        case 0xFC:
        case 0xFD:
            throw std::runtime_error(std::format("Illegal opcode reached: %02X", opcode));
            break;

        default:
            throw std::runtime_error(std::format("Unimplemented opcode reached: %02X", opcode));
    }
}