#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <iostream>

#include "cpu/sm83.hpp"
#include "cpu/register.hpp"
#include "cpu/flags.hpp"
#include "cpu/bus.hpp"

class SM83Tests {
    private:
        std::array<uint8_t, 0x10000> memory{};
        MemoryBus bus { memory };

        SM83 cpu { bus };

    public:
        SM83Tests() {
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
            cpu.a = 32;
            cpu.execute(0x47);

            REQUIRE(cpu.b == 32);
        }

        void instruction_ld_r8_imm8() {
            memory[0] = 0xAB;

            cpu.execute(0x3E);

            REQUIRE(cpu.a == 0xAB);
        }

        void instruction_ld_r8_indirect_hl() {
            memory[0x100] = 0xAB;
            cpu.hl = 0x100;

            cpu.execute(0x7E);

            REQUIRE(cpu.a == 0xAB);
        }

        void instruction_ld_a_r16mem() {
            memory[0x100] = 0xAB;
            cpu.bc = 0x100;

            cpu.execute(0x0A);

            REQUIRE(cpu.a == 0xAB);
        }

        void adcResult() {
            cpu.a = 5;
            cpu.b = 7;
            cpu.carry = true;

            cpu.adc(cpu.b);

            REQUIRE(cpu.a == 13);
        } 

