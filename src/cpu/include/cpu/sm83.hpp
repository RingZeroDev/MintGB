#include "register.hpp"

#include <cstdint>

class SM83 {
    private:
        uint8_t a, f, b, c, d, e, h, l;
        uint16_t pc, sp;

        RegisterPair af{ a, f };
        RegisterPair bc{ b, c };
        RegisterPair de{ d, e };
        RegisterPair hl{ h, l };
};