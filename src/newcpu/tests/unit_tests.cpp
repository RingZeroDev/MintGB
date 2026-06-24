#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "cpu/cpu.hpp"

namespace MintGB {

class CPUTests {
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

    private:
    CPU cpu {};
};

METHOD_AS_TEST_CASE(CPUTests::registerPairComp, "Register Pair Composition", "[Register Pair]");
METHOD_AS_TEST_CASE(CPUTests::registerPairDecomp, "Register Pair Decomp", "[Register Pair]");

}