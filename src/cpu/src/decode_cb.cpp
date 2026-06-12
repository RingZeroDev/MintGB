#include "sm83.hpp"

void SM83::decodeCB(uint8_t opcode) {
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
                case 0b000: rlc(value); break;
                case 0b001: rrc(value); break;
                case 0b010: rl(value); break;
                case 0b011: rr(value); break;
                case 0b100: sla(value); break;
                case 0b101: sra(value); break;
                case 0b110: swap(value); break;
                case 0b111: srl(value); break;
            }
            break;
        }

        case 0b01: bit(b3, value); break;
        case 0b10: res(b3, value); break;
        case 0b11: set(b3, value); break;
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