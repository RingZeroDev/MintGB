#pragma once

#include "register.hpp"
#include "flags.hpp"
#include "bus.hpp"

#include <cstdint>
#include <array>

class CPU {
    friend class CPUTests;
    friend class SingleStepTests;

    protected:
        Bus& bus;

        uint8_t a = 0x00, f = 0x00, b = 0x00, c = 0x00, d = 0x00, e = 0x00, h = 0x00, l = 0x00;
        uint16_t pc = 0x0000, sp = 0x0000;

        bool ime = false, imePending = false, halted = false;

        RegisterPair af{ a, f };
        RegisterPair bc{ b, c };
        RegisterPair de{ d, e };
        RegisterPair hl{ h, l };

        Flag<7> zero{ f };
        Flag<6> subtract{ f };
        Flag<5> halfCarry{ f };
        Flag<4> carry{ f };

        uint8_t readByte(uint16_t addr);
        uint16_t readWord(uint16_t addr);

        void writeByte(uint16_t addr, uint8_t value);
        void writeWord(uint16_t addr, uint16_t value);

        void cycle();
        void cycle(int amount);

        uint8_t fetchByte();
        uint16_t fetchWord();
        int8_t fetchRelative();

        void decode(uint8_t opcode);
        void decodeCB(uint8_t opcode);
        void step();

        uint16_t carryCompare(uint8_t value, uint16_t res);
        uint32_t carryCompare(uint16_t value, uint32_t res);
        uint8_t carryCompare(uint16_t reg, int8_t offset, uint16_t res);

        // Load instructions

        void ld_r8_r8(uint8_t& r1, uint8_t& r2);
        void ld_r8_n8(uint8_t& r);
        void ld_r16_n16(RegisterPair& rp);
        void ld_ind_hl_r8(uint8_t& r);
        void ld_ind_hl_n8();
        void ld_r8_ind_hl(uint8_t& r);
        void ld_ind_r16_a(RegisterPair& rp);
        void ld_a16_a();
        void ldh_a16_a();
        void ldh_ind_c_a();
        void ld_a_ind_r16(RegisterPair& rp);
        void ld_a_a16();
        void ldh_a_a16();
        void ldh_a_ind_c();
        void ld_ind_hli_a();
        void ld_ind_hld_a();
        void ld_a_ind_hli();
        void ld_a_ind_hld();

        // 8-bit arithmetic instructions

        void adc(uint8_t value);
        void add(uint8_t value);
        void add(uint16_t value);
        void cp(uint8_t value);
        void dec(uint8_t& reg);
        void inc(uint8_t& reg);
        void sbc(uint8_t value);
        void sub(uint8_t value);

        void adc_a_r8(uint8_t &r);
        void adc_a_ind_hl();
        void adc_a_n8();
        void add_a_r8(uint8_t &r);
        void add_a_ind_hl();
        void add_a_n8();
        void cp_a_r8(uint8_t &r);
        void cp_a_ind_hl();
        void cp_a_n8();
        void dec_r8(uint8_t &r);
        void dec_ind_hl();
        void inc_r8(uint8_t &r);
        void inc_ind_hl();
        void sbc_a_r8(uint8_t &r);
        void sbc_a_ind_hl();
        void sbc_a_n8();
        void sub_a_r8(uint8_t &r);
        void sub_a_ind_hl();
        void sub_a_n8();

        // 16-bit arithmetic instructions

        void add_hl_r16(RegisterPair &rp);
        void dec_r16(RegisterPair &rp);
        void inc_r16(RegisterPair &rp);

        // Bitwise logic instructions

        void and_(uint8_t value);
        void or_(uint8_t value);
        void xor_(uint8_t value);

        void and_a_r8(uint8_t &r);
        void and_a_ind_hl();
        void and_a_n8();
        void cpl();
        void or_a_r8(uint8_t &r);
        void or_a_ind_hl();
        void or_a_n8();
        void xor_a_r8(uint8_t &r);
        void xor_a_ind_hl();
        void xor_a_n8();

        // Bit flag instructions

        void bit(uint8_t u3, uint8_t& value);
        void res(uint8_t u3, uint8_t& value);
        void set(uint8_t u3, uint8_t& value);

        void bit_u3_r8(uint8_t u3, uint8_t& r);
        void bit_u3_ind_hl(uint8_t u3);
        void res_u3_r8(uint8_t u3, uint8_t& r);
        void res_u3_ind_hl(uint8_t u3);
        void set_u3_r8(uint8_t u3, uint8_t& r);
        void set_u3_ind_hl(uint8_t u3);

        // Bit shift instructions

        void rl(uint8_t &value);
        void rlc(uint8_t& value);
        void rr(uint8_t& value);
        void rrc(uint8_t& value);
        void sla(uint8_t& value);
        void sra(uint8_t& value);
        void srl(uint8_t& value);
        void swap(uint8_t& value);

        void rl_r8(uint8_t& r);
        void rl_ind_hl();
        void rla();
        void rlc_r8(uint8_t& r);
        void rlc_ind_hl();
        void rlca();
        void rr_r8(uint8_t& r);
        void rr_ind_hl();
        void rra();
        void rrc_r8(uint8_t& r);
        void rrc_ind_hl();
        void rrca();
        void sla_r8(uint8_t& r);
        void sla_ind_hl();
        void sra_r8(uint8_t& r);
        void sra_ind_hl();
        void srl_r8(uint8_t& r);
        void srl_ind_hl();
        void swap_r8(uint8_t& r);
        void swap_ind_hl();
    
        // Jumps and subroutine instructions

        void call(uint16_t addr);
        void ret();

        void call_a16();
        void call_cc_a16(bool cond);
        void jp_hl();
        void jp_a16();
        void jp_cc_a16(bool cond);
        void jr_a16();
        void jr_cc_a16(bool cond);
        void ret_cc(bool cond);
        void ret(bool cond);
        void reti();
        void rst_vec(uint8_t vec);

        // Carry flag instructions

        void ccf();
        void scf();

        // Stack manipulation instructions

        uint16_t pop();
        void push(uint16_t value);

        void add_hl_sp();
        void add_sp_e8();
        void dec_sp();
        void inc_sp();
        void ld_sp_n16();
        void ld_a16_sp();
        void ld_hl_sp_e8();
        void ld_sp_hl();
        void pop_af();
        void pop_r16(RegisterPair &rp);
        void push_af();
        void push_r16(RegisterPair &rp);

        // Interrupt-related instructions

        void di();
        void ei();
        void halt();

        // Miscellaneous instructions

        void daa();
        void nop();
        void stop();

    public:
        explicit CPU(Bus& bus);
};