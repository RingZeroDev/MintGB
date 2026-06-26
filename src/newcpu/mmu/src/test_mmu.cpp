#include "test_mmu.hpp"

#include <stdexcept>

namespace MintGB {

uint8_t MMU::readByte(uint16_t addr) {
    uint8_t value = memory[addr];
    transactions.emplace(Transaction {
        .addr = addr,
        .value = value,
        .type = TransactionType::Read
    });
    return value;
}

void MMU::writeByte(uint16_t addr, uint8_t value) {
    transactions.emplace(Transaction {
        .addr = addr,
        .value = value,
        .type = TransactionType::Write
    });
    memory[addr] = value;
}

void MMU::waitCycle() {
    if (!transactions.empty()) {
        const Transaction& prev = transactions.back();
        transactions.emplace(Transaction {
            .addr = prev.addr,
            .value = prev.value,
            .type = TransactionType::Wait  
        });
    } else {
        transactions.emplace(Transaction {
            .addr = 0x0000,
            .value = 0x00,
            .type = TransactionType::Wait
        });
    }
}

const MMU::Transaction& MMU::nextTransaction() {
    if (!transactions.empty()) {
        const Transaction& first = transactions.front();
        transactions.pop();
        return first;
    } else {
        throw std::runtime_error("Attempted to access next transaction when transaction queue is empty.");
    }
}

}