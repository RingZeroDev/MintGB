#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <iostream>

#include "cpu/cpu.hpp"
#include "cpu/register.hpp"
#include "cpu/flags.hpp"
#include "cpu/bus.hpp"

class CPUTests : private CPU {
    private:
        std::array<uint8_t, 0x10000> memory{};
        MemoryBus bus { memory };

    public:
        CPUTests() : CPU(bus) {
            bus.accesses.push_back(BusAccess { 0x0000, 0x00, AccessType::None });
        }

        void registerPairDecomp() {
            uint8_t low = 0;
            uint8_t high = 0;
            RegisterPair rp{ high, low };

            rp = 0xABCD;
            REQUIRE(low == 0xCD);
            REQUIRE(high == 0xAB);
        }

        void registerPairComp() {
            uint8_t low = 0xCD;
            uint8_t high = 0xAB;
            RegisterPair rp{ high, low };

            REQUIRE(rp == 0xABCD);
        }

        void registerPairPostfix() {
            uint8_t low = 0xFF;
            uint8_t high = 0x00;
            RegisterPair rp{ high, low };

            REQUIRE(rp++ == 0x00FF);
            REQUIRE(rp == 0x0100);
            REQUIRE(rp-- == 0x0100);
            REQUIRE(rp == 0x00FF);
        }

        void flagsDecomp() {
            uint8_t f = 0b11110000;
            
            Flag<7> zero{ f };
            Flag<6> subtract{ f };
            Flag<5> halfCarry{ f };
            Flag<6> carry{ f };

            REQUIRE(carry);
            REQUIRE(halfCarry);
            REQUIRE(subtract);
            REQUIRE(zero);
        }

        void flagsComp() {
            uint8_t f = 0;

            Flag<7> zero{ f };
            Flag<6> subtract{ f };
            Flag<5> halfCarry{ f };
            Flag<4> carry{ f };

            zero = true;
            subtract = true;
            halfCarry = true;
            carry = true;

            REQUIRE(f == 0b11110000);
        }

        void instruction_ld_r8_r8() {
            a = 32;
            decode(0x47);

            REQUIRE(b == 32);
        }

        void instruction_ld_r8_imm8() {
            memory[0] = 0xAB;

            decode(0x3E);

            REQUIRE(a == 0xAB);
        }

        void instruction_ld_r8_indirect_hl() {
            memory[0x100] = 0xAB;
            hl = 0x100;

            decode(0x7E);

            REQUIRE(a == 0xAB);
        }

        void instruction_ld_a_r16mem() {
            memory[0x100] = 0xAB;
            bc = 0x100;

            decode(0x0A);

            REQUIRE(a == 0xAB);
        }

        void adcResult() {
            a = 5;
            b = 7;
            carry = true;

            adc(b);

            REQUIRE(a == 13);
        } 

        void adcFlags() {
            a = 0xFF;
            b = 0x01;
            subtract = true;

            adc(b);

            REQUIRE(carry);
            REQUIRE(zero);
            REQUIRE(halfCarry);
            REQUIRE_FALSE(subtract);
        }

        void addResult() {
            a = 5;
            b = 7;
            carry = true;

            add(b);

            REQUIRE(a == 12);
        } 

        void addFlags() {
            a = 0xFF;
            b = 0x00;
            subtract = true;

            add(b);

            REQUIRE_FALSE(carry);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(halfCarry);
            REQUIRE_FALSE(subtract);
        }

        void addRegisterPairResult() {
            hl = 0xFFFF;
            bc = 0x0000;
            carry = true;

            add(bc);

            REQUIRE(hl == 0xFFFF);
        }

        void addRegisterPairFlags() {
            hl = 0xFFFF;
            bc = 0x0001;

            add(bc);

            REQUIRE(carry);
            REQUIRE(halfCarry);
            REQUIRE_FALSE(subtract);
        }

        void instruction_add_sp_e8() {
            memory[0x00] = 0xFF;
            sp = 0x0008;

            decode(0xE8);

            REQUIRE(sp == 0x0007);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE(halfCarry);
            REQUIRE(carry);
        }

        void instruction_ld_hl_sp_e8() {
            memory[0x00] = 0xFF;
            sp = 0x0008;

            decode(0xF8);

            REQUIRE(hl == 0x0007);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE(halfCarry);
            REQUIRE(carry);
        }

