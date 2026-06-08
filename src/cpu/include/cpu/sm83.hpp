#include "register.hpp"
#include "flags.hpp"

#include <cstdint>

class SM83 {
    private:
        uint8_t a, f, b, c, d, e, h, l;
        uint16_t pc, sp;

        RegisterPair af{ a, f };
        RegisterPair bc{ b, c };
        RegisterPair de{ d, e };
        RegisterPair hl{ h, l };

        Flags flags{f};

    public:
        template<Register8 Reg8>
        uint8_t get() {
            if constexpr (Reg8 == Register8::B) {
                return b; 
            } 
            if constexpr (Reg8 == Register8::C) {
                return c;
            }
            if constexpr (Reg8 == Register8::D) {
                return d;
            }
            if constexpr (Reg8 == Register8::E) {
                return e;
            }
            if constexpr (Reg8 == Register8::H) {
                return h;
            } 
            if constexpr (Reg8 == Register8::L) {
                return l;
            }
            if constexpr (Reg8 == Register8::IndirectHL) {
                return 0;
            }
            if constexpr (Reg8 == Register8::A) {
                return a;
            }
        }

        template<Register8 Reg8>
        void set(uint8_t value) {
            if constexpr (Reg8 == Register8::B) {
                b = value;
            } 
            if constexpr (Reg8 == Register8::C) {
                c = value;
            }
            if constexpr (Reg8 == Register8::D) {
                d = value;
            }
            if constexpr (Reg8 == Register8::E) {
                e = value;
            }
            if constexpr (Reg8 == Register8::H) {
                h = value;
            } 
            if constexpr (Reg8 == Register8::L) {
                l = value;
            }
            if constexpr (Reg8 == Register8::IndirectHL) {
                return;
            }
            if constexpr (Reg8 == Register8::A) {
                a = value;
            }
        }

        template<Register8 Reg1, Register8 Reg2>
        void ld() {
            uint8_t value = get<Reg2>();
            set<Reg1>(value);
        }
};