#pragma once

#include <cstdint>

class Joypad {
    private:
        uint8_t joyp = 0b00110000; // P1/JOYP: Joypad

        void setInput(bool inp1, bool inp2, bool inp3, bool inp4);

    public:
        bool right = false, left = false, up = false, down = false;
        bool a = false, b = false, select = false, start = false;

        uint8_t readJOYP();
        void writeJOYP(uint8_t value);
};