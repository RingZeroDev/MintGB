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

        SM83<MemoryBus> cpu { bus };

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

            REQUIRE(flags.get<FlagBits::Carry>() == true);
            REQUIRE(flags.get<FlagBits::HalfCarry>() == true);
            REQUIRE(flags.get<FlagBits::Subtraction>() == true);
            REQUIRE(flags.get<FlagBits::Zero>() == true);
        }

        void flagsComp() {
            uint8_t f = 0;
            Flags flags{ f };

            flags.set<FlagBits::Carry>(true);
            flags.set<FlagBits::HalfCarry>(true);
            flags.set<FlagBits::Subtraction>(true);
            flags.set<FlagBits::Zero>(true);

            REQUIRE(f == 0b11110000);
        }

        void registerAccess() {
            cpu.set<Register8::A>(32);
    
            REQUIRE(cpu.get<Register8::A>() == 32);
        }

        void registerPairAccess() {
            cpu.set<Register16::HL>(0xABCD);

            REQUIRE(cpu.get<Register16::HL>() == 0xABCD);
        }

        void instruction_ld_r8_r8() {
            cpu.b = 32;
            cpu.ld<Register8::A, Register8::B>();

            REQUIRE(cpu.a == 32);
        }

        void instruction_ld_r8_imm8() {
            memory[0] = 0xAB;

            cpu.ld<Register8::A, Extended::Immediate8>();

            REQUIRE(cpu.a == 0xAB);
        }

        void instruction_ld_r8_indirect_hl() {
            memory[0x100] = 0xAB;
            cpu.hl = 0x100;

            cpu.ld<Register8::A, Register8::IndirectHL>();
            REQUIRE(cpu.a == 0xAB);
        }
};


METHOD_AS_TEST_CASE(SM83Tests::registerPairDecomp, "Register Pair Decomposition", "[RegisterPair][Decomposition]")
METHOD_AS_TEST_CASE(SM83Tests::registerPairComp, "Register Pair Composition", "[RegisterPair][Composition]")
METHOD_AS_TEST_CASE(SM83Tests::registerPairPostfix, "Register Pair Postfix Operators", "[RegisterPair]")
METHOD_AS_TEST_CASE(SM83Tests::flagsDecomp, "Flags Decomposition", "[Flags][Decomposition]")
METHOD_AS_TEST_CASE(SM83Tests::flagsComp, "Flags Composition", "[Flags][Composition]")
METHOD_AS_TEST_CASE(SM83Tests::registerAccess, "Register Access", "[Register][Access]")
METHOD_AS_TEST_CASE(SM83Tests::registerPairAccess, "Register Pair Access", "[RegisterPair][Access]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_r8_r8, "Instruction: ld r8, r8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_r8_imm8, "Instruction: ld r8, imm8", "[Instruction][ld]")
METHOD_AS_TEST_CASE(SM83Tests::instruction_ld_r8_indirect_hl, "Instruction: ld r8, [hl]", "[Instruction][ld]")