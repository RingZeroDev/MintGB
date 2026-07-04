#pragma once

#include <cstdint>

struct JoypadInput {
    bool right; bool left; bool up; bool down;
    bool a; bool b; bool select; bool start;
};

class Joypad {
    private:
        uint8_t joyp = 0b11111111; // P1/JOYP: Joypad

        static uint8_t constructInput(bool inp0, bool inp1, bool inp2, bool inp3);

    public:
        void setInput(JoypadInput input);
        JoypadInput getInput();

        uint8_t read();
        void write(uint8_t value);
};