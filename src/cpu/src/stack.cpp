#include "sm83.hpp"

void SM83::push(uint16_t value) {
    cycle();
    sp--;
    writeByte(sp, value >> 8);
    sp--;
    writeByte(sp, value & 0xFF);
}

uint16_t SM83::pop() {
    uint16_t value = readWord(sp);
    sp += 2;
    return value;
}