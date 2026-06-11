#include "sm83.hpp"

void SM83::decode(uint8_t opcode) {
    switch (opcode) {
        // ld r8, r8
        case 0x7F: a = a; break;
        case 0x78: a = b; break;
        case 0x79: a = c; break;
        case 0x7A: a = d; break;
        case 0x7B: a = e; break;
        case 0x7C: a = h; break;
        case 0x7D: a = l; break;

        case 0x47: b = a; break;
        case 0x40: b = b; break;
        case 0x41: b = c; break;
        case 0x42: b = d; break;
        case 0x43: b = e; break;
        case 0x44: b = h; break;
        case 0x45: b = l; break;

        case 0x4F: c = a; break;
        case 0x48: c = b; break;
        case 0x49: c = c; break;
        case 0x4A: c = d; break;
        case 0x4B: c = e; break;
        case 0x4C: c = h; break;
        case 0x4D: c = l; break;

        case 0x57: d = a; break;
        case 0x50: d = b; break;
        case 0x51: d = c; break;
        case 0x52: d = d; break;
        case 0x53: d = e; break;
        case 0x54: d = h; break;
        case 0x55: d = l; break;

        case 0x5F: e = a; break;
        case 0x58: e = b; break;
        case 0x59: e = c; break;
        case 0x5A: e = d; break;
        case 0x5B: e = e; break;
        case 0x5C: e = h; break;
        case 0x5D: e = l; break;

        case 0x67: h = a; break;
        case 0x60: h = b; break;
        case 0x61: h = c; break;
        case 0x62: h = d; break;
        case 0x63: h = e; break;
        case 0x64: h = h; break;
        case 0x65: h = l; break;

        case 0x6F: l = a; break;
        case 0x68: l = b; break;
        case 0x69: l = c; break;
        case 0x6A: l = d; break;
        case 0x6B: l = e; break;
        case 0x6C: l = h; break;
        case 0x6D: l = l; break;

        // ld r8, n8
        case 0x3E: a = fetchByte(); break;
        case 0x06: b = fetchByte(); break;
        case 0x0E: c = fetchByte(); break;
        case 0x16: d = fetchByte(); break;
        case 0x1E: e = fetchByte(); break;
        case 0x26: h = fetchByte(); break;
        case 0x2E: l = fetchByte(); break;

        // ld r16, n16
        case 0x01: bc = fetchWord(); break;
        case 0x11: de = fetchWord(); break;
        case 0x21: hl = fetchWord(); break;
        case 0x31: sp = fetchWord(); break;

        // ld [HL], r8
        case 0x77: writeByte(hl, a); break;
        case 0x70: writeByte(hl, b); break;
        case 0x71: writeByte(hl, c); break;
        case 0x72: writeByte(hl, d); break;
        case 0x73: writeByte(hl, e); break;
        case 0x74: writeByte(hl, h); break;
        case 0x75: writeByte(hl, l); break;

        // ld [hl], n8
        case 0x36: writeByte(hl, fetchByte()); break;

        // ld r8, [hl]
        case 0x7E: a = readByte(hl); break;
        case 0x46: b = readByte(hl); break;
        case 0x4E: c = readByte(hl); break;
        case 0x56: d = readByte(hl); break;
        case 0x5E: e = readByte(hl); break;
        case 0x66: h = readByte(hl); break;
        case 0x6E: l = readByte(hl); break;

        // ld [r16], a
        case 0x02: writeByte(bc, a); break;
        case 0x12: writeByte(de, a); break;
        
        // ld [n16], a
        case 0xEA: writeByte(fetchWord(), a); break;

        // ldh [n16], a
        case 0xE0: writeByte(0xFF00 + fetchByte(), a); break;

        // ldh [c], a
        case 0xE2: writeByte(0xFF00 + c, a); break;

        // ld a, [r16]
        case 0x0A: a = readByte(bc); break;
        case 0x1A: a = readByte(de); break;

        // ld a, [n16]
        case 0xFA: a = readByte(fetchWord()); break;

        // ldh a, [n16]
        case 0xF0: a = readByte(0xFF00 + fetchByte()); break;

        // ldh a, [c]
        case 0xF2: a = readByte(0xFF00 + c); break;

        // ld [hl+], a
        case 0x22: writeByte(hl++, a); break;
        
        // ld [hl-], a
        case 0x32: writeByte(hl--, a); break;

        // ld a, [hl+]
        case 0x2A: a = readByte(hl++); break;
        
        // ld a, [hl-]
        case 0x3A: a = readByte(hl--); break;

        // adc a, r8
        case 0x8F: adc(a); break;
        case 0x88: adc(b); break;
        case 0x89: adc(c); break;
        case 0x8A: adc(d); break;
        case 0x8B: adc(e); break;
        case 0x8C: adc(h); break;
        case 0x8D: adc(l); break;

        // adc a, [hl]
        case 0x8E: adc(readByte(hl)); break;

        // adc a, n8
        case 0xCE: adc(fetchByte()); break;

        // add a, r8
        case 0x87: add(a); break;
        case 0x80: add(b); break;
        case 0x81: add(c); break;
        case 0x82: add(d); break;
        case 0x83: add(e); break;
        case 0x84: add(h); break;
        case 0x85: add(l); break;

        // add a, [hl]
        case 0x86: add(readByte(hl)); break;

        // add a, n8
        case 0xC6: add(fetchByte()); break;

        // cp a, r8
        case 0xBF: cp(a); break;
        case 0xB8: cp(b); break;
        case 0xB9: cp(c); break;
        case 0xBA: cp(d); break;
        case 0xBB: cp(e); break;
        case 0xBC: cp(h); break;
        case 0xBD: cp(l); break;

        // dec r8
        case 0x3D: dec(a); break;
        case 0x05: dec(b); break;
        case 0x0D: dec(c); break;
        case 0x15: dec(d); break;
        case 0x1D: dec(e); break;
        case 0x25: dec(h); break;
        case 0x2D: dec(l); break;

        // dec [hl]
        case 0x35: {
            uint8_t value = readByte(hl);
            dec(value);
            writeByte(hl, value);
            break;
        }

        // inc r8
        case 0x3C: inc(a); break;
        case 0x04: inc(b); break;
        case 0x0C: inc(c); break;
        case 0x14: inc(d); break;
        case 0x1C: inc(e); break;
        case 0x24: inc(h); break;
        case 0x2C: inc(l); break;

        // inc [hl]
        case 0x34: {
            uint8_t value = readByte(hl);
            inc(value);
            writeByte(hl, value);
            break;
        }

        // add hl, r16
        case 0x09: add(bc); break;
        case 0x19: add(de); break;
        case 0x29: add(hl); break;
        case 0x39: add(sp); break;

        // sub a, r8
    }
}