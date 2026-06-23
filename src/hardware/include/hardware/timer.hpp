#pragma once

#include <cstdint>

class Timer {
    private:
        uint16_t div; // DIV: Divider register
        uint8_t tima; // TIMA: Timer counter
        uint8_t tma; // TMA: Timer modulo
        uint8_t tac; // TAC: Timer control

        uint8_t frequency;

    public:
        void cycle();

        uint8_t readDIV();
        void resetDIV();

        uint8_t readTIMA();
        void writeTIMA(uint8_t value);

        uint8_t readTMA();
        void writeTMA(uint8_t value);

        uint8_t readTAC();
        void writeTAC(uint8_t value);
};