#pragma once

#include "interrupt.hpp"

#include <cstdint>

class Timer {
    private:
        InterruptSystem& interrupt;

        uint16_t div; // DIV: Divider register
        uint8_t tima; // TIMA: Timer counter
        uint8_t tma; // TMA: Timer modulo
        uint8_t tac; // TAC: Timer control

        uint16_t frequency;

    public:
        Timer(InterruptSystem& interrupt);

        void cycle();

        uint8_t readDivider();
        void resetDivider();

        uint8_t readCounter();
        void writeCounter(uint8_t value);

        uint8_t readModulo();
        void writeModulo(uint8_t value);

        uint8_t readControl();
        void writeControl(uint8_t value);
};