#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "cpu/register.hpp"

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