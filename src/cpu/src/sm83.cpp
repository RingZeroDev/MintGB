#include "sm83.hpp"

#include "bus.hpp"
#include "register.hpp"

SM83::SM83(Bus& bus) : bus(bus) {}

uint8_t SM83::readByte(uint16_t addr) {
        return bus.read(addr);
}

uint16_t SM83::readWord(uint16_t addr) {
    return bus.read(addr) | (bus.read(addr+1) << 8);
}

void SM83::writeByte(uint16_t addr, uint8_t value) {
    bus.write(addr, value);
}

void SM83::writeWord(uint16_t addr, uint16_t value) {
    bus.write(addr, value & 0xFF);
    bus.write(addr+1, value >> 8);
}

void SM83::cycle() {
    bus.cycle();
}

void SM83::cycle(int amount) {
    for (int i = 0; i < amount; i++) cycle();
}

uint8_t SM83::fetchByte() {
    return readByte(pc++);
}

uint16_t SM83::fetchWord() {
    uint16_t value = readWord(pc);
    pc += 2;
    return value;
}

int8_t SM83::fetchRelative() {
    return static_cast<int8_t>(fetchByte());
}

void SM83::step() {
    uint8_t opcode = fetchByte();

    if (opcode == 0xCB) {
        uint8_t cbOpcode = fetchByte();
        executeCB(cbOpcode);
    } else {
        execute(opcode);
    }
}


