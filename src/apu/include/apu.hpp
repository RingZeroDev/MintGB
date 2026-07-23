#pragma once

#include <cstdint>
#include <array>

class APU {
    private:
        static constexpr int sampleCount = 512
        float samples[sampleCount];

        uint8_t nr50 = 0x00; // NR50: Master volume & VIN panning
        uint8_t nr51 = 0x00; // NR51: Sound panning
        uint8_t nr52 = 0x00; // NR52: Audio master control
        
        uint8_t nr10 = 0x00; // Channel 1 sweep
        uint8_t nr30 = 0x00; // Channel 3 DAC enable

        std::array<uint8_t, 4> nrx1 {}; // Length timers
        std::array<uint8_t, 4> nrx2 {}; // Volume and envelope
        std::array<uint8_t, 4> nrx3 {}; // Period
        std::array<uint8_t, 4> nrx4 {}; // Trigger and length timer enable bits

        std::array<uint8_t, 16> wave {}; // Wave pattern RAM
        
    public:
        uint8_t readMasterVolume();
        void writeMasterVolume();

        uint8_t readPanning();
        void writePanning(uint8_t value);

        uint8_t readMasterControl();
        void writeMasterControl(uint8_t value);

        uint8_t readSweep();
        void writeSweep(uint8_t value);

        uint8_t readDAC();
        void writeDAC(uint8_t value);

        uint8_t readLength(uint8_t channel);
        void writeLength(uint8_t channel, uint8_t value);

        uint8_t readVolume(uint8_t channel);
        void writeVolume(uint8_t channel, uint8_t value);

        uint8_t readPeriod(uint8_t channel);
        void writePeriod(uint8_t channel, uint8_t value);

        uint8_t readTrigger(uint8_t channel);
        void writeTrigger(uint8_t channel, uint8_t value);

        uint8_t readWave(uint8_t addr);
        void writeWave(uint8_t addr, uint16_t value);
};