#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <iostream>

#include "cpu/sm83.hpp"
#include "cpu/register.hpp"
#include "cpu/flags.hpp"
#include "cpu/bus.hpp"

class SM83Tests {
    private:
        std::array<uint8_t, 65536> memory{};
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
            Flags flags{ f };

            REQUIRE(flags.get<Flag::Carry>() == true);
            REQUIRE(flags.get<Flag::HalfCarry>() == true);
            REQUIRE(flags.get<Flag::Subtraction>() == true);
            REQUIRE(flags.get<Flag::Zero>() == true);
        }

        void flagsComp() {
            uint8_t f = 0;
            Flags flags{ f };

            flags.set<Flag::Carry>(true);
            flags.set<Flag::HalfCarry>(true);
            flags.set<Flag::Subtraction>(true);
            flags.set<Flag::Zero>(true);

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

            cpu.flags.set<Flag::Carry>(true);
            cpu.adc(cpu.b);

            REQUIRE(cpu.a == 13);
        } 

        void adcFlags() {
            cpu.a = 0xFF;
            cpu.b = 0x01;

            cpu.flags.set<Flag::Subtraction>(true);
            cpu.adc(cpu.b);

            REQUIRE(cpu.flags.get<Flag::Carry>() == true);
            REQUIRE(cpu.flags.get<Flag::Zero>() == true);
            REQUIRE(cpu.flags.get<Flag::HalfCarry>() == true);
            REQUIRE(cpu.flags.get<Flag::Subtraction>() == false);
        }

        void addResult() {
            cpu.a = 5;
            cpu.b = 7;

            cpu.flags.set<Flag::Carry>(true);
            cpu.add(cpu.b);

            REQUIRE(cpu.a == 12);
        } 

        void addFlags() {
            cpu.a = 0xFF;
            cpu.b = 0x00;

            cpu.flags.set<Flag::Subtraction>(true);
            cpu.add(cpu.b);

            REQUIRE(cpu.flags.get<Flag::Carry>() == false);
            REQUIRE(cpu.flags.get<Flag::Zero>() == false);
            REQUIRE(cpu.flags.get<Flag::HalfCarry>() == false);
            REQUIRE(cpu.flags.get<Flag::Subtraction>() == false);
        }

        void addRegisterPairResult() {
            cpu.hl = 0xFFFF;
            cpu.bc = 0x0000;

            cpu.flags.set<Flag::Carry>(true);
            cpu.add(cpu.bc);

            REQUIRE(cpu.hl == 0xFFFF);
        }

        void addRegisterPairFlags() {
            cpu.hl = 0xFFFF;
            cpu.bc = 0x0001;

            cpu.add(cpu.bc);

            REQUIRE(cpu.flags.get<Flag::Carry>() == true);
            REQUIRE(cpu.flags.get<Flag::HalfCarry>() == true);
            REQUIRE(cpu.flags.get<Flag::Subtraction>() == false);
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

            REQUIRE(cpu.flags.get<Flag::Zero>() == false);
            REQUIRE(cpu.flags.get<Flag::Carry>() == true);
            REQUIRE(cpu.flags.get<Flag::HalfCarry>() == true);
            REQUIRE(cpu.flags.get<Flag::Subtraction>() == true);
        }

        void decResult() {
            cpu.a = 0x01;
            
            cpu.dec(cpu.a);

            REQUIRE(cpu.a == 0x00);
        }

        void decFlags() {
            cpu.a = 0x10;

            cpu.dec(cpu.a);

            REQUIRE(cpu.flags.get<Flag::Zero>() == false);
            REQUIRE(cpu.flags.get<Flag::Subtraction>() == true);
            REQUIRE(cpu.flags.get<Flag::HalfCarry>() == true);
            REQUIRE(cpu.flags.get<Flag::Carry>() == false);
        }

        void incResult() {
            cpu.a = 0x00;
            
            cpu.inc(cpu.a);

            REQUIRE(cpu.a == 0x01);
        }

        void incFlags() {
            cpu.a = 0x0F;

            cpu.inc(cpu.a);

            REQUIRE(cpu.flags.get<Flag::Zero>() == false);
            REQUIRE(cpu.flags.get<Flag::Subtraction>() == false);
            REQUIRE(cpu.flags.get<Flag::HalfCarry>() == true);
            REQUIRE(cpu.flags.get<Flag::Carry>() == false);
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