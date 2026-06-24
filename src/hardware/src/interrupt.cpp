#include "interrupt.hpp"

uint8_t InterruptSystem::readIE() {
    return ie;
}

void InterruptSystem::writeIE(uint8_t value) {
    ie = value | 0b11100000;
}

uint8_t InterruptSystem::readIF() {
    return if_;
}

void InterruptSystem::writeIF(uint8_t value) {
    if_ = value | 0b11100000;
}

void InterruptSystem::issueInterrupt(InterruptSource source) {
    if_ |= static_cast<uint8_t>(source);
}