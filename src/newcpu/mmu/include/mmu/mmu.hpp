#ifndef MMU_HPP
#define MMU_HPP

#include <cstdint>

namespace MintGB {

/**
 * @brief A class that handles memory transactions between the CPU and hardware components.
 * 
 * There are two seperate versions of MMU. This is the one that implements full Gameboy functionality.
 */
class MMU {
    public:
    explicit MMU();

    /**
     * @brief Reads a byte from the memory bus.
     */
    uint8_t readByte(uint16_t addr);

    /**
     * @brief Writes a byte to the memory bus.
     */
    void writeByte(uint16_t addr, uint8_t value);

    /**
     * @brief Waits one M-cycle.
     */
    void waitCycle();
};

}

#endif