#pragma once

#include "mmu/bus.hpp"

#include <cstdint>
#include <string_view>
#include <array>
#include <vector>

enum class Operand {
    None,

    A,
    B,
    C,
    D,
    E,
    H,
    L,

    AF,
    BC,
    DE,
    HL,
    SP,

    IndirectC,
    IndirectBC,
    IndirectDE,
    IndirectHL,
    IndirectHLI,
    IndirectHLD,
    IndirectAddr8,
    IndirectAddr16,

    Imm8,
    Imm16,
    Rel8,
    Addr16,

    NC,
    NZ,
    Z,

    Vec00h,
    Vec08h,
    Vec10h,
    Vec18h,
    Vec20h,
    Vec28h,
    Vec30h,
    Vec38h,

    SPRel8,
    CB,

    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven
};

struct Instruction {
    std::string_view mnemonic;
    Operand operand1;
    Operand operand2;
    uint8_t length;
};

class Disassembler {
    private:
        Bus* bus;
        uint16_t currentAddr = 0x0000;

        static const Instruction& getUnprefixedInstruction(uint8_t opcode);
        static const Instruction& getCBPrefixedInstruction(uint8_t opcode);

        std::string getOperandStr(Operand operand);
        std::string getInstructionStr(const Instruction &ins);

        std::string disassembleSingle();

    public:
        void attachBus(Bus* newBus);
        void setCurrentAddr(uint16_t addr);
        std::vector<std::string> disassemble();
        std::vector<std::string> disassemble(unsigned int amount);
};