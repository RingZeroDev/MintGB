#include "mmu.hpp" 

MMU::MMU(Cartridge& cart) : cart(cart) {}

uint8_t MMU::read(uint16_t addr) {
    if (addr <= 0x7FFF) { 
        // 16 KiB ROM Bank 00 and 16 KiB ROM Bank 01-NN
        return cart.read(addr);
    } else if (addr <= 0x9FFF) { 
        // 8-KiB Video RAM (VRAM)
        return vram[addr - 0x8000];
    } else if (addr <= 0xBFFF) {
        // 8-KiB External RAM
        return 0xFF; // mappers not supported yet
    } else if (addr <= 0xDFFF) {
        // 8-KiB Work RAM (WRAM)
        return wram[addr - 0xC000];
    } else if (addr <= 0xFDFF) {
        // Echo RAM
        throw std::runtime_error("Echo RAM is prohibited!");
    } else if (addr <= 0xFE9F) {
        // OAM Memory
        return oam[addr - 0xFE00];
    } else if (addr <= 0xFEFF) {
        // Not usable
        throw std::runtime_error("Prohibited memory range written to!");
    } else if (addr <= 0xFF7F) {
        // I/O Registers
        return readIO(addr & 0xFF);
    } else if (addr <= 0xFFFE) {
        // High RAM (HRAM)
        return hram[addr - 0xFF80];
    } else {
        // Interrupt Enable Register
        return 0xFF;
    }
}

void MMU::write(uint16_t addr, uint8_t value) {
    if (addr <= 0x7FFF) { 
        // 16 KiB ROM Bank 00 and 16 KiB ROM Bank 01-NN
        return; // do nothing (bank switching unimplemented)
    } else if (addr <= 0x9FFF) { 
        // 8-KiB Video RAM (VRAM)
        vram[addr - 0x8000] = value;
    } else if (addr <= 0xBFFF) {
        // 8-KiB External RAM
        return; // do nothing (external ram not supported)
    } else if (addr <= 0xDFFF) {
        // 8-KiB Work RAM (WRAM)
        wram[addr - 0xC000] = value;
    } else if (addr <= 0xFDFF) {
        // Echo RAM
        return; // echo RAM not supported
    } else if (addr <= 0xFE9F) {
        // OAM Memory
        oam[addr - 0xFE00] = value;
    } else if (addr <= 0xFEFF) {
        // Not usable
        return; // prohibited memory range
    } else if (addr <= 0xFF7F) {
        // I/O Registers
        writeIO(addr & 0xFF, value);
    } else if (addr <= 0xFFFE) {
        // High RAM (HRAM)
        hram[addr - 0xFF80] = value;
    } else {
        // Interrupt Enable Register
        return;
    }
}

uint8_t MMU::readIO(uint8_t addr) {
    switch (addr) {
        // Joypad input
        case 0x00: return 0xFF;

        // Serial transfer
        case 0x01: 
        case 0x02:
            return 0xFF;

        // Timer and divider
        case 0x04:
        case 0x05:
        case 0x06: 
        case 0x07:
            return 0xFF;

        // Interrupts
        case 0x0F: return 0xFF;

        // Audio
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
            return 0xFF;

        // Wave pattern
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:
            return 0xFF;

        // LCD Control, Status, Position, Scrolling, and Palettes
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44: return 0x94; // hack for now
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
            return 0xFF;

        // Boot ROM Mapping Control
        case 0x50: return 0xFF;

        default:
            return 0xFF; // Ignore the write (unused IO registers)
    }
} 

void MMU::writeIO(uint8_t addr, uint8_t value) {
    switch (addr) {
        // Joypad input
        case 0x00: return;

        // Serial transfer
        case 0x01: 
        case 0x02:
            return;

        // Timer and divider
        case 0x04:
        case 0x05:
        case 0x06: 
        case 0x07:
            return;

        // Interrupts
        case 0x0F: return;

        // Audio
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
            return;

        // Wave pattern
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:
            return;

        // LCD Control, Status, Position, Scrolling, and Palettes
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
            return;

        // Boot ROM Mapping Control
        case 0x50: return;

        default:
            return; // Ignore the write (unused IO registers)
    }
}

void MMU::cycle() {}