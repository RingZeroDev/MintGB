#ifndef MMU_HPP
#define MMU_HPP

#include <cstdint>
#include <array>
#include <queue>

namespace MintGB {

/**
 * @brief A class that handles memory transactions between the CPU and hardware components using a dummy implementation.
 * 
 * There are two versions of MMU. This is the one that implements a flat memory array for testing.
 */
class MMU {
    public:
    struct Transaction;
    enum class TransactionType;

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

    /**
     * @brief Retrieves the oldest transaction in a series of transactions and pops it from the queue.
     */
    const Transaction& nextTransaction();

    /**
     * @brief Clears the current transaction history.
     */
    void clearTransactions();

    private:

    static constexpr size_t memorySize = 0x10000;
    std::array<uint8_t, 0x10000> memory{};

    std::queue<Transaction> transactions{};
};

/**
 * @brief The type of a bus transaction.
 */
enum class MMU::TransactionType {
    Read,
    Write,
    Wait
};

/**
 * @brief Describes a bus transaction
 */
struct MMU::Transaction {
    uint16_t addr;
    uint8_t value;
    MMU::TransactionType type;

    bool operator==(const Transaction&) const = default;
};

}

#endif