#include "sm83.hpp"

void SM83::call(uint16_t addr) {
    push(pc);
    pc = addr;
}

void SM83::call(bool cond, uint16_t addr) {
    if (cond) call(addr);
}

void SM83::jp(bool cond, uint16_t addr) {
    if (cond) { pc = addr; cycle(); }
}

void SM83::jr(bool cond, int8_t offset) {
    if (cond) { pc += offset; cycle(); }
}

void SM83::ret(bool cond) {
    cycle();
    if (cond) { pc = pop(); cycle(); }
}

void SM83::stop() {
    // complicated behavior, just halt for now
    halted = true;
}
