#include "cpu.hpp"

#include "bus.hpp"
#include "register.hpp"

CPU::CPU(Bus& bus) : bus(bus) {}

uint8_t CPU::readByte(uint16_t addr) {
        return bus.read(addr);
}

uint16_t CPU::readWord(uint16_t addr) {
    return bus.read(addr) | (bus.read(addr+1) << 8);
}

void CPU::writeByte(uint16_t addr, uint8_t value) {
    bus.write(addr, value);
}

void CPU::writeWord(uint16_t addr, uint16_t value) {
    bus.write(addr, value & 0xFF);
    bus.write(addr+1, value >> 8);
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

void CPU::step() {
    uint8_t opcode = fetchByte();

    if (opcode == 0xCB) {
        uint8_t cbOpcode = fetchByte();
        decodeCB(cbOpcode);
    } else {
        decode(opcode);
    }
}


