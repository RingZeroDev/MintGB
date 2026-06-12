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

        // cp a, n8
        case 0xFE: cp(fetchByte()); break;

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

        // sbc a, r8
        case 0x9F: sbc(a); break;
        case 0x98: sbc(b); break;
        case 0x99: sbc(c); break;
        case 0x9A: sbc(d); break;
        case 0x9B: sbc(e); break;
        case 0x9C: sbc(h); break;
        case 0x9D: sbc(l); break;

        // sbc a, [hl]
        case 0x9E: sbc(readByte(hl)); break;

        // sbc a, n8
        case 0xDE: sbc(fetchByte()); break;

        // sub a, r8
        case 0x97: sub(a); break;
        case 0x90: sub(b); break;
        case 0x91: sub(c); break;
        case 0x92: sub(d); break;
        case 0x93: sub(e); break;
        case 0x94: sub(h); break;
        case 0x95: sub(l); break;
        
        // sub a, [hl]
        case 0x96: sub(readByte(hl)); break;

        // sub a, n8
        case 0xD6: sub(fetchByte()); break;

        // add hl, r16
        case 0x09: add(bc); break;
        case 0x19: add(de); break;
        case 0x29: add(hl); break;
        case 0x39: add(sp); break;

        // dec r16
        case 0x0B: bc--; break;
        case 0x1B: de--; break;
        case 0x2B: hl--; break;
        case 0x3B: sp--; break;

        // inc r16
        case 0x03: bc++; break;
        case 0x13: de++; break;
        case 0x23: hl++; break;
        case 0x33: sp++; break;

        // and a, r8
        case 0xA7: and_(a); break;
        case 0xA0: and_(b); break;
        case 0xA1: and_(c); break;
        case 0xA2: and_(d); break;
        case 0xA3: and_(e); break;
        case 0xA4: and_(h); break;
        case 0xA5: and_(l); break;
        
        // and a, [hl]
        case 0xA6: and_(readByte(hl)); break;

        // and a, n8
        case 0xE6: and_(fetchByte()); break;

        // cpl
        case 0x2F: cpl(); break;

        // or a, r8
        case 0xB7: or_(a); break;
        case 0xB0: or_(b); break;
        case 0xB1: or_(c); break;
        case 0xB2: or_(d); break;
        case 0xB3: or_(e); break;
        case 0xB4: or_(h); break;
        case 0xB5: or_(l); break;
        
        // or a, [hl]
        case 0xB6: or_(readByte(hl)); break;

        // or a, n8
        case 0xF6: or_(fetchByte()); break;

        // xor a, r8
        case 0xAF: xor_(a); break;
        case 0xA8: xor_(b); break;
        case 0xA9: xor_(c); break;
        case 0xAA: xor_(d); break;
        case 0xAB: xor_(e); break;
        case 0xAC: xor_(h); break;
        case 0xAD: xor_(l); break;
        
        // and a, [hl]
        case 0xAE: xor_(readByte(hl)); break;

        // and a, n8
        case 0xEE: xor_(fetchByte()); break;

        // rla
        case 0x17: rla(); break;

        // rlca
        case 0x07: rlca(); break;

        // rra
        case 0x1F: rra(); break;

        // rrca
        case 0x0F: rrca(); break;

        // call n16
        case 0xCD: call(fetchWord()); break;

        // call cc, n16
        case 0xC4: call(!zero, fetchWord()); break;
        case 0xCC: call(zero, fetchWord()); break;
        case 0xD4: call(!carry, fetchWord()); break;
        case 0xDC: call(carry, fetchWord()); break;

        // jp hl
        case 0xE9: pc = hl; break;

        // jp n16
        case 0xC3: pc = fetchWord(); break;

        // jp cc, n16
        case 0xC2: jp(!zero, fetchWord()); break;
        case 0xCA: jp(zero, fetchWord()); break;
        case 0xD2: jp(!carry, fetchWord()); break;
        case 0xDA: jp(carry, fetchWord()); break;

        // jr n16
        case 0x18: pc += fetchRelative(); break;

        // jr cc, n16
        case 0x20: jr(!zero, fetchRelative()); break;
        case 0x28: jr(zero, fetchRelative()); break;
        case 0x30: jr(!carry, fetchRelative()); break;
        case 0x38: jr(carry, fetchRelative()); break;

        // ret
        case 0xC9: pop(pc); break;

        // ret cc
        case 0xC0: ret(!zero); break;
        case 0xC8: ret(zero); break;
        case 0xD0: ret(!carry); break;
        case 0xD8: ret(carry); break;

        // reti
        case 0xD9: reti(); break;

        // rst vec
        case 0xC7: call(0x00); break;
        case 0xCF: call(0x08); break;
        case 0xD7: call(0x10); break;
        case 0xDF: call(0x18); break;
        case 0xE7: call(0x20); break;
        case 0xEF: call(0x28); break;
        case 0xF7: call(0x30); break;
        case 0xFF: call(0x38); break;

        // ccf
        case 0x3F: ccf(); break;

        // scf
        case 0x37: scf(); break;

        // add sp, e8
        case 0xE8: add(fetchRelative()); break;

        // ld hl, sp+e8
        case 0xF8: hl = add(sp, fetchRelative()); break;

        // pop r16
        case 0xF1: {
            uint16_t value;
            pop(value);
            af = value;
            break;
        }
        case 0xC1: {
            uint16_t value;
            pop(value);
            bc = value;
            break;
        } 
        case 0xD1: {
            uint16_t value;
            pop(value);
            de = value;
            break;
        }
        case 0xE1: {
            uint16_t value;
            pop(value);
            hl = value;
            break;
        }

        // push r16
        case 0xF5: push(af); break;
        case 0xC5: push(bc); break;
        case 0xD5: push(de); break;
        case 0xE5: push(hl); break;

        // di
        case 0xF3: ime = false; break;

        // ei
        case 0xFB: imePending = true; break;

        // halt
        case 0x76: halted = true; break;

        // daa
        case 0x27: daa(); break;

        // nop
        case 0x00: break;

        // stop
        case 0x10: break;
    }
}