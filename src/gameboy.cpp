#include "gameboy.hpp"

#include <stdexcept>

void Gameboy::reset() {
    cpu.reset();
    interrupt.reset();
    timer.reset();
    joypad.reset();
}

void Gameboy::insertCartridge(Cartridge* newCart) {
    reset();
    cart = newCart;
    mmu.insertCartridge(newCart);
}

void Gameboy::stepFrame() {
    if (cart == nullptr) {
        throw std::runtime_error("Attempted to run Gameboy system with no cartridge present");
    }

    while (!ppu.isFrameComplete()) {
        cpu.step();
    }
}

void Gameboy::stepInstruction() {
    if (cart == nullptr) {
        throw std::runtime_error("Attempted to run Gameboy system with no cartridge present");
    }

    cpu.step();
}

void Gameboy::stepInstructions(unsigned int amount) {
    if (cart == nullptr) {
        throw std::runtime_error("Attempted to run Gameboy system with no cartridge present");
    }

    for (int i = 0; i < amount; i++) {
        cpu.step();
    }
}

const Bus& Gameboy::getBus() const {
    return mmu;
}

Bus& Gameboy::getBus() {
    return mmu;
}

CPUState Gameboy::getCPUState() {
    return cpu.getState();
}

void Gameboy::setCPUState(CPUState state) {
    cpu.setState(state);
}

TimerState Gameboy::getTimerState() {
    return timer.getState();
}

void Gameboy::setTimerState(TimerState state) {
    timer.setState(state);
}

JoypadInput Gameboy::getInput() {
    return joypad.getInput();
}

void Gameboy::setInput(JoypadInput input) {
    joypad.setInput(input);
}

uint8_t Gameboy::getJoypad() {
    return joypad.read();
}

void Gameboy::setJoypad(uint8_t value) {
    joypad.write(value);
}

std::pair<uint8_t, uint8_t> Gameboy::getInterruptState() {
    return std::make_pair(interrupt.readEnable(), interrupt.readFlag());
}

void Gameboy::setInterruptState(std::pair<uint8_t, uint8_t> state) {
    interrupt.writeEnable(state.first);
    interrupt.writeFlag(state.second);
}

void Gameboy::setInterruptCallback(InterruptCallback newCallback) {
    interrupt.setCallback(newCallback);
}

const std::array<uint8_t, 0x2000>& Gameboy::getVRAM() const { return mmu.vram; }
const std::array<uint8_t, 0x2000>& Gameboy::getWRAM() const { return mmu.wram; }
const std::array<uint8_t, 160>& Gameboy::getOAM() const { return mmu.oam; }
const std::array<uint8_t, 127>& Gameboy::getHRAM() const { return mmu.hram; }

std::array<uint8_t, 0x2000>& Gameboy::getVRAM() { return mmu.vram; }
std::array<uint8_t, 0x2000>& Gameboy::getWRAM() { return mmu.wram; }
std::array<uint8_t, 160>& Gameboy::getOAM() { return mmu.oam; }
std::array<uint8_t, 127>& Gameboy::getHRAM() { return mmu.hram; };