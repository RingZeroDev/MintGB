#pragma once

#include <cstdint>

enum class InterruptSource : uint8_t {
    VBlank = 1 << 0,
    LCD = 1 << 1,
    Timer = 1 << 2,
    Serial = 1 << 3,
    Joypad = 1 << 4,
};

class InterruptSystem {
    private:
        uint8_t ie = 0b11100000; // IE: Interrupt Enable
        uint8_t if_ = 0b11100000; // IF: Interrupt Flag

    public:
        uint8_t readEnable();
        void writeEnable(uint8_t value);

        uint8_t readFlag();
        void writeFlag(uint8_t value);

        void issueInterrupt(InterruptSource source);
};
