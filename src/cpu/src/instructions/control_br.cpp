#include "sm83.hpp"

void SM83::call(uint16_t addr) {
    push(pc);
    pc = addr;
}

void SM83::jp_hl() {
    pc = hl;
}

void SM83::jp_a16() {
    pc = fetchWord();
    cycle();
}

void SM83::jp_cc_a16(bool cond) {
    uint16_t addr = fetchWord();
    if (cond) {
        pc = addr;
        cycle();
    }
}

void SM83::jr_a16() {
    pc += fetchRelative();
    cycle();
}

void SM83::jr_cc_a16(bool cond) {
    int8_t offset = fetchRelative();
    if (cond) {
        pc += offset; 
        cycle();
    }
}

void SM83::call_a16() {
    uint16_t addr = fetchWord();
    call(addr);
}

void SM83::call_cc_a16(bool cond) {
    uint16_t addr = fetchWord();
    if (cond) {
        call(addr);
    }
}

void SM83::ret() {
    pc = pop(); 
    cycle();
}

void SM83::ret_cc(bool cond) {
    cycle();
    if (cond) { 
        ret();
    }
}

void SM83::reti() {
    ret();
    ime = true;
}

void SM83::rst_vec(uint8_t vec) {
    call(vec);
}