        void cpFlags() {
            a = 0x00;
            b = 0x01;

            cp(b);

            REQUIRE_FALSE(zero);
            REQUIRE(carry);
            REQUIRE(halfCarry);
            REQUIRE(subtract);
        }

        void decResult() {
            a = 0x01;
            
            dec(a);

            REQUIRE(a == 0x00);
        }

        void decFlags() {
            a = 0x10;

            dec(a);

            REQUIRE_FALSE(zero);
            REQUIRE(subtract);
            REQUIRE(halfCarry);
            REQUIRE_FALSE(carry);
        }

        void incResult() {
            a = 0x00;
            
            inc(a);

            REQUIRE(a == 0x01);
        }

        void incFlags() {
            a = 0x0F;

            inc(a);

            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE(halfCarry);
            REQUIRE_FALSE(carry);
        }

        void sbcResult() {
            a = 0xAB;
            b = 0x03;
            carry = true;

            sbc(b);

            REQUIRE(a == 0xA7);
        }

        void sbcFlags() {
            a = 0x42;
            b = 0x42;
            carry = true;

            sbc(b);

            REQUIRE_FALSE(zero);
            REQUIRE(subtract);
            REQUIRE(halfCarry);
            REQUIRE(carry);
        }

        void subResult() {
            a = 0xAB;
            b = 0x03;
            carry = true;

            sub(b);

            REQUIRE(a == 0xA8);
        }

        void subFlags() {
            a = 0x10;
            b = 0x01;

            sub(b);

            REQUIRE_FALSE(zero);
            REQUIRE(subtract);
            REQUIRE(halfCarry);
            REQUIRE_FALSE(carry);
        }

        void andResult() {
            a = 0b11110000;
            b = 0b00110000;

            and_(b);

            REQUIRE(a == 0b00110000);
        }

        void andFlags() {
            a = 0b11110000;
            b = 0b00001111;

            and_(b);

            REQUIRE(zero);
            REQUIRE(halfCarry);
        }

        void instruction_cpl() {
            a = 0b10101010;

            decode(0x2F);

            REQUIRE(a == 0b01010101);
            REQUIRE(subtract);
            REQUIRE(subtract);
        }
        
        void orResult() {
            a = 0b10101010;
            b = 0b01010101;

            or_(b);

            REQUIRE(a == 0b11111111);
        }

        void orFlags() {
            a = 0;
            b = 0;

            or_(b);

            REQUIRE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
            REQUIRE_FALSE(carry);
        }

        void xorResult() {
            a = 0b00110000;
            b = 0b01010000;

            xor_(b);

            REQUIRE(a == 0b01100000);
        }

        void xorFlags() {
            a = 0;
            b = 0;

            xor_(b);

            REQUIRE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
            REQUIRE_FALSE(carry);
        }

        void rlResultAndFlags() {
            a = 0b00000001;
            carry = true;

            rl(a);

            REQUIRE(a == 0b00000011);
            REQUIRE_FALSE(carry);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
        }

        void rlcResultAndFlags() {
            a = 0b10000000;

            rlc(a);

            REQUIRE(a == 0b00000001);
            REQUIRE(carry);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
        }

        void rrResultAndFlags() {
            a = 0b10000000;
            carry = true;

            rr(a);

            REQUIRE(a == 0b11000000);
            REQUIRE_FALSE(carry);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
        }

        void rrcResultAndFlags() {
            a = 0b00000001;

            rrc(a);

            REQUIRE(a == 0b10000000);
            REQUIRE(carry);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
        }

        void instruction_sla_r8() {
            a = 0b10000000;

            decodeCB(0x27);

            REQUIRE(a == 0);
            REQUIRE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
            REQUIRE(carry);
        }

        void instruction_sra_r8() {
            a = 0b10000001;

            decodeCB(0x2F);

            REQUIRE(a == 0b11000000);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
            REQUIRE(carry);
        }

        void instruction_srl_r8() {
            a = 0b00000001;

            decodeCB(0x3F);

            REQUIRE(a == 0);
            REQUIRE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
            REQUIRE(carry);
        }

        void instruction_swap_r8() {
            a = 0b11110000;
            zero = true;
            
            decodeCB(0x37);
            
            REQUIRE(a == 0b00001111);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(subtract);
            REQUIRE_FALSE(halfCarry);
            REQUIRE_FALSE(carry);
        }

        void popRegistersAndStack() {
            sp = 0x0100;
            memory[0x0100] = 0xCD;
            memory[0x0101] = 0xAB;

            uint16_t value = pop();

            REQUIRE(value == 0xABCD);
            REQUIRE(sp == 0x102);
        }

