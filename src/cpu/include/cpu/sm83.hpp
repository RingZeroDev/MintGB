#pragma once

#include "register.hpp"
#include "flags.hpp"

#include <cstdint>
#include <array>

template<typename Bus>
class SM83 {
    private:
        Bus& bus;

        uint8_t a = 0x00, f = 0x00, b = 0x00, c = 0x00, d = 0x00, e = 0x00, h = 0x00, l = 0x00;

        RegisterPair af{ a, f };
        RegisterPair bc{ b, c };
        RegisterPair de{ d, e };
        RegisterPair hl{ h, l };

        Flags flags{f};

        static constexpr std::array<uint8_t SM83::*, 8> REG_TABLE {
            &SM83::b,
            &SM83::c,
            &SM83::d,
            &SM83::e,
            &SM83::h,
            &SM83::l,
            nullptr,
            &SM83::a,
        };

        static constexpr std::array<RegisterPair SM83::*, 4> REGPAIR_TABLE {
            &SM83::bc,
            &SM83::de,
            &SM83::hl,
            &SM83::af
        };

        uint8_t readByte(uint16_t addr) {
            return bus.read(addr);
        }

        uint16_t readWord(uint16_t addr) {
            return bus.read(addr) | (bus.read(addr+1) << 8);
        }

        void writeByte(uint16_t addr, uint8_t value) {
            bus.write(addr, value);
        }

        void writeWord(uint16_t addr, uint16_t value) {
            bus.write(addr, value & 0xFF);
            bus.write(addr+1, value >> 8);
        }

    public:
        friend class SM83Tests;

        uint16_t pc = 0x0000, sp = 0x0000;

        SM83(Bus& bus) : bus(bus) {}

        #include "operand.tpp"
        #include "instruction.tpp"
};
