#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "cpu/sm83.hpp"
#include "cpu/register.hpp"
#include "cpu/flags.hpp"

TEST_CASE("Working RegisterPair decomposition", "[regpair_decomp]") {
    uint8_t low = 0;
    uint8_t high = 0;
    RegisterPair rp{ high, low };

    rp = 0xABCD;
    REQUIRE(low == 0xCD);
    REQUIRE(high == 0xAB);
}

TEST_CASE("Working RegisterPair composition", "[regpair_comp]") {
    uint8_t low = 0xCD;
    uint8_t high = 0xAB;
    RegisterPair rp{ high, low };

    REQUIRE(rp == 0xABCD);
}

TEST_CASE("Working flags decomposition", "[flags_decomp]") {
    uint8_t f = 0b11110000;
    Flags flags{ f };

    REQUIRE(flags.get<FlagBits::Carry>() == true);
    REQUIRE(flags.get<FlagBits::HalfCarry>() == true);
    REQUIRE(flags.get<FlagBits::Subtraction>() == true);
    REQUIRE(flags.get<FlagBits::Zero>() == true);
}

TEST_CASE("Working flags composition", "[flags_comp]") {
    uint8_t f = 0;
    Flags flags{ f };

    flags.set<FlagBits::Carry>(true);
    flags.set<FlagBits::HalfCarry>(true);
    flags.set<FlagBits::Subtraction>(true);
    flags.set<FlagBits::Zero>(true);

    REQUIRE(f == 0b11110000);
}

TEST_CASE("Register access", "[reg_access]") {
    SM83 cpu{};

    cpu.set<Register8::A>(32);
    
    REQUIRE(cpu.get<Register8::A>() == 32);
}

TEST_CASE("ld r8, r8", "[ld]") {
    SM83 cpu{};

    cpu.set<Register8::B>(32);
    cpu.ld<Register8::A, Register8::B>();

    REQUIRE(cpu.get<Register8::A>() == 32);
}