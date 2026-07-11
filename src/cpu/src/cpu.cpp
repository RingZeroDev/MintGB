#include "cpu.hpp"

#include <bit>

#include "mmu/bus.hpp"
#include "register.hpp"

CPU::CPU(Bus& bus, InterruptSystem& interrupt) : bus(bus), interrupt(interrupt) {}

uint8_t CPU::readByte(uint16_t addr) {
    bus.cycle();
    return bus.read(addr);
}

uint16_t CPU::readWord(uint16_t addr) {
    return readByte(addr) | (readByte(addr+1) << 8);
}

void CPU::writeByte(uint16_t addr, uint8_t value) {
    bus.cycle();
    bus.write(addr, value);
}

void CPU::writeWord(uint16_t addr, uint16_t value) {
    writeByte(addr, value & 0xFF);
    writeByte(addr+1, value >> 8);
}

void CPU::cycle() {
    bus.cycle();
}

void CPU::cycle(int amount) {
    for (int i = 0; i < amount; i++) cycle();
}

uint8_t CPU::fetchByte() {
    return readByte(pc++);
}

uint16_t CPU::fetchWord() {
    uint16_t value = readWord(pc);
    pc += 2;
    return value;
}

int8_t CPU::fetchRelative() {
    return static_cast<int8_t>(fetchByte());
}

void CPU::reset() {
    pc = 0x0000;
    sp = 0x0000;
    af = 0x0000;
    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;

    zero = false;
    subtract = false;
    halfCarry = false;
    carry = false;

    ime = false;
    imePending = false;
    halted = false;
}

void CPU::step() {
    if (imePending) {
        imePending = false;
        ime = true;
    }

    uint8_t inte = interrupt.readEnable() & interrupt.readFlag();
    if (ime && (inte & 0b00011111)) {
        serviceInterrupt(inte);
        return;
    }

    uint8_t opcode = fetchByte();

    if (opcode == 0xCB) {
        uint8_t cbOpcode = fetchByte();
        decodeCB(cbOpcode);
    } else {
        decode(opcode);
    }
}

constexpr std::array<uint8_t, 5> interruptVectors = {{ 0x40, 0x48, 0x50, 0x58, 0x60 }};

void CPU::serviceInterrupt(uint8_t inte) {
    int bit = std::countr_zero(inte);

    interrupt.writeFlag(interrupt.readFlag() & ~(1 << bit));
    ime = false;

    cycle(2);
    call(interruptVectors[bit]);
    cycle();
}

CPUState CPU::getState() const {
    return CPUState {
        pc, sp, af, bc, de, hl,

        zero, subtract, halfCarry, carry,
        ime, imePending, halted
    };
}

void CPU::setState(const CPUState& state) {
    pc = state.pc;
    sp = state.sp;
    af = state.af & 0b1111111111110000;
    bc = state.bc;
    de = state.de;
    hl = state.hl;

    zero = state.zero;
    subtract = state.subtract;
    halfCarry = state.halfCarry;
    carry = state.carry;

    ime = state.ime;
    imePending = state.imePending;
    halted = state.halted;
}


