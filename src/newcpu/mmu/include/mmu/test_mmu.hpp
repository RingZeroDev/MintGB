#ifndef MMU_HPP
#define MMU_HPP

#include <cstdint>
#include <array>

namespace MintGB {

/**
 * @brief A class that handles memory transactions between the CPU and hardware components using a dummy implementation.
 * 
 * There are two versions of MMU. This is the one that implements a flat memory array for testing.
 */
class MMU {
    public:
    explicit MMU() = default;

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

    private:
    static constexpr size_t memorySize = 0x10000;
    std::array<uint8_t, 0x10000> memory{};
};

}

#endif