#include "joypad.hpp"

void Joypad::setInput(bool inp1, bool inp2, bool inp3, bool inp4) {
    joyp &= 0b11110000;
    joyp |= !inp1;
    joyp |= (!inp2 << 1);
    joyp |= (!inp3 << 2);
    joyp |= (!inp4 << 3);
}

uint8_t Joypad::readJOYP() {
    if (!(joyp >> 4 & 1)) setInput(right, left, up, down);
    if (!(joyp >> 5 & 1)) setInput(a, b, select, start);
    return joyp;
}

void Joypad::writeJOYP(uint8_t value) {
    joyp &= 0b11001111;
    joyp |= value & 0b00110000;
}

