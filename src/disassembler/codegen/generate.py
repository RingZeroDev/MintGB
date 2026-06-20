from jinja2 import Environment, FileSystemLoader
import json

env = Environment(loader = FileSystemLoader("templates"), trim_blocks=True, lstrip_blocks=True)
template = env.get_template("table.cpp.j2")

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
    "00H": "Vec00h",
    "08H": "Vec08h",
    "10H": "Vec10h",
    "18H": "Vec18h",
    "20H": "Vec20h",
    "28H": "Vec28h",
    "30H": "Vec30h",
    "38H": "Vec38h",
    "SP+r8": "SPRel8",
    "0": "Zero",
    "1": "One",
    "2": "Two",
    "3": "Three",
    "4": "Four",
    "5": "Five",
    "6": "Six",
    "7": "Seven"
}

def formatOperand(operand):
    if operand in operandsMap:
        return f"Operand::{operandsMap[operand]}"
    return f"Operand::{operand}"

def getOperand(opcode, pos):
    if pos == 1:
        if "operand1" in opcode:
            return formatOperand(opcode["operand1"])
    elif pos == 2:
        if "operand2" in opcode:
            return formatOperand(opcode["operand2"])
    return "Operand::None"

state = {
    "getOperand": getOperand
}

opcodes = {}
with open("game-boy-opcodes/opcodes.json") as opcodesFile:
    opcodes = json.load(opcodesFile)

def outputTable(path, name, prefix, getterName):
    with open(path, "w") as outputFile:
        output = template.render(state, tableName=name, opcodes=opcodes[prefix], getterName=getterName)
        outputFile.write(output)

outputTable("unprefixed_table.cpp", "UNPREFIXED_TABLE", "unprefixed", "getUnprefixedInstruction")
outputTable("cb_table.cpp", "CB_TABLE", "cbprefixed", "getCBPrefixedInstruction")