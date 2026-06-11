#include "sm83.hpp"

void SM83::push(uint16_t value) {
    writeWord(sp-2, value);
    sp -= 2;
}

void SM83::pop(uint16_t& reg) {
    reg = readWord(sp-2);
    sp += 2;
}+
