#pragma once

#include "interrupt.hpp"

#include <cstdint>

struct TimerState {
    uint16_t div = 0x0000;
    uint8_t tima = 0x00;
    uint8_t tma = 0x00;
    uint8_t tac = 0b11111000;
};

class Timer {
    private:
        InterruptSystem& interrupt;

        uint16_t div = 0x0000; // DIV: Divider register
        uint8_t tima = 0x00; // TIMA: Timer counter
        uint8_t tma = 0x00; // TMA: Timer modulo
        uint8_t tac = 0b11111000; // TAC: Timer control

        uint16_t frequency = 0;

    public:
        Timer(InterruptSystem& interrupt);

        void reset();
        void cycle();

        uint8_t readDivider();
        void resetDivider();

        uint8_t readCounter();
        void writeCounter(uint8_t value);

        uint8_t readModulo();
        void writeModulo(uint8_t value);

        uint8_t readControl();
        void writeControl(uint8_t value);

        TimerState getState() const;
        void setState(const TimerState& state);
};