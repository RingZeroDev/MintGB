#include "timer.hpp" 

#include <array>

Timer::Timer(InterruptSystem& interrupt) : interrupt(interrupt) {}

void Timer::reset() {
    div = 0x0000; 
    tima = 0x00; 
    tma = 0x00; 
    tac = 0b11111000;

    frequency = 0;
}

void Timer::cycle() {
    div += 4;

    bool counterEnable = tac >> 2 & 1;
    if (counterEnable && div % frequency == 0) {
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

constexpr std::array<uint16_t, 4> frequencies = {{ 1024, 16, 64, 256 }}; 

void Timer::writeControl(uint8_t value) {
    tac = value | 0b11111000;
    frequency = frequencies[tac & 0b11];
}

TimerState Timer::getState() const {
    return TimerState {
        .div = div,
        .tima = tima,
        .tma = tma,
        .tac = tac
    };
}

void Timer::setState(TimerState state) {
    div = state.div;
    tima = state.tima;
    tma = state.tma;
    tac = state.tac | 0b11111000;
}