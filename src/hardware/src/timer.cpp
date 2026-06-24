#include "timer.hpp" 

#include <array>

void Timer::cycle() {
    div += 4;

    if ((tac >> 2 & 1) && div % (frequency * 4) == 0) {
        tima++;
        if (tima == 0) tima = tma;
    }
}

uint8_t Timer::readTIMA() {
    return tima;
}

void Timer::writeTIMA(uint8_t value) {
    tima = value;
}

uint8_t Timer::readDIV() {
    return div >> 8;
}

void Timer::resetDIV() {
    div = 0;
}

uint8_t Timer::readTMA() {
    return tma;
}

void Timer::writeTMA(uint8_t value) {
    tma = value;
}

uint8_t Timer::readTAC() {
    return tac;
}

constexpr std::array<uint16_t, 4> frequencies = {{ 256, 4, 16, 64 }}; 

void Timer::writeTAC(uint8_t value) {
    tac = value | 0b11111000;
    frequency = frequencies[tac & 0b11];
}