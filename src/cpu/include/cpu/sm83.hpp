#pragma once

#include "register.hpp"
#include "flags.hpp"
#include "bus.hpp"

#include <cstdint>
#include <array>

class SM83 {
    friend class SM83Tests;

    private:
        Bus& bus;

        uint8_t a = 0x00, f = 0x00, b = 0x00, c = 0x00, d = 0x00, e = 0x00, h = 0x00, l = 0x00;
        uint16_t pc = 0x0000, sp = 0x0000;

        bool ime = false, imePending = false, halted = false;

        RegisterPair af{ a, f };
        RegisterPair bc{ b, c };
        RegisterPair de{ d, e };
        RegisterPair hl{ h, l };

        Flags flags{f};

        uint8_t readByte(uint16_t addr);
        uint16_t readWord(uint16_t addr);

        void writeByte(uint16_t addr, uint8_t value);
        void writeWord(uint16_t addr, uint16_t value);

        uint8_t fetchByte();
        uint16_t fetchWord();
        int8_t fetchRelative();

        void decode(uint8_t opcode);

        uint8_t carry(uint8_t value, uint8_t res);
        uint16_t carry(uint16_t value, uint16_t res);
        uint8_t carry(uint16_t reg, int8_t offset, uint16_t res);

        void adc(uint8_t value);
        void add(uint8_t value);
        void add(uint16_t value);
        void add(int8_t value);
        void add(uint16_t reg, int8_t offset);
        void cp(uint8_t value);
        void dec(uint8_t& reg);
        void inc(uint8_t& reg);
        void sbc(uint8_t value);
        void sub(uint8_t value);
        void and_(uint8_t value);
        void cpl();
        void or_(uint8_t value);
        void xor_(uint8_t value);
        void rla();
        void rlca();
        void rra();
        void rrca();
        void ccf();
        void scf();
        void daa();
        void bit(uint8_t pos, uint8_t value);
        void res(uint8_t& value, uint8_t pos);
        void set(uint8_t& value, uint8_t pos);
        
        void call(uint16_t addr);
        void ret();
        void reti();

        template<uint8_t vec>
        void rst() {
            call(vec);
        }

        void push(uint16_t value);
        void pop(uint16_t& reg);

    public:
        explicit SM83(Bus& bus);
};