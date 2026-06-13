#include "sm83.hpp"

void SM83::push(uint16_t value) {
    writeWord(sp-2, value);
    sp -= 2;
}

uint16_t SM83::pop() {
    uint16_t value = readWord(sp);
    sp += 2;
    return value;
}