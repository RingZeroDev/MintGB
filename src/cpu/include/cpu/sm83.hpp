#pragma once

#include "register.hpp"
#include "flags.hpp"

#include <cstdint>
#include <array>

class SM83 {
    private:
        uint8_t a, f, b, c, d, e, h, l;
        uint16_t pc, sp;

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

    public:
        template<Register8 Reg8>
        uint8_t get() {
            if constexpr (Reg8 == Register8::IndirectHL) {
                return 0;
            } 
            return this->*REG_TABLE[static_cast<uint8_t>(Reg8)];
        }

        template<Register8 Reg8>
        void set(uint8_t value) {
            if constexpr (Reg8 == Register8::IndirectHL) {
                return;
            } 
            this->*REG_TABLE[static_cast<uint8_t>(Reg8)] = value;
        }

        template<Register16Stack Reg16>
        uint16_t get() {
            return this->*REGPAIR_TABLE[static_cast<uint8_t>(Reg16)];
        }

        template<Register16Stack Reg16>
        void set(uint16_t value) {
            this->*REGPAIR_TABLE[static_cast<uint8_t>(Reg16)] = value;
        }

        template<Register8 Reg1, Register8 Reg2>
        void ld() {
            uint8_t value = get<Reg2>();
            set<Reg1>(value);
        }

        template<Register16Stack Reg1, Register16Stack Reg2>
        void ld() {
            uint16_t value = get<Reg2>();
            set<Reg1>(value);
        }
};