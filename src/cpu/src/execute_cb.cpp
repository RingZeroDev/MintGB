#include "sm83.hpp"

void SM83::executeCB(uint8_t opcode) {
    uint8_t value;
    uint8_t b3 = opcode >> 3 & 0b111;
    uint8_t r8 = opcode & 0b111;

    switch (r8) {
        case 0b000: value = b; break;
        case 0b001: value = c; break;
        case 0b010: value = d; break;
        case 0b011: value = e; break;
        case 0b100: value = h; break;
        case 0b101: value = l; break;
        case 0b110: value = readByte(hl); break;
        case 0b111: value = a; break;
    }


    switch (opcode >> 6 & 0b11) {
        case 0b00: {
            switch (b3) {
                // rlc r8
                case 0b000: 
                    rlc(value); 
                    zero = value == 0;
                    break;
                
                // rrc r8
                case 0b001: 
                    rrc(value); 
                    zero = value == 0;
                    break;

                // rl r8
                case 0b010: 
                    rl(value); 
                    zero = value == 0;
                    break;

                // rr r8
                case 0b011: 
                    rr(value); 
                    zero = value == 0;
                    break;

                // sla r8
                case 0b100: {
                    uint8_t msb = value >> 7;
                    value <<= 1;
                    carry = msb;
                    zero = value == 0;
                    subtract = false;
                    halfCarry = false;
                    break;
                }

                // sra r8
                case 0b101: {
                    uint8_t lsb = value & 1;
                    uint8_t msb = value >> 7;
                    value >>= 1;
                    value |= msb << 7;
                    carry = lsb;
                    zero = value == 0;
                    subtract = false;
                    halfCarry = false;
                    break;
                }

                // swap r8
                case 0b110: {
                    uint8_t lowNibble = value & 0x0F;
                    uint8_t highNibble = (value & 0xF0) >> 4;
                    value = (lowNibble << 4) | highNibble;
                    zero = value == 0;
                    subtract = false;
                    halfCarry = false;
                    carry = false;
                    break;
                }

                // srl r8
                case 0b111: {
                    uint8_t lsb = value & 1;
                    value >>= 1;
                    carry = lsb;
                    zero = value == 0;
                    subtract = false;
                    halfCarry = false;
                    break;
                } 
            }
            break;
        }

        // bit u3, r8
        case 0b01: zero = value >> b3 & 1; break;
        
        // res u3, r8
        case 0b10: value &= ~(1 << b3); break;
        
        // set u3, r8
        case 0b11: value |= (1 << b3); break;
    }

    switch (r8) {
        case 0b000: b = value; break;
        case 0b001: c = value; break;
        case 0b010: d = value; break;
        case 0b011: e = value; break;
        case 0b100: h = value; break;
        case 0b101: l = value; break;
        case 0b110: writeByte(hl, value); break;
        case 0b111: a = value; break;
    }
}