        void adcFlags() {
            cpu.a = 0xFF;
            cpu.b = 0x01;
            cpu.subtract = true;

            cpu.adc(cpu.b);

            REQUIRE(cpu.carry);
            REQUIRE(cpu.zero);
            REQUIRE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.subtract);
        }

        void addResult() {
            cpu.a = 5;
            cpu.b = 7;
            cpu.carry = true;

            cpu.add(cpu.b);

            REQUIRE(cpu.a == 12);
        } 

        void addFlags() {
            cpu.a = 0xFF;
            cpu.b = 0x00;
            cpu.subtract = true;

            cpu.add(cpu.b);

            REQUIRE_FALSE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.subtract);
        }

        void addRegisterPairResult() {
            cpu.hl = 0xFFFF;
            cpu.bc = 0x0000;
            cpu.carry = true;

            cpu.add(cpu.bc);

            REQUIRE(cpu.hl == 0xFFFF);
        }

        void addRegisterPairFlags() {
            cpu.hl = 0xFFFF;
            cpu.bc = 0x0001;

            cpu.add(cpu.bc);

            REQUIRE(cpu.carry);
            REQUIRE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.subtract);
        }

        void instruction_add_sp_e8() {
            memory[0x00] = 0xFF;
            cpu.sp = 0x0008;

            cpu.execute(0xE8);

            REQUIRE(cpu.sp == 0x0007);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE(cpu.halfCarry);
            REQUIRE(cpu.carry);
        }

        void instruction_ld_hl_sp_e8() {
            memory[0x00] = 0xFF;
            cpu.sp = 0x0008;

            cpu.execute(0xF8);

            REQUIRE(cpu.hl == 0x0007);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE(cpu.halfCarry);
            REQUIRE(cpu.carry);
        }

        void cpFlags() {
            cpu.a = 0x00;
            cpu.b = 0x01;

            cpu.cp(cpu.b);

            REQUIRE_FALSE(cpu.zero);
            REQUIRE(cpu.carry);
            REQUIRE(cpu.halfCarry);
            REQUIRE(cpu.subtract);
        }

        void decResult() {
            cpu.a = 0x01;
            
            cpu.dec(cpu.a);

            REQUIRE(cpu.a == 0x00);
        }

        void decFlags() {
            cpu.a = 0x10;

            cpu.dec(cpu.a);

            REQUIRE_FALSE(cpu.zero);
            REQUIRE(cpu.subtract);
            REQUIRE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.carry);
        }

        void incResult() {
            cpu.a = 0x00;
            
            cpu.inc(cpu.a);

            REQUIRE(cpu.a == 0x01);
        }

        void incFlags() {
            cpu.a = 0x0F;

            cpu.inc(cpu.a);

            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.carry);
        }

        void sbcResult() {
            cpu.a = 0xAB;
            cpu.b = 0x03;
            cpu.carry = true;

            cpu.sbc(cpu.b);

            REQUIRE(cpu.a == 0xA7);
        }

        void sbcFlags() {
            cpu.a = 0x42;
            cpu.b = 0x42;
            cpu.carry = true;

            cpu.sbc(cpu.b);

            REQUIRE_FALSE(cpu.zero);
            REQUIRE(cpu.subtract);
            REQUIRE(cpu.halfCarry);
            REQUIRE(cpu.carry);
        }

        void subResult() {
            cpu.a = 0xAB;
            cpu.b = 0x03;
            cpu.carry = true;

            cpu.sub(cpu.b);

            REQUIRE(cpu.a == 0xA8);
        }

        void subFlags() {
            cpu.a = 0x10;
            cpu.b = 0x01;

            cpu.sub(cpu.b);

            REQUIRE_FALSE(cpu.zero);
            REQUIRE(cpu.subtract);
            REQUIRE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.carry);
        }

        void andResult() {
            cpu.a = 0b11110000;
            cpu.b = 0b00110000;

            cpu.and_(cpu.b);

            REQUIRE(cpu.a == 0b00110000);
        }

        void andFlags() {
            cpu.a = 0b11110000;
            cpu.b = 0b00001111;

            cpu.and_(cpu.b);

            REQUIRE(cpu.zero);
            REQUIRE(cpu.halfCarry);
        }

        void instruction_cpl() {
            cpu.a = 0b10101010;

            cpu.execute(0x2F);

            REQUIRE(cpu.a == 0b01010101);
            REQUIRE(cpu.subtract);
            REQUIRE(cpu.subtract);
        }
        
        void orResult() {
            cpu.a = 0b10101010;
            cpu.b = 0b01010101;

            cpu.or_(cpu.b);

            REQUIRE(cpu.a == 0b11111111);
        }

        void orFlags() {
            cpu.a = 0;
            cpu.b = 0;

            cpu.or_(cpu.b);

            REQUIRE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.carry);
        }

        void xorResult() {
            cpu.a = 0b00110000;
            cpu.b = 0b01010000;

            cpu.xor_(cpu.b);

            REQUIRE(cpu.a == 0b01100000);
        }

        void xorFlags() {
            cpu.a = 0;
            cpu.b = 0;

            cpu.xor_(cpu.b);

            REQUIRE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.carry);
        }

        void rlResultAndFlags() {
            cpu.a = 0b00000001;
            cpu.carry = true;

            cpu.rl(cpu.a);

            REQUIRE(cpu.a == 0b00000011);
            REQUIRE_FALSE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
        }

        void rlcResultAndFlags() {
            cpu.a = 0b10000000;

            cpu.rlc(cpu.a);

            REQUIRE(cpu.a == 0b00000001);
            REQUIRE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
        }

        void rrResultAndFlags() {
            cpu.a = 0b10000000;
            cpu.carry = true;

            cpu.rr(cpu.a);

            REQUIRE(cpu.a == 0b11000000);
            REQUIRE_FALSE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
        }

        void rrcResultAndFlags() {
            cpu.a = 0b00000001;

            cpu.rrc(cpu.a);

            REQUIRE(cpu.a == 0b10000000);
            REQUIRE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
        }

        void instruction_sla_r8() {
            cpu.a = 0b10000000;

            cpu.executeCB(0x27);

            REQUIRE(cpu.a == 0);
            REQUIRE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE(cpu.carry);
        }

        void instruction_sra_r8() {
            cpu.a = 0b10000001;

            cpu.executeCB(0x2F);

            REQUIRE(cpu.a == 0b11000000);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE(cpu.carry);
        }

        void instruction_srl_r8() {
            cpu.a = 0b00000001;

            cpu.executeCB(0x3F);

            REQUIRE(cpu.a == 0);
            REQUIRE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE(cpu.carry);
        }

        void instruction_swap_r8() {
            cpu.a = 0b11110000;
            cpu.zero = true;
            
            cpu.executeCB(0x37);
            
            REQUIRE(cpu.a == 0b00001111);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.carry);
        }

        void popRegistersAndStack() {
            cpu.sp = 0x0100;
            memory[0x0100] = 0xCD;
            memory[0x0101] = 0xAB;

            uint16_t value = cpu.pop();

            REQUIRE(value == 0xABCD);
            REQUIRE(cpu.sp == 0x102);
        }

        void instruction_pop_af() {
            cpu.sp = 0x0100;
            memory[0x0100] = 0b11111111;
            memory[0x0101] = 0xAB;

            cpu.execute(0xF1);

            REQUIRE(cpu.f == 0b11110000);
        }

        void pushRegistersAndStack() {
            cpu.bc = 0xABCD;
            cpu.sp = 0x0100;

            cpu.push(cpu.bc);

            REQUIRE(cpu.sp == 0x00FE);
            REQUIRE(memory[0x00FF] == 0xAB);
            REQUIRE(memory[0x00FE] == 0xCD);
        }

        void callRegistersAndStack() {
            cpu.pc = 0xDCBB;
            cpu.sp = 0x0100;
            memory[0xDCBB] = 0xCD;
            memory[0xDCBC] = 0xAB;

            cpu.call(cpu.fetchWord());

            REQUIRE(cpu.pc == 0xABCD);
            REQUIRE(cpu.sp == 0x00FE);
            REQUIRE(memory[0xFE] == 0xBD);
            REQUIRE(memory[0xFF] == 0xDC);
        }

        void retRegistersAndStack() {
            cpu.pc = 0xABCD;
            cpu.sp = 0x0100;
            memory[0x0100] = 0x11;
            memory[0x0101] = 0xBA;

            cpu.pc = cpu.pop();

            REQUIRE(cpu.pc == 0xBA11);
            REQUIRE(cpu.sp == 0x0102);
        }

        void instruction_bit_u3_r8() {
            cpu.a = 0b00100000;
            cpu.zero = false;

            cpu.executeCB(0x77);

            REQUIRE(cpu.zero);
        }

        void instruction_res_u3_r8() {
            cpu.a = 0b01000000;

            cpu.executeCB(0xB7);

            REQUIRE(cpu.a == 0);
        }

        void instruction_set_u3_r8() {
            cpu.a = 0;

            cpu.executeCB(0xF7);

            REQUIRE(cpu.a == 0b01000000);
        }

        void instruction_daa() {
            cpu.a = 0x12;
            cpu.halfCarry = true;

            cpu.execute(0x27);

            REQUIRE(cpu.a == 0x18);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.halfCarry);
            REQUIRE_FALSE(cpu.carry);

            cpu.a = 0x33;
            cpu.subtract = true;

            cpu.execute(0x27);

            REQUIRE(cpu.a == 0x33);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE(cpu.subtract);
            REQUIRE_FALSE(cpu.carry);
            REQUIRE_FALSE(cpu.halfCarry);
        }
};


