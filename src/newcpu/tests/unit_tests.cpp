#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "cpu/cpu.hpp"

namespace MintGB {

class CPU::Tests {
    public:
    void registerPairComp() {
        cpu.b = 0xAB;
        cpu.c = 0xCD;

        REQUIRE(cpu.bc() == 0xABCD);
    }
    
    void registerPairDecomp() {
        cpu.bc(0xABCD);

        REQUIRE(cpu.b == 0xAB);
        REQUIRE(cpu.c == 0xCD);
    }

    void afRegisterPairSet() {
        cpu.af(0xFFFF);

        REQUIRE(cpu.af() == 0xFFF0);
    }

    void cpuStateGet() {
        cpu.state(CPU::State{
            .af = 0xABCD,
            .bc = 0xABCD,
            .de = 0xABCD,
            .hl = 0xABCD,

            .pc = 0xABCD,
            .sp = 0xABCD,

            .ime = true,
            .ei = true,
            .halted = true
        });

        REQUIRE(cpu.af() == 0xABC0); // lower nibble gets cleared
        REQUIRE(cpu.bc() == 0xABCD);
        REQUIRE(cpu.de() == 0xABCD);
        REQUIRE(cpu.hl() == 0xABCD);

        REQUIRE(cpu.pc == 0xABCD);
        REQUIRE(cpu.sp == 0xABCD);

        REQUIRE(cpu.ime);
        REQUIRE(cpu.ei);
        REQUIRE(cpu.halted);
    }

    void cpuStateSet() {
        cpu.af(0xABCD);
        cpu.bc(0xABCD);
        cpu.de(0xABCD);
        cpu.hl(0xABCD);

        cpu.pc = 0xABCD;
        cpu.sp = 0xABCD;

        cpu.ime = true;
        cpu.ei = true;
        cpu.halted = true;

        REQUIRE(cpu.state() == CPU::State{
            .af = 0xABC0, // lower nibble gets cleared
            .bc = 0xABCD,
            .de = 0xABCD,
            .hl = 0xABCD,
            
            .pc = 0xABCD,
            .sp = 0xABCD,

            .ime = true,
            .ei = true,
            .halted = true
        });
    }

    void defaultState() {
        REQUIRE(cpu.state() == CPU::State{
            .af = 0x0000,
            .bc = 0x0000,
            .de = 0x0000,
            .hl = 0x0000,

            .pc = 0x0000,
            .sp = 0x0000,

            .ime = false,
            .ei = false,
            .halted = false
        });
    }

    void flagComp() {
        cpu.f = 0xF0;

        REQUIRE(cpu.zf());
        REQUIRE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE(cpu.cf());
    }

    void flagDecomp() {
        cpu.zf(true);
        cpu.nf(true);
        cpu.hf(true);
        cpu.cf(true);

        REQUIRE(cpu.f == 0xF0);
    }

    void memoryRead() {
        cpu.readByte(0xABCD);

        REQUIRE(mmu.nextTransaction() == MMU::Transaction{
            .addr = 0xABCD,
            .value = 0x00,
            .type = MMU::TransactionType::Read
        });
    }

    void memoryWrite() {
        cpu.writeByte(0xABCD, 0xAB);

        REQUIRE(mmu.nextTransaction() == MMU::Transaction{
            .addr = 0xABCD,
            .value = 0xAB,
            .type = MMU::TransactionType::Write
        });
    }

    void memoryWait() {
        cpu.waitCycle();

        REQUIRE(mmu.nextTransaction() == MMU::Transaction{
            .addr = 0x0000,
            .value = 0x00,
            .type = MMU::TransactionType::Wait
        });
    }

    void verifyTestMemory() {
        cpu.writeByte(0xABCD, 0xAB);
        uint8_t value = cpu.readByte(0xABCD);

        REQUIRE(value == 0xAB);
    }

    void instruction_ld_r8_r8() {
        cpu.b = 0xAB;

        cpu.execute(Instruction::LD_A_B);

        REQUIRE(cpu.a == 0xAB);
    }

    void instruction_ld_r8_n8() {
        cpu.writeByte(0x0000, 0xAB);

        cpu.execute(Instruction::LD_B_Imm8);

        REQUIRE(cpu.b == 0xAB);
    }

    void instruction_ld_r16_n16() {
        cpu.writeWord(0x0000, 0xABCD);

        cpu.execute(Instruction::LD_BC_Imm16);
        
        REQUIRE(cpu.bc() == 0xABCD);
    }

    void instruction_ld_ind_hl_r8() {
        cpu.hl(0xABCD);
        cpu.a = 0xAB;

        cpu.execute(Instruction::LD_IndirectHL_A);

        REQUIRE(cpu.readByte(0xABCD) == 0xAB);
    }

    void instruction_ld_ind_hl_n8() {
        cpu.hl(0xABCD);
        cpu.writeByte(0x0000, 0xAB);

        cpu.execute(Instruction::LD_IndirectHL_Imm8);

        REQUIRE(cpu.readByte(0xABCD) == 0xAB);
    }

