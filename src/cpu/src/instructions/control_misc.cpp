#include "sm83.hpp"

void SM83::di() {
    ime = false;
}

void SM83::ei() {
    imePending = true;
}

void SM83::halt() {
    halted = true; 
    cycle(2);
}

void SM83::nop() {
    return;
}

void SM83::stop() {
    halted = true; 
    cycle(2);
}