        void instruction_pop_af() {
            sp = 0x0100;
            memory[0x0100] = 0b11111111;
            memory[0x0101] = 0xAB;

            decode(0xF1);

            REQUIRE(f == 0b11110000);
        }

        void pushRegistersAndStack() {
            bc = 0xABCD;
            sp = 0x0100;

            push(bc);

            REQUIRE(sp == 0x00FE);
            REQUIRE(memory[0x00FF] == 0xAB);
            REQUIRE(memory[0x00FE] == 0xCD);
        }

        void callRegistersAndStack() {
            pc = 0xDCBB;
            sp = 0x0100;
            memory[0xDCBB] = 0xCD;
            memory[0xDCBC] = 0xAB;

            call(fetchWord());

            REQUIRE(pc == 0xABCD);
            REQUIRE(sp == 0x00FE);
            REQUIRE(memory[0xFE] == 0xBD);
            REQUIRE(memory[0xFF] == 0xDC);
        }

        void retRegistersAndStack() {
            pc = 0xABCD;
            sp = 0x0100;
            memory[0x0100] = 0x11;
            memory[0x0101] = 0xBA;

            pc = pop();

            REQUIRE(pc == 0xBA11);
            REQUIRE(sp == 0x0102);
        }

        void instruction_bit_u3_r8() {
            a = 0b00100000;
            zero = false;

            decodeCB(0x77);

            REQUIRE(zero);
        }

        void instruction_res_u3_r8() {
            a = 0b01000000;

            decodeCB(0xB7);

            REQUIRE(a == 0);
        }

        void instruction_set_u3_r8() {
            a = 0;

            decodeCB(0xF7);

            REQUIRE(a == 0b01000000);
        }

        void instruction_daa() {
            a = 0x12;
            halfCarry = true;

            decode(0x27);

            REQUIRE(a == 0x18);
            REQUIRE_FALSE(zero);
            REQUIRE_FALSE(halfCarry);
            REQUIRE_FALSE(carry);

            a = 0x33;
            subtract = true;

            decode(0x27);

            REQUIRE(a == 0x33);
            REQUIRE_FALSE(zero);
            REQUIRE(subtract);
            REQUIRE_FALSE(carry);
            REQUIRE_FALSE(halfCarry);
        }
};


