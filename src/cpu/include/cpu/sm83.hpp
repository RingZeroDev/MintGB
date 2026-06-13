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

        Flag<7> zero{ f };
        Flag<6> subtract{ f };
        Flag<5> halfCarry{ f };
        Flag<4> carry{ f };

        uint8_t readByte(uint16_t addr);
        uint16_t readWord(uint16_t addr);

        void writeByte(uint16_t addr, uint8_t value);
        void writeWord(uint16_t addr, uint16_t value);

        uint8_t fetchByte();
        uint16_t fetchWord();
        int8_t fetchRelative();

        void execute(uint8_t opcode);
        void executeCB(uint8_t opcode);

        uint8_t carryCompare(uint8_t value, uint8_t res);
        uint16_t carryCompare(uint16_t value, uint16_t res);
        uint8_t carryCompare(uint16_t reg, int8_t offset, uint16_t res);

        void adc(uint8_t value);
        void add(uint8_t value);
        void add(uint16_t value);
        void cp(uint8_t value);
        void dec(uint8_t& reg);
        void inc(uint8_t& reg);
        void sbc(uint8_t value);
        void sub(uint8_t value);
        void and_(uint8_t value);
        void or_(uint8_t value);
        void xor_(uint8_t value);
        void rl(uint8_t& value);
        void rlc(uint8_t& value);
        void rr(uint8_t& value);
        void rrc(uint8_t& value);
        
        void call(uint16_t addr);
        void call(bool cond, uint16_t addr);
        void jp(bool cond, uint16_t addr);
        void jr(bool cond, int8_t offset);
        void ret(bool cond);
        void stop();

        void push(uint16_t value);
        uint16_t pop();

    public:
        explicit SM83(Bus& bus);
};