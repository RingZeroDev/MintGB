#include "disassembler.hpp"

#include <format>
#include <iostream>

static constexpr char HEX[] = "0123456789ABCDEF";

template <typename T>
constexpr std::string getHexStr(T value) {
    constexpr int numDigits = sizeof(T) * 2;
    constexpr int numBits = sizeof(T) * 8;
    std::string str(numDigits, '0');

    for (int i = 0; i < numDigits; i++) {
        str[i] = HEX[(value >> (numBits - 4 * (i+1))) & 0xF];
    }

    return str;
}

void Disassembler::attachBus(Bus* newBus) {
    bus = newBus;
}

std::string Disassembler::getOperandStr(Operand operand) {
    switch (operand) {
        // 8-bit registers
        case Operand::A: return "A";
        case Operand::B: return "B";
        case Operand::C: return "C";
        case Operand::D: return "D";
        case Operand::E: return "E";
        case Operand::H: return "H";
        case Operand::L: return "L";
        // 16-bit registers
        case Operand::AF: return "AF";
        case Operand::BC: return "BC";
        case Operand::DE: return "DE";
        case Operand::HL: return "HL";
        case Operand::SP: return "SP";
        // Indirect registers
        case Operand::IndirectC: return "(C)";
        case Operand::IndirectBC: return "(BC)";
        case Operand::IndirectDE: return "(DE)";
        case Operand::IndirectHL: return "(HL)";
        case Operand::IndirectHLI: return "(HL+)";
        case Operand::IndirectHLD: return "(HL-)";
        // Conditions
        case Operand::NC: return "NC";
        case Operand::NZ: return "NZ";
        case Operand::Z: return "Z";
        // Vectors
        case Operand::Vec00h: return "$00";
        case Operand::Vec08h: return "$08";
        case Operand::Vec10h: return "$10";
        case Operand::Vec18h: return "$18";
        case Operand::Vec20h: return "$20";
        case Operand::Vec28h: return "$28";
        case Operand::Vec30h: return "$30";
        case Operand::Vec38h: return "$38";
        // Numbers
        case Operand::Zero: return "0";
        case Operand::One: return "1";
        case Operand::Two: return "2";
        case Operand::Three: return "3";
        case Operand::Four: return "4";
        case Operand::Five: return "5";
        case Operand::Six: return "6";
        case Operand::Seven: return "7";
        // Extended 8-bit
        case Operand::Imm8:
            return "$" + getHexStr(bus->read(currentAddr+1));
        case Operand::Rel8: {
            uint16_t ea = static_cast<int8_t>(bus->read(currentAddr+1)) + currentAddr + 2;
            return "$" + getHexStr(ea);
        }
        // Extended 16-bit
        case Operand::Imm16:
        case Operand::Addr16: {
            uint16_t compound = bus->read(currentAddr+1) | bus->read(currentAddr+2) << 8;
            return "$" + getHexStr(compound);
        }
        // Indirect 8-bit
        case Operand::IndirectAddr8:
            return "($" + getHexStr(bus->read(currentAddr+1)) + ")";
        // Indirect 16-bit
        case Operand::IndirectAddr16: {
            uint16_t compound = bus->read(currentAddr+1) | bus->read(currentAddr+2) << 8;
            return "($" + getHexStr(compound) + ")";
        }
        // Misc
        case Operand::SPRel8:
            return "SP+" + getHexStr(bus->read(currentAddr+1));
        default:
            throw std::runtime_error(std::format("Unsupported operand type reached: {}", static_cast<int>(operand)));
    }
}

std::string Disassembler::getInstructionStr(const Instruction& ins) {
    std::string line;
    line.reserve(16);
    
    line += ins.mnemonic;
    if (ins.operand1 != Operand::None) {
        line += " " + getOperandStr(ins.operand1);
        if (ins.operand2 != Operand::None) {
            line += ", " + getOperandStr(ins.operand2);   
        }
    }
    return line;
}

std::string Disassembler::disassembleSingle() {
    uint8_t opcode = bus->read(currentAddr);
    
    std::string output;
    if (opcode == 0xCB) {
        const Instruction& ins = getCBPrefixedInstruction(opcode);
        output = getHexStr(currentAddr) + ": " + getInstructionStr(ins);
        currentAddr += ins.length;
    } else {
        const Instruction& ins = getUnprefixedInstruction(opcode);
        output = getHexStr(currentAddr) + ": " + getInstructionStr(ins);
        currentAddr += ins.length;
    }

    return output;
}

void Disassembler::setCurrentAddr(uint16_t addr) {
    currentAddr = addr;
}

std::vector<std::string> Disassembler::disassemble() {
    if (bus == nullptr) {
        throw std::runtime_error("Disassembler needs to be attached to a bus");
    }

    std::vector<std::string> output;
    while (currentAddr < 0x1000) {
        output.emplace_back(disassembleSingle());
    }
    return output;
}

std::vector<std::string> Disassembler::disassemble(unsigned int amount) {
    if (bus == nullptr) {
        throw std::runtime_error("Disassembler needs to be attached to a bus");
    }

    std::vector<std::string> output;
    for (int i = 0; i < amount; i++) {
        output.emplace_back(disassembleSingle());
    }
    return output;
}