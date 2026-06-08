#pragma once

#include <cstdint>

enum class Register8 : uint8_t {
    B = 0,
    C = 1,
    D = 2,
    E = 3,
    H = 4,
    L = 5,
    IndirectHL = 6,
    A = 7
};

enum class Register16 : uint8_t {
    BC = 0,
    DE = 1,
    HL = 2,
    SP = 3 
};

enum class Register16Stack {
    BC = 0,
    DE = 1,
    HL = 2,
    AF = 3
};

enum class Register16Mem {
    BC = 0,
    DE = 1,
    HLI = 2,
    HLD = 3
};

class RegisterPair {
    private:
        uint8_t& low;
        uint8_t& high;

    public:
        RegisterPair(uint8_t& high, uint8_t& low);

        operator uint16_t() const;
        RegisterPair& operator=(uint16_t value);
};