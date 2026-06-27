#include "cpu.hpp"
#include "instruction.hpp"

#include <stdexcept>
#include <format>

namespace MintGB {

static constexpr uint16_t highPage = 0xFF00;

void CPU::execute(Instruction ins) {
    switch (ins) {
        // ld r8, r8
        case Instruction::LD_A_A: a = a; break;
        case Instruction::LD_A_B: a = b; break;
        case Instruction::LD_A_C: a = c; break;
        case Instruction::LD_A_D: a = d; break;
        case Instruction::LD_A_E: a = e; break;
        case Instruction::LD_A_H: a = h; break;
        case Instruction::LD_A_L: a = l; break;

        case Instruction::LD_B_A: b = a; break;
        case Instruction::LD_B_B: b = b; break;
        case Instruction::LD_B_C: b = c; break;
        case Instruction::LD_B_D: b = d; break;
        case Instruction::LD_B_E: b = e; break;
        case Instruction::LD_B_H: b = h; break;
        case Instruction::LD_B_L: b = l; break;

        case Instruction::LD_C_A: c = a; break;
        case Instruction::LD_C_B: c = b; break;
        case Instruction::LD_C_C: c = c; break;
        case Instruction::LD_C_D: c = d; break;
        case Instruction::LD_C_E: c = e; break;
        case Instruction::LD_C_H: c = h; break;
        case Instruction::LD_C_L: c = l; break;

        case Instruction::LD_D_A: d = a; break;
        case Instruction::LD_D_B: d = b; break;
        case Instruction::LD_D_C: d = c; break;
        case Instruction::LD_D_D: d = d; break;
        case Instruction::LD_D_E: d = e; break;
        case Instruction::LD_D_H: d = h; break;
        case Instruction::LD_D_L: d = l; break;

        case Instruction::LD_E_A: e = a; break;
        case Instruction::LD_E_B: e = b; break;
        case Instruction::LD_E_C: e = c; break;
        case Instruction::LD_E_D: e = d; break;
        case Instruction::LD_E_E: e = e; break;
        case Instruction::LD_E_H: e = h; break;
        case Instruction::LD_E_L: e = l; break;

        case Instruction::LD_H_A: h = a; break;
        case Instruction::LD_H_B: h = b; break;
        case Instruction::LD_H_C: h = c; break;
        case Instruction::LD_H_D: h = d; break;
        case Instruction::LD_H_E: h = e; break;
        case Instruction::LD_H_H: h = h; break;
        case Instruction::LD_H_L: h = l; break;

        case Instruction::LD_L_A: l = a; break;
        case Instruction::LD_L_B: l = b; break;
        case Instruction::LD_L_C: l = c; break;
        case Instruction::LD_L_D: l = d; break;
        case Instruction::LD_L_E: l = e; break;
        case Instruction::LD_L_H: l = h; break;
        case Instruction::LD_L_L: l = l; break;

        // ld r8, n8
        case Instruction::LD_A_Imm8: a = fetchByte(); break;
        case Instruction::LD_B_Imm8: b = fetchByte(); break;
        case Instruction::LD_C_Imm8: c = fetchByte(); break;
        case Instruction::LD_D_Imm8: d = fetchByte(); break;
        case Instruction::LD_E_Imm8: e = fetchByte(); break;
        case Instruction::LD_H_Imm8: h = fetchByte(); break;
        case Instruction::LD_L_Imm8: l = fetchByte(); break;

        // ld r16, n16
        case Instruction::LD_BC_Imm16: bc(fetchWord()); break;
        case Instruction::LD_DE_Imm16: de(fetchWord()); break;
        case Instruction::LD_HL_Imm16: hl(fetchWord()); break;

        // ld [hl], r8
        case Instruction::LD_IndirectHL_A: writeByte(hl(), a); break;
        case Instruction::LD_IndirectHL_B: writeByte(hl(), b); break;
        case Instruction::LD_IndirectHL_C: writeByte(hl(), c); break;
        case Instruction::LD_IndirectHL_D: writeByte(hl(), d); break;
        case Instruction::LD_IndirectHL_E: writeByte(hl(), e); break;
        case Instruction::LD_IndirectHL_H: writeByte(hl(), h); break;
        case Instruction::LD_IndirectHL_L: writeByte(hl(), l); break;

        // ld [hl], n8
        case Instruction::LD_IndirectHL_Imm8: writeByte(hl(), fetchByte()); break;

        // ld r8, [hl]
        case Instruction::LD_A_IndirectHL: a = readByte(hl()); break;
        case Instruction::LD_B_IndirectHL: b = readByte(hl()); break;
        case Instruction::LD_C_IndirectHL: c = readByte(hl()); break;
        case Instruction::LD_D_IndirectHL: d = readByte(hl()); break;
        case Instruction::LD_E_IndirectHL: e = readByte(hl()); break;
        case Instruction::LD_H_IndirectHL: h = readByte(hl()); break;
        case Instruction::LD_L_IndirectHL: l = readByte(hl()); break;

        // ld [r16], a
        case Instruction::LD_IndirectBC_A: writeByte(bc(), a); break;
        case Instruction::LD_IndirectDE_A: writeByte(de(), a); break;

        // ld [a16], a
        case Instruction::LD_IndirectAddr16_A: writeByte(fetchWord(), a); break;

        // ldh [a16], a
        case Instruction::LDH_IndirectAddr8_A: writeByte(highPage + fetchByte(), a); break;

        // ldh [c], a
        case Instruction::LD_IndirectC_A: writeByte(highPage + c, a); break;

        // ld a, [r16]
        case Instruction::LD_A_IndirectBC: a = readByte(bc()); break;
        case Instruction::LD_A_IndirectDE: a = readByte(de()); break;

        // ld a, [a16]
        case Instruction::LD_A_IndirectAddr16: a = readByte(fetchWord()); break;

        // ldh a, [a16]
        case Instruction::LDH_A_IndirectAddr8: a = readByte(highPage + fetchByte()); break;

        // ldh a, [c]
        case Instruction::LD_A_IndirectC: a = readByte(highPage + c); break;

        // ld [hli], a
        case Instruction::LD_IndirectHLI_A: {
            uint16_t addr = hl();
            writeByte(addr, a);
            hl(addr + 1);
        } break;

        // ld [hld], a
        case Instruction::LD_IndirectHLD_A: {
            uint16_t addr = hl();
            writeByte(addr, a);
            hl(addr - 1);
        } break;

        // ld a, [hli]
        case Instruction::LD_A_IndirectHLI: {
            uint16_t addr = hl();
            a = readByte(addr);
            hl(addr + 1);
        } break;

        // ld a, [hld]
        case Instruction::LD_A_IndirectHLD: {
            uint16_t addr = hl();
            a = readByte(addr);
            hl(addr - 1);
        } break;

        // adc a, r8
        case Instruction::ADC_A_A: adc(a); break;
        case Instruction::ADC_A_B: adc(b); break;
        case Instruction::ADC_A_C: adc(c); break;
        case Instruction::ADC_A_D: adc(d); break; 
        case Instruction::ADC_A_E: adc(e); break;
        case Instruction::ADC_A_H: adc(h); break;
        case Instruction::ADC_A_L: adc(l); break;

        // adc a, [hl]
        case Instruction::ADC_A_IndirectHL: adc(readByte(hl())); break;

        // adc a, n8
        case Instruction::ADC_A_Imm8: adc(fetchByte()); break;

        // add a, r8
        case Instruction::ADD_A_A: add(a); break;
        case Instruction::ADD_A_B: add(b); break;
        case Instruction::ADD_A_C: add(c); break;
        case Instruction::ADD_A_D: add(d); break;
        case Instruction::ADD_A_E: add(e); break;
        case Instruction::ADD_A_H: add(h); break;
        case Instruction::ADD_A_L: add(l); break;

        // add a, [hl]
        case Instruction::ADD_A_IndirectHL: add(readByte(hl())); break;

        // add a, n8
        case Instruction::ADD_A_Imm8: add(fetchByte()); break;

        // cp a, r8
        case Instruction::CP_A: cp(a); break;
        case Instruction::CP_B: cp(b); break;
        case Instruction::CP_C: cp(c); break;
        case Instruction::CP_D: cp(d); break;
        case Instruction::CP_E: cp(e); break;
        case Instruction::CP_H: cp(h); break;
        case Instruction::CP_L: cp(l); break;

        // cp a, [hl]
        case Instruction::CP_IndirectHL: cp(readByte(hl())); break;

        // cp a, n8
        case Instruction::CP_Imm8: cp(fetchByte()); break; 

        // dec r8
        case Instruction::DEC_A: dec(a); break;
        case Instruction::DEC_B: dec(b); break;
        case Instruction::DEC_C: dec(c); break;
        case Instruction::DEC_D: dec(d); break;
        case Instruction::DEC_E: dec(e); break;
        case Instruction::DEC_H: dec(h); break;
        case Instruction::DEC_L: dec(l); break;

        // dec [hl]
        case Instruction::DEC_IndirectHL: {
            uint8_t value = readByte(hl());
            dec(value);
            writeByte(hl(), value);
        } break;

        // inc r8
        case Instruction::INC_A: inc(a); break;
        case Instruction::INC_B: inc(b); break;
        case Instruction::INC_C: inc(c); break;
        case Instruction::INC_D: inc(d); break;
        case Instruction::INC_E: inc(e); break;
        case Instruction::INC_H: inc(h); break;
        case Instruction::INC_L: inc(l); break;

        // inc [hl]
        case Instruction::INC_IndirectHL: {
            uint8_t value = readByte(hl());
            inc(value);
            writeByte(hl(), value); 
        } break;

        // sbc a, r8
        case Instruction::SBC_A_A: sbc(a); break;
        case Instruction::SBC_A_B: sbc(b); break;
        case Instruction::SBC_A_C: sbc(c); break;
        case Instruction::SBC_A_D: sbc(d); break;
        case Instruction::SBC_A_E: sbc(e); break;
        case Instruction::SBC_A_H: sbc(h); break;
        case Instruction::SBC_A_L: sbc(l); break;

        // sbc a, [hl]
        case Instruction::SBC_A_IndirectHL: sbc(readByte(hl())); break; 

        // sbc a, n8
        case Instruction::SBC_A_Imm8: sbc(fetchByte()); break;

        // sub a, r8
        case Instruction::SUB_A: sub(a); break;
        case Instruction::SUB_B: sub(b); break;
        case Instruction::SUB_C: sub(c); break;
        case Instruction::SUB_D: sub(d); break;
        case Instruction::SUB_E: sub(e); break;
        case Instruction::SUB_H: sub(h); break;
        case Instruction::SUB_L: sub(l); break;

        // sub a, [hl]
        case Instruction::SUB_IndirectHL: sub(readByte(hl())); break; 

        // sub a, n8
        case Instruction::SUB_Imm8: sub(fetchByte()); break;

        // add hl, r16
        case Instruction::ADD_HL_BC: add(bc()); break; 
        case Instruction::ADD_HL_DE: add(de()); break;
        case Instruction::ADD_HL_HL: add(hl()); break;

        // dec r16
        case Instruction::DEC_BC: {
            bc(bc() - 1);
            waitCycle();
        } break;
        case Instruction::DEC_DE: {
            de(de() - 1);
            waitCycle();
        } break;
        case Instruction::DEC_HL: {
            hl(hl() - 1);
            waitCycle();
        } break;

        // inc r16
        case Instruction::INC_BC: {
            bc(bc() + 1);
            waitCycle();
        } break;
        case Instruction::INC_DE: {
            de(de() + 1);
            waitCycle();
        } break;
        case Instruction::INC_HL: {
            hl(hl() + 1);
            waitCycle();
        } break;

        // and a, r8
        case Instruction::AND_A: and_(a); break; 
        case Instruction::AND_B: and_(b); break;
        case Instruction::AND_C: and_(c); break;
        case Instruction::AND_D: and_(d); break;
        case Instruction::AND_E: and_(e); break;
        case Instruction::AND_H: and_(h); break;
        case Instruction::AND_L: and_(l); break;

        // and a, [hl]
        case Instruction::AND_IndirectHL: and_(readByte(hl())); break;

        // and a, n8
        case Instruction::AND_Imm8: and_(fetchByte()); break;

        // cpl
        case Instruction::CPL: 
            a = ~a; 
            nf(true);
            hf(true); 
            break;
        
        // or a, r8
        case Instruction::OR_A: or_(a); break; 
        case Instruction::OR_B: or_(b); break;
        case Instruction::OR_C: or_(c); break;
        case Instruction::OR_D: or_(d); break;
        case Instruction::OR_E: or_(e); break;
        case Instruction::OR_H: or_(h); break;
        case Instruction::OR_L: or_(l); break;

        // or a, [hl]
        case Instruction::OR_IndirectHL: or_(readByte(hl())); break;

        // or a, n8
        case Instruction::OR_Imm8: or_(fetchByte()); break;

        // xor a, r8
        case Instruction::XOR_A: xor_(a); break; 
        case Instruction::XOR_B: xor_(b); break;
        case Instruction::XOR_C: xor_(c); break;
        case Instruction::XOR_D: xor_(d); break;
        case Instruction::XOR_E: xor_(e); break;
        case Instruction::XOR_H: xor_(h); break;
        case Instruction::XOR_L: xor_(l); break;

        // or a, [hl]
        case Instruction::XOR_IndirectHL: xor_(readByte(hl())); break;

        // or a, n8
        case Instruction::XOR_Imm8: xor_(fetchByte()); break;

        // rla
        case Instruction::RLA: rla(); break;

        // rlca
        case Instruction::RLCA: rlca(); break;

        // rra
        case Instruction::RRA: rra(); break;

        // rrca
        case Instruction::RRCA: rrca(); break;

        // call n16
        case Instruction::CALL_Addr16: call(fetchWord()); break;

        // call cc, n16
        case Instruction::CALL_NZ_Addr16: {
            uint16_t addr = fetchWord();
            if (!zf()) call(addr);
        } break;
        case Instruction::CALL_Z_Addr16: {
            uint16_t addr = fetchWord();
            if (zf()) call(addr);
        } break;
        case Instruction::CALL_NC_Addr16: {
            uint16_t addr = fetchWord();
            if (!cf()) call(addr);
        } break;
        case Instruction::CALL_C_Addr16: {
            uint16_t addr = fetchWord();
            if (!cf()) call(addr);
        }
        
        // jp hl
        case Instruction::JP_IndirectHL: pc = hl(); break;

        // jp n16
        case Instruction::JP_Addr16: pc = fetchWord(); break;
        
        // jp cc, n16
        case Instruction::JP_NZ_Addr16: {
            uint16_t addr = fetchWord();
            if (!zf()) { pc = addr; waitCycle(); }
        } break;
        case Instruction::JP_Z_Addr16: {
            uint16_t addr = fetchWord();
            if (zf()) { pc = addr; waitCycle(); }
        } break;
        case Instruction::JP_NC_Addr16: {
            uint16_t addr = fetchWord();
            if (!cf()) { pc = addr; waitCycle(); }
        } break;
        case Instruction::JP_C_Addr16: {
            uint16_t addr = fetchWord();
            if (cf()) { pc = addr; waitCycle(); }
        } break;

        // jr n16
        case Instruction::JR_Rel8: 
            pc += fetchRelative(); 
            waitCycle();
            break;

        // jr cc, n16
        case Instruction::JR_NZ_Rel8: {
            int8_t offset = fetchRelative();
            if (!zf()) { pc += offset; waitCycle(); }
        } break;
        case Instruction::JR_Z_Rel8: {
            int8_t offset = fetchRelative();
            if (zf()) { pc += offset; waitCycle(); }
        } break;
        case Instruction::JR_NC_Rel8: {
            int8_t offset = fetchRelative();
            if (!cf()) { pc += offset; waitCycle(); }
        } break;
        case Instruction::JR_C_Rel8: {
            int8_t offset = fetchRelative();
            if (cf()) { pc += offset; waitCycle(); }
        } break;

        // ret
        case Instruction::RET: ret(); break;

        // ret cc
        case Instruction::RET_NZ: {
            waitCycle();
            if (!zf()) ret(); 
        } break;
        case Instruction::RET_Z: {
            waitCycle();
            if (zf()) ret(); 
        } break;
        case Instruction::RET_NC: {
            waitCycle();
            if (!cf()) ret(); 
        } break;
        case Instruction::RET_C: {
            waitCycle();
            if (cf()) ret(); 
        } break;

        // reti
        case Instruction::RETI:
            ret();
            ime = true;
            break;

        // rst
        case Instruction::RST_00H: call(0x00); break;
        case Instruction::RST_08H: call(0x08); break;
        case Instruction::RST_10H: call(0x10); break;
        case Instruction::RST_18H: call(0x18); break;
        case Instruction::RST_20H: call(0x20); break;
        case Instruction::RST_28H: call(0x28); break;
        case Instruction::RST_30H: call(0x30); break;
        case Instruction::RST_38H: call(0x38); break;

        // ccf
        case Instruction::CCF:
            cf(!cf());
            hf(false);
            nf(false);
            break;

        // scf
        case Instruction::SCF:
            cf(true);
            hf(false);
            nf(false);
            break;

        // add hl, sp
        case Instruction::ADD_HL_SP: add(sp); break;

        // add sp, e8
        case Instruction::ADD_SP_Rel8: sp = spRel(); break;

        // dec sp
        case Instruction::DEC_SP:
            sp--;
            waitCycle();
            break;

        // inc sp
        case Instruction::INC_SP:
            sp++;
            waitCycle();
            break;

        // ld sp, n16
        case Instruction::LD_SP_Imm16: sp = fetchWord(); break;

        // ld [n16], sp
        case Instruction::LD_IndirectAddr16_SP: writeWord(fetchWord(), sp); break;

        // ld hl, sp+e8
        case Instruction::LD_HL_SPRel8: hl(spRel()); break;

        // di
        case Instruction::DI: ime = false; break;

        // ei
        case Instruction::EI: ei = true; break;

        // halt
        case Instruction::HALT: halt(); break;

        // daa
        case Instruction::DAA: daa(); break;

        // nop
        case Instruction::NOP: break;

        // stop
        case Instruction::STOP_0: stop(); break;

        default:
            throw std::runtime_error(std::format("Illegal opcode reached: {:02X}", static_cast<uint8_t>(ins)));
    }
}

}