METHOD_AS_TEST_CASE(CPUTests::registerPairDecomp, "Register Pair Decomposition", "[RegisterPair][Decomposition]")
METHOD_AS_TEST_CASE(CPUTests::registerPairComp, "Register Pair Composition", "[RegisterPair][Composition]")
METHOD_AS_TEST_CASE(CPUTests::registerPairPostfix, "Register Pair Postfix Operators", "[RegisterPair]")
METHOD_AS_TEST_CASE(CPUTests::flagsDecomp, "Flags Decomposition", "[Flags][Decomposition]")
METHOD_AS_TEST_CASE(CPUTests::flagsComp, "Flags Composition", "[Flags][Composition]")
METHOD_AS_TEST_CASE(CPUTests::instruction_ld_r8_r8, "Instruction: ld r8, r8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(CPUTests::instruction_ld_r8_imm8, "Instruction: ld r8, imm8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(CPUTests::instruction_ld_r8_indirect_hl, "Instruction: ld r8, [hl]", "[Instruction][ld]")
METHOD_AS_TEST_CASE(CPUTests::instruction_ld_a_r16mem, "Instruction: ld a, [r16mem]", "[Instruction][ld]")
METHOD_AS_TEST_CASE(CPUTests::adcResult, "ADC Correct Result", "[Arithmetic][adc]")
METHOD_AS_TEST_CASE(CPUTests::adcFlags, "ADC Correct Flags", "[Arithmetic][adc]")
METHOD_AS_TEST_CASE(CPUTests::addResult, "ADD Correct Result", "[Arithmetic][add]")
METHOD_AS_TEST_CASE(CPUTests::addFlags, "ADD Correct Flags", "[Arithmetic][add]")
METHOD_AS_TEST_CASE(CPUTests::addRegisterPairResult, "ADD Register Pair Result", "[Arithmetic][add][RegisterPair]")
METHOD_AS_TEST_CASE(CPUTests::addRegisterPairFlags, "ADD Register Pair Flags", "[Arithmetic][add][RegisterPair]")
METHOD_AS_TEST_CASE(CPUTests::instruction_add_sp_e8, "Instruction: add sp, e8", "[Instruction][add]")
METHOD_AS_TEST_CASE(CPUTests::instruction_ld_hl_sp_e8, "Instruction: ld hl, sp+e8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(CPUTests::cpFlags, "CP Flags", "[Arithmetic][cp]")
METHOD_AS_TEST_CASE(CPUTests::decResult, "DEC Result", "[Arithmetic][dec]")
METHOD_AS_TEST_CASE(CPUTests::decFlags, "DEC Flags", "[Arithmetic][dec]")
METHOD_AS_TEST_CASE(CPUTests::incResult, "INC Result", "[Arithmetic][inc]")
METHOD_AS_TEST_CASE(CPUTests::incFlags, "INC Flags", "[Arithmetic][inc]")
METHOD_AS_TEST_CASE(CPUTests::sbcResult, "SBC Result", "[Arithmetic][sbc]")
METHOD_AS_TEST_CASE(CPUTests::sbcFlags, "SBC Flags", "[Arithmetic][sbc]")
METHOD_AS_TEST_CASE(CPUTests::subResult, "SUB Result", "[Arithmetic][sub]")
METHOD_AS_TEST_CASE(CPUTests::subFlags, "SUB Flags", "[Arithmetic][sub]")
METHOD_AS_TEST_CASE(CPUTests::andResult, "AND Result", "[Logic][and]")
METHOD_AS_TEST_CASE(CPUTests::andFlags, "AND Flags", "[Logic][and]")
METHOD_AS_TEST_CASE(CPUTests::instruction_cpl, "Instruction cpl", "[Instruction][cpl]")
METHOD_AS_TEST_CASE(CPUTests::orResult, "OR Result", "[Logic][or]")
METHOD_AS_TEST_CASE(CPUTests::orFlags, "OR Flags", "[Logic][or]")
METHOD_AS_TEST_CASE(CPUTests::xorResult, "XOR Result", "[Logic][xor]")
METHOD_AS_TEST_CASE(CPUTests::xorFlags, "XOR Flags", "[Logic][xor]")
METHOD_AS_TEST_CASE(CPUTests::rlResultAndFlags, "RL Result and Flags", "[Shift][rl]")
METHOD_AS_TEST_CASE(CPUTests::rlcResultAndFlags, "RLC Result and Flags", "[Shift][rlc]")
METHOD_AS_TEST_CASE(CPUTests::rrResultAndFlags, "RR Result and Flags", "[Shift][rr]")
METHOD_AS_TEST_CASE(CPUTests::rrcResultAndFlags, "RRC Result and Flags", "[Shift][rrc]")
METHOD_AS_TEST_CASE(CPUTests::instruction_sla_r8, "Instruction: sla r8", "[Instruction][sla]")
METHOD_AS_TEST_CASE(CPUTests::instruction_sra_r8, "Instruction: sra r8", "[Instruction][sra]")
METHOD_AS_TEST_CASE(CPUTests::instruction_srl_r8, "Instruction: srl r8", "[Instruction][srl]")
METHOD_AS_TEST_CASE(CPUTests::instruction_swap_r8, "Instruction: swap r8", "[Instruction][swap]")
METHOD_AS_TEST_CASE(CPUTests::popRegistersAndStack, "POP Registers and Stack", "[Stack][pop]")
METHOD_AS_TEST_CASE(CPUTests::instruction_pop_af, "Instruction: pop af", "[Instruction][pop]")
METHOD_AS_TEST_CASE(CPUTests::pushRegistersAndStack, "PUSH Registers and Stack", "[Stack][push]")
METHOD_AS_TEST_CASE(CPUTests::callRegistersAndStack, "CALL Registers and Stack", "[Control][call]")
METHOD_AS_TEST_CASE(CPUTests::retRegistersAndStack, "RET Registers and Stack", "[Control][ret]")
METHOD_AS_TEST_CASE(CPUTests::instruction_bit_u3_r8, "Instruction: bit u3, r8", "[Instruction][bit]")
METHOD_AS_TEST_CASE(CPUTests::instruction_res_u3_r8, "Instruction: res u3, r8", "[Instruction][res]")
METHOD_AS_TEST_CASE(CPUTests::instruction_set_u3_r8, "Instruction: set u3, r8", "[Instruction][set]")
METHOD_AS_TEST_CASE(CPUTests::instruction_daa, "Instruction: daa", "[Instruction][daa]")