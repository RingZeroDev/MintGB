#include "interrupt.hpp"

void InterruptSystem::reset() {
    ie = 0b11100000;
    if_ = 0b11100000;
}

uint8_t InterruptSystem::readEnable() {
    return ie;
}

void InterruptSystem::writeEnable(uint8_t value) {
    ie = value | 0b11100000;
}

uint8_t InterruptSystem::readFlag() {
    return if_;
}

void InterruptSystem::writeFlag(uint8_t value) {
    if_ = value | 0b11100000;
}

void InterruptSystem::issueInterrupt(InterruptSource source) {
    if_ |= static_cast<uint8_t>(source);
    if (callback != nullptr) {
        callback(source);
    }
}

void InterruptSystem::setCallback(InterruptCallback newCallback) {
    callback = newCallback;
}