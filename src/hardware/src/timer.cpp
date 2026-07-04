#include "timer.hpp" 

#include <array>

Timer::Timer(InterruptSystem& interrupt) : interrupt(interrupt) {}

void Timer::cycle() {
    div += 4;

    if ((tac >> 2 & 1) && div % (frequency * 4) == 0) {
        tima++;
        if (tima == 0) {
            tima = tma;
            interrupt.issueInterrupt(InterruptSource::Timer);
        }
    }
}

uint8_t Timer::readCounter() {
    return tima;
}

void Timer::writeCounter(uint8_t value) {
    tima = value;
}

void Timer::resetDivider() {
    div = 0;
}

uint8_t Timer::readDivider() {
    return div >> 8;
}

uint8_t Timer::readModulo() {
    return tma;
}

void Timer::writeModulo(uint8_t value) {
    tma = value;
}

uint8_t Timer::readControl() {
    return tac;
}

constexpr std::array<uint16_t, 4> frequencies = {{ 256, 4, 16, 64 }}; 

void Timer::writeControl(uint8_t value) {
    tac = value | 0b11111000;
    frequency = frequencies[tac & 0b11];
}