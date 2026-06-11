#include "sm83.cpp"

void SM83::call(uint16_t addr) {
    push(pc + 1);
    pc = addr;
}

void SM83::ret() {
    pop(pc);
}

void SM83::reti() {
    ret();
    imePending = true;
}

