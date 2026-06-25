from jinja2 import Environment, FileSystemLoader
import json

env = Environment(loader = FileSystemLoader("templates"), trim_blocks=True, lstrip_blocks=True)
template = env.get_template("enum.cpp.j2")

operandsMap = {
    "d8": "Imm8",
    "d16": "Imm16",
    "a16": "Addr16",
    "(C)": "IndirectC",
    "(BC)": "IndirectBC",
    "(DE)": "IndirectDE",
    "(HL)": "IndirectHL",
    "(HL+)": "IndirectHLI",
    "(HL-)": "IndirectHLD",
    "(a8)": "IndirectAddr8",
    "(a16)": "IndirectAddr16",
    "r8": "Rel8",
    "SP+r8": "SPRel8",
}

def formatOperand(operand):
    if operand in operandsMap:
        return f"_{operandsMap[operand]}"
    return f"_{operand}"

def getOperand(opcode, pos):
    if pos == 1:
        if "operand1" in opcode:
            return formatOperand(opcode["operand1"])
    elif pos == 2:
        if "operand2" in opcode:
            return formatOperand(opcode["operand2"])
    return ""

state = {
    "getOperand": getOperand
}

opcodes = {}
with open("game-boy-opcodes/opcodes.json") as opcodesFile:
    opcodes = json.load(opcodesFile)

def outputTable(path, name, guard, prefix, comment):
    with open(path, "w") as outputFile:
        output = template.render(state, enumName=name, guard=guard, opcodes=opcodes[prefix], comment=comment)
        outputFile.write(output)

outputTable("instruction.hpp", "Instruction", "INSTRUCTION_HPP", "unprefixed", """
/**
 * @brief An enum for all of the legal unprefixed instructions for the CPU.
 */""")

outputTable("cb_instruction.hpp", "CBInstruction", "CB_INSTRUCTION_HPP", "cbprefixed", """
/**
 * @brief An enum for all of the legal CB prefixed instructions for the CPU.
 */""")