METHOD_AS_TEST_CASE(SM83Tests::registerPairDecomp, "Register Pair Decomposition", "[RegisterPair][Decomposition]")
METHOD_AS_TEST_CASE(SM83Tests::registerPairComp, "Register Pair Composition", "[RegisterPair][Composition]")
METHOD_AS_TEST_CASE(SM83Tests::registerPairPostfix, "Register Pair Postfix Operators", "[RegisterPair]")
METHOD_AS_TEST_CASE(SM83Tests::flagsDecomp, "Flags Decomposition", "[Flags][Decomposition]")
METHOD_AS_TEST_CASE(SM83Tests::flagsComp, "Flags Composition", "[Flags][Composition]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_r8_r8, "Instruction: ld r8, r8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_r8_imm8, "Instruction: ld r8, imm8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_r8_indirect_hl, "Instruction: ld r8, [hl]", "[Instruction][ld]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_a_r16mem, "Instruction: ld a, [r16mem]", "[Instruction][ld]")
METHOD_AS_TEST_CASE(SM83Tests::adcResult, "ADC Correct Result", "[Arithmetic][adc]")
METHOD_AS_TEST_CASE(SM83Tests::adcFlags, "ADC Correct Flags", "[Arithmetic][adc]")
METHOD_AS_TEST_CASE(SM83Tests::addResult, "ADD Correct Result", "[Arithmetic][add]")
METHOD_AS_TEST_CASE(SM83Tests::addFlags, "ADD Correct Flags", "[Arithmetic][add]")
METHOD_AS_TEST_CASE(SM83Tests::addRegisterPairResult, "ADD Register Pair Result", "[Arithmetic][add][RegisterPair]")
METHOD_AS_TEST_CASE(SM83Tests::addRegisterPairFlags, "ADD Register Pair Flags", "[Arithmetic][add][RegisterPair]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_add_sp_e8, "Instruction: add sp, e8", "[Instruction][add]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_hl_sp_e8, "Instruction: ld hl, sp+e8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(SM83Tests::cpFlags, "CP Flags", "[Arithmetic][cp]")
METHOD_AS_TEST_CASE(SM83Tests::decResult, "DEC Result", "[Arithmetic][dec]")
METHOD_AS_TEST_CASE(SM83Tests::decFlags, "DEC Flags", "[Arithmetic][dec]")
METHOD_AS_TEST_CASE(SM83Tests::incResult, "INC Result", "[Arithmetic][inc]")
METHOD_AS_TEST_CASE(SM83Tests::incFlags, "INC Flags", "[Arithmetic][inc]")
METHOD_AS_TEST_CASE(SM83Tests::sbcResult, "SBC Result", "[Arithmetic][sbc]")
METHOD_AS_TEST_CASE(SM83Tests::sbcFlags, "SBC Flags", "[Arithmetic][sbc]")
METHOD_AS_TEST_CASE(SM83Tests::subResult, "SUB Result", "[Arithmetic][sub]")
METHOD_AS_TEST_CASE(SM83Tests::subFlags, "SUB Flags", "[Arithmetic][sub]")
METHOD_AS_TEST_CASE(SM83Tests::andResult, "AND Result", "[Logic][and]")
METHOD_AS_TEST_CASE(SM83Tests::andFlags, "AND Flags", "[Logic][and]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_cpl, "Instruction cpl", "[Instruction][cpl]")
METHOD_AS_TEST_CASE(SM83Tests::orResult, "OR Result", "[Logic][or]")
METHOD_AS_TEST_CASE(SM83Tests::orFlags, "OR Flags", "[Logic][or]")
METHOD_AS_TEST_CASE(SM83Tests::xorResult, "XOR Result", "[Logic][xor]")
METHOD_AS_TEST_CASE(SM83Tests::xorFlags, "XOR Flags", "[Logic][xor]")
METHOD_AS_TEST_CASE(SM83Tests::rlResultAndFlags, "RL Result and Flags", "[Shift][rl]")
METHOD_AS_TEST_CASE(SM83Tests::rlcResultAndFlags, "RLC Result and Flags", "[Shift][rlc]")
METHOD_AS_TEST_CASE(SM83Tests::rrResultAndFlags, "RR Result and Flags", "[Shift][rr]")
METHOD_AS_TEST_CASE(SM83Tests::rrcResultAndFlags, "RRC Result and Flags", "[Shift][rrc]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_sla_r8, "Instruction: sla r8", "[Instruction][sla]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_sra_r8, "Instruction: sra r8", "[Instruction][sra]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_srl_r8, "Instruction: srl r8", "[Instruction][srl]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_swap_r8, "Instruction: swap r8", "[Instruction][swap]")
METHOD_AS_TEST_CASE(SM83Tests::popRegistersAndStack, "POP Registers and Stack", "[Stack][pop]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_pop_af, "Instruction: pop af", "[Instruction][pop]")
METHOD_AS_TEST_CASE(SM83Tests::pushRegistersAndStack, "PUSH Registers and Stack", "[Stack][push]")
METHOD_AS_TEST_CASE(SM83Tests::callRegistersAndStack, "CALL Registers and Stack", "[Control][call]")
METHOD_AS_TEST_CASE(SM83Tests::retRegistersAndStack, "RET Registers and Stack", "[Control][ret]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_bit_u3_r8, "Instruction: bit u3, r8", "[Instruction][bit]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_res_u3_r8, "Instruction: res u3, r8", "[Instruction][res]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_set_u3_r8, "Instruction: set u3, r8", "[Instruction][set]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_daa, "Instruction: daa", "[Instruction][daa]")