    void instruction_ld_r8_ind_hl() {
        cpu.hl(0xABCD);
        cpu.writeByte(0xABCD, 0xAB);

        cpu.execute(Instruction::LD_A_IndirectHL);

        REQUIRE(cpu.a == 0xAB);
    }

    void instruction_ld_ind_r16_a() {
        cpu.bc(0xABCD);
        cpu.a = 0xAB;

        cpu.execute(Instruction::LD_IndirectBC_A);

        REQUIRE(cpu.readByte(0xABCD) == 0xAB);
    }

    void instruction_ld_ind_a16_a() {
        cpu.writeWord(0x0000, 0xABCD);
        cpu.a = 0xAB;

        cpu.execute(Instruction::LD_IndirectAddr16_A);

        REQUIRE(cpu.readByte(0xABCD) == 0xAB);
    }

    void instruction_ldh_ind_a16_a() {
        cpu.writeByte(0x0000, 0xAB);
        cpu.a = 0xAB;

        cpu.execute(Instruction::LDH_IndirectAddr8_A);

        REQUIRE(cpu.readByte(0xFFAB) == 0xAB);
    }

    void instruction_ldh_ind_c_a() {
        cpu.c = 0xAB;
        cpu.a = 0xAB;

        cpu.execute(Instruction::LD_IndirectC_A);

        REQUIRE(cpu.readByte(0xFFAB) == 0xAB);
    }

    void instruction_ld_a_ind_r16() {
        cpu.bc(0xABCD);
        cpu.writeByte(0xABCD, 0xAB);

        cpu.execute(Instruction::LD_A_IndirectBC);

        REQUIRE(cpu.a == 0xAB);
    }

    void instruction_ld_a_ind_a16() {
        cpu.writeWord(0x0000, 0xABCD);
        cpu.writeByte(0xABCD, 0xAB);

        cpu.execute(Instruction::LD_A_IndirectAddr16);

        REQUIRE(cpu.a == 0xAB);
    }

    void instruction_ldh_a_ind_a16() {
        cpu.writeByte(0x0000, 0xAB);
        cpu.writeByte(0xFFAB, 0xAB);

        cpu.execute(Instruction::LDH_A_IndirectAddr8);

        REQUIRE(cpu.a == 0xAB);
    }

    void instruction_ldh_a_ind_c() {
        cpu.c = 0xAB;
        cpu.writeByte(0xFFAB, 0xAB);

        cpu.execute(Instruction::LD_A_IndirectC);

        REQUIRE(cpu.a == 0xAB);
    }

    void instruction_ld_ind_hli_a() {
        cpu.hl(0xABCD);
        cpu.a = 0xAB;

        cpu.execute(Instruction::LD_IndirectHLI_A);

        REQUIRE(cpu.readByte(0xABCD) == 0xAB);
        REQUIRE(cpu.hl() == 0xABCE);
    }

    void instruction_ld_ind_hld_a() {
        cpu.hl(0xABCD);
        cpu.a = 0xAB;

        cpu.execute(Instruction::LD_IndirectHLD_A);

        REQUIRE(cpu.readByte(0xABCD) == 0xAB);
        REQUIRE(cpu.hl() == 0xABCC);
    }

    void instruction_ld_a_ind_hli() {
        cpu.hl(0xABCD);
        cpu.writeByte(0xABCD, 0xAB);

        cpu.execute(Instruction::LD_A_IndirectHLI);

        REQUIRE(cpu.a == 0xAB);
        REQUIRE(cpu.hl() == 0xABCE);
    }

    void instruction_ld_a_ind_hld() {
        cpu.hl(0xABCD);
        cpu.writeByte(0xABCD, 0xAB);

        cpu.execute(Instruction::LD_A_IndirectHLD);

        REQUIRE(cpu.a == 0xAB);
        REQUIRE(cpu.hl() == 0xABCC);
    }

    void instructionGroupAdc() {
        cpu.a = 0xFF;
        cpu.b = 0x00;
        cpu.cf(true);

        cpu.execute(Instruction::ADC_A_B);

        REQUIRE(cpu.a == 0x00);
        REQUIRE(cpu.zf());
        REQUIRE_FALSE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE(cpu.cf());
    }

    void instructionGroupAdd() {
        cpu.a = 0xFF;
        cpu.b = 0x01;

        cpu.execute(Instruction::ADD_A_B);

        REQUIRE(cpu.a == 0x00);
        REQUIRE(cpu.zf());
        REQUIRE_FALSE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE(cpu.cf());
    }

    void instructionGroupCp() {
        cpu.a = 0x00;
        cpu.b = 0x01;

        cpu.execute(Instruction::CP_B);

        REQUIRE(cpu.a == 0x00);
        REQUIRE_FALSE(cpu.zf());
        REQUIRE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE(cpu.cf());
    }

    void instructionGroupDec() {
        cpu.a = 0x10;

        cpu.execute(Instruction::DEC_A);

        REQUIRE(cpu.a == 0x0F);
        REQUIRE_FALSE(cpu.zf());
        REQUIRE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE_FALSE(cpu.cf());
    }

