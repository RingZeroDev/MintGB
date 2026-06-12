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
            cpu.decode(0x47);

            REQUIRE(cpu.b == 32);
        }

        void instruction_ld_r8_imm8() {
            memory[0] = 0xAB;

            cpu.decode(0x3E);

            REQUIRE(cpu.a == 0xAB);
        }

        void instruction_ld_r8_indirect_hl() {
            memory[0x100] = 0xAB;
            cpu.hl = 0x100;

            cpu.decode(0x7E);

            REQUIRE(cpu.a == 0xAB);
        }

        void instruction_ld_a_r16mem() {
            memory[0x100] = 0xAB;
            cpu.bc = 0x100;

            cpu.decode(0x0A);

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

        void addRelativeResult() {
            memory[0x00] = 0xFF;

            cpu.sp = 0x0001;

            cpu.add(cpu.fetchRelative());

            REQUIRE(cpu.sp == 0x0000);
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

        void cplResultAndFlags() {
            cpu.a = 0b10101010;

            cpu.cpl();

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

        void rlaResultAndFlags() {
            cpu.a = 0b00000001;
            cpu.carry = true;

            cpu.rla();

            REQUIRE(cpu.a == 0b00000011);
            REQUIRE_FALSE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
        }

        void rlcaResultAndFlags() {
            cpu.a = 0b10000000;

            cpu.rlca();

            REQUIRE(cpu.a == 0b00000001);
            REQUIRE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
        }

        void rraResultAndFlags() {
            cpu.a = 0b10000000;
            cpu.carry = true;

            cpu.rra();

            REQUIRE(cpu.a == 0b11000000);
            REQUIRE_FALSE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
            REQUIRE_FALSE(cpu.halfCarry);
        }

        void rrcaResultAndFlags() {
            cpu.a = 0b00000001;

            cpu.rrca();

            REQUIRE(cpu.a == 0b10000000);
            REQUIRE(cpu.carry);
            REQUIRE_FALSE(cpu.zero);
            REQUIRE_FALSE(cpu.subtract);
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
METHOD_AS_TEST_CASE(SM83Tests::addRelativeResult, "ADD Relative Result", "[Arithmetic][add]")
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
METHOD_AS_TEST_CASE(SM83Tests::cplResultAndFlags, "CPL Result and Flags", "[Logic][cpl]")
METHOD_AS_TEST_CASE(SM83Tests::orResult, "OR Result", "[Logic][or]")
METHOD_AS_TEST_CASE(SM83Tests::orFlags, "OR Flags", "[Logic][or]")
METHOD_AS_TEST_CASE(SM83Tests::xorResult, "XOR Result", "[Logic][xor]")
METHOD_AS_TEST_CASE(SM83Tests::xorFlags, "XOR Flags", "[Logic][xor]")
METHOD_AS_TEST_CASE(SM83Tests::rlaResultAndFlags, "RLA Result and Flags", "[Shift][rla]")
METHOD_AS_TEST_CASE(SM83Tests::rlcaResultAndFlags, "RLCA Result and Flags", "[Shift][rlca]")
METHOD_AS_TEST_CASE(SM83Tests::rraResultAndFlags, "RRA Result and Flags", "[Shift][rra]")
METHOD_AS_TEST_CASE(SM83Tests::rrcaResultAndFlags, "RRCA Result and Flags", "[Shift][rrca]")