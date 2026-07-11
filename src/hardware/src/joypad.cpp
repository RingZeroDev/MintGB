#include "joypad.hpp"

void Joypad::reset() {
    joyp = 0b11111111;
}

void Joypad::update() {
    bool action = joyp >> 4 & 1;
    bool direction = joyp >> 5 & 1;
    if (action & direction) {
        joyp = 0b11110000; // all buttons pressed
    } else if (action) {
        joyp |= constructInput(a, b, select, start);
    } else if (direction) {
        joyp |= constructInput(right, left, up, down);
    } else {
        joyp = 0b11111111; // no buttons pressed
    }
}

void Joypad::setInput(const JoypadInput& input) {
    right = input.right;
    left = input.left;
    up = input.up;
    down = input.down;

    a = input.a;
    b = input.b;
    select = input.select;
    start = input.start;

    update();
}

JoypadInput Joypad::getInput() {
    return JoypadInput {
        .right = right,
        .left = left,
        .up = up,
        .down = down,

        .a = a,
        .b = b,
        .select = select,
        .start = start,
     };
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
    joyp = value | 0b11000000;
    update();
}

