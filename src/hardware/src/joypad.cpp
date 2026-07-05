#include "joypad.hpp"

void Joypad::reset() {
    joyp = 0b11111111;
}

void Joypad::setInput(JoypadInput input) {
    bool action = joyp >> 4 & 1;
    bool direction = joyp >> 5 & 1;
    if (action & direction) {
        joyp = 0b11110001; // all buttons pressed
    } else if (action) {
        joyp |= constructInput(input.a, input.b, input.select, input.start);
    } else if (direction) {
        joyp |= constructInput(input.right, input.left, input.up, input.down);
    } else {
        joyp = 0b11111111; // no buttons pressed
    }
}

JoypadInput Joypad::getInput() {
    return JoypadInput { };
}

uint8_t Joypad::constructInput(bool inp0, bool inp1, bool inp2, bool inp3) {
    uint8_t inp = 0x00;
    inp |= !inp0;
    inp |= !inp1 << 1;
    inp |= !inp2 << 2;
    inp |= !inp3 << 3;
    return inp;
}

uint8_t Joypad::read() {
    return joyp;
}

void Joypad::write(uint8_t value) {
    joyp &= 0b11001111;
    joyp |= value & 0b00110000;
}

