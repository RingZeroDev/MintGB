#pragma once

#include "bus.hpp"
#include "cartridge/cartridge.hpp"

class MMU : public Bus {
    private:
        Cartridge& cart;
    
    public:
        MMU(Cartridge& cart);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
        void cycle();
};