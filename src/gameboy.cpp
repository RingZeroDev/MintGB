#include "gameboy.hpp"

#include <stdexcept>

void Gameboy::reset() {
    cpu.reset();
}

void Gameboy::insertCartridge(Cartridge* newCart) {
    reset();
    cart = newCart;
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

CPUState Gameboy::getCPUState() {
    return cpu.getState();
}

void Gameboy::setCPUState(CPUState state) {
    cpu.setState(state);
}