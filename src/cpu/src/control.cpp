#include "sm83.hpp"

void SM83::call(uint16_t addr) {
    push(pc + 1);
    pc = addr;
}

void SM83::call(bool cond, uint16_t addr) {
    if (cond) call(addr);
}

void SM83::jp(bool cond, uint16_t addr) {
    if (cond) pc = addr;
}

void SM83::jr(bool cond, int8_t offset) {
    if (cond) pc = offset;
}

void SM83::ret(bool cond) {
    if (cond) pc = pop();
}

void SM83::reti() {
    pc = pop();
    imePending = true;
}

void SM83::stop() {
    // complicated behavior, just halt for now
    halted = true;
}