    void instructionGroupInc() {
        cpu.a = 0x0F;

        cpu.execute(Instruction::INC_A);

        REQUIRE(cpu.a == 0x10);
        REQUIRE_FALSE(cpu.zf());
        REQUIRE_FALSE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE_FALSE(cpu.cf());
    }

    void instructionGroupSbc() {
        cpu.a = 0x00;
        cpu.b = 0x00;
        cpu.cf(1);

        cpu.execute(Instruction::SBC_A_B);

        REQUIRE(cpu.a == 0xFF);
        REQUIRE_FALSE(cpu.zf());
        REQUIRE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE(cpu.cf());
    }

    void instructionGroupSub() {
        cpu.a = 0x00;
        cpu.b = 0x01;

        cpu.execute(Instruction::SUB_B);

        REQUIRE(cpu.a == 0xFF);
        REQUIRE_FALSE(cpu.zf());
        REQUIRE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE(cpu.cf());
    }

    void instruction_add_hl_r16() {
        cpu.hl(0xFFFF);
        cpu.bc(0x0001);
        cpu.zf(true);

        cpu.execute(Instruction::ADD_HL_BC);

        REQUIRE(cpu.hl() == 0x0000);
        REQUIRE_FALSE(cpu.nf());
        REQUIRE(cpu.hf());
        REQUIRE(cpu.cf());
    }

    void instruction_dec_r16() {
        cpu.bc(0x0000);

        cpu.execute(Instruction::DEC_BC);

        REQUIRE(cpu.bc() == 0xFFFF);
    }

    void instruction_inc_r16() {
        cpu.bc(0xFFFF);

        cpu.execute(Instruction::INC_BC);

        REQUIRE(cpu.bc() == 0x0000);
    }

    void instructionGroupAnd() {
    }

    void instruction_cpl() {
        
    }

    void instructionGroupOr() {
        
    }

    void instructionGroupXor() {
        
    }

    private:
    MMU mmu{};
    CPU cpu{&mmu};
};

METHOD_AS_TEST_CASE(CPU::Tests::registerPairComp, "Register Pair Composition", "[RegisterPair]");
METHOD_AS_TEST_CASE(CPU::Tests::registerPairDecomp, "Register Pair Decomp", "[RegisterPair]");
METHOD_AS_TEST_CASE(CPU::Tests::afRegisterPairSet, "Set AF Register Pair", "[RegisterPair]");
METHOD_AS_TEST_CASE(CPU::Tests::cpuStateGet, "Get CPU State", "[State]");
METHOD_AS_TEST_CASE(CPU::Tests::cpuStateSet, "Set CPU State", "[State]");
METHOD_AS_TEST_CASE(CPU::Tests::defaultState, "Check default CPU State", "[State]");
METHOD_AS_TEST_CASE(CPU::Tests::flagComp, "Flag Composition", "[Flags]");
METHOD_AS_TEST_CASE(CPU::Tests::flagDecomp, "Flag Decomposition", "[Flags]");
METHOD_AS_TEST_CASE(CPU::Tests::memoryRead, "Memory Read", "[Memory]");
METHOD_AS_TEST_CASE(CPU::Tests::memoryWrite, "Memory Write", "[Memory]");
METHOD_AS_TEST_CASE(CPU::Tests::memoryWait, "Memory Wait", "[Memory]");
METHOD_AS_TEST_CASE(CPU::Tests::verifyTestMemory, "Verify Test Memory", "[Memory]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_r8_r8, "Instruction: ld r8, r8", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_r8_n8, "Instruction: ld r8, n8", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_r16_n16, "Instruction: ld r16, n16", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_ind_hl_r8, "Instruction: ld [hl], r8", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_ind_hl_n8, "Instruction: ld [hl], n8", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_r8_ind_hl, "Instruction: ld r8, [hl]", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_ind_r16_a, "Instruction: ld [r16], a", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_ind_a16_a, "Instruction: ld [a16], a", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ldh_ind_a16_a, "Instruction: ldh [a16], a", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ldh_ind_c_a, "Instruction: ldh [c], a", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_a_ind_r16, "Instruction: ld a, [r16]", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_a_ind_a16, "Instruction: ld a, [a16]", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ldh_a_ind_a16, "Instruction: ldh a, [a16]", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ldh_a_ind_c, "Instruction: ldh a, [c]", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_ind_hli_a, "Instruction: ld [hl+], a", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_ind_hld_a, "Instruction: ld [hl-], a", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_a_ind_hli, "Instruction: ld a, [hl+]", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_ld_a_ind_hld, "Instruction: ld a, [hl-]", "[Load][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instructionGroupAdc, "Instruction Group: adc", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instructionGroupAdd, "Instruction Group: add", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instructionGroupCp, "Instruction Group: cp", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instructionGroupDec, "Instruction Group: dec", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instructionGroupInc, "Instruction Group: inc", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instructionGroupSbc, "Instruction Group: sbc", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instructionGroupSub, "Instruction Group: sub", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_add_hl_r16, "Instruction: add hl, r16", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_dec_r16, "Instruction: dec r16", "[ALU][Instruction]");
METHOD_AS_TEST_CASE(CPU::Tests::instruction_inc_r16, "Instruction: inc r16", "[ALU][Instruction]");

}