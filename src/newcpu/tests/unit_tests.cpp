#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "cpu/cpu.hpp"

namespace MintGB {

class CPU::Tests {
    public:
    void registerPairComp() {
        cpu.b = 0xAB;
        cpu.c = 0xCD;

        REQUIRE(cpu.bc() == 0xABCD);
    }
    
    void registerPairDecomp() {
        cpu.bc(0xABCD);

        REQUIRE(cpu.b == 0xAB);
        REQUIRE(cpu.c == 0xCD);
    }

    void afRegisterPairSet() {
        cpu.af(0xFFFF);

        REQUIRE(cpu.af() == 0xFFF0);
    }

    void cpuStateGet() {
        cpu.state(CPU::State {
            .af = 0xABCD,
            .bc = 0xABCD,
            .de = 0xABCD,
            .hl = 0xABCD,

            .pc = 0xABCD,
            .sp = 0xABCD
        });

        REQUIRE(cpu.af() == 0xABC0); // lower nibble gets cleared
        REQUIRE(cpu.bc() == 0xABCD);
        REQUIRE(cpu.de() == 0xABCD);
        REQUIRE(cpu.hl() == 0xABCD);

        REQUIRE(cpu.pc == 0xABCD);
        REQUIRE(cpu.sp == 0xABCD);
    }

    void cpuStateSet() {
        cpu.af(0xABCD);
        cpu.bc(0xABCD);
        cpu.de(0xABCD);
        cpu.hl(0xABCD);

        cpu.pc = 0xABCD;
        cpu.sp = 0xABCD;

        REQUIRE(cpu.state() == CPU::State {
            .af = 0xABC0, // lower nibble gets cleared
            .bc = 0xABCD,
            .de = 0xABCD,
            .hl = 0xABCD,
            
            .pc = 0xABCD,
            .sp = 0xABCD
        });
    }

    void defaultState() {
        REQUIRE(cpu.state() == CPU::State {
            .af = 0x0000,
            .bc = 0x0000,
            .de = 0x0000,
            .hl = 0x0000,

            .pc = 0x0000,
            .sp = 0x0000
        });
    }

    private:
    CPU cpu {};
};

METHOD_AS_TEST_CASE(CPU::Tests::registerPairComp, "Register Pair Composition", "[RegisterPair]");
METHOD_AS_TEST_CASE(CPU::Tests::registerPairDecomp, "Register Pair Decomp", "[RegisterPair]");
METHOD_AS_TEST_CASE(CPU::Tests::afRegisterPairSet, "Set AF Register Pair", "[RegisterPair]");
METHOD_AS_TEST_CASE(CPU::Tests::cpuStateGet, "Get CPU State", "[State]");
METHOD_AS_TEST_CASE(CPU::Tests::cpuStateSet, "Set CPU State", "[State]");
METHOD_AS_TEST_CASE(CPU::Tests::defaultState, "Check default CPU State", "[State]");

}