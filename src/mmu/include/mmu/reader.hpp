#pragma once

#include "bus.hpp"
#include "cartridge/cartridge.hpp"

class CartReader final : public Bus {
    private:
        Cartridge& cart;

    public:
        CartReader(Cartridge& cart);

        uint8_t read(uint16_t addr) override;
};