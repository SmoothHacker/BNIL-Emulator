#include "emulator.h"

namespace {
ret_val visit_LLIL_SET_REG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_SET_REG_SPLIT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_SET_FLAG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_LOAD(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_STORE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_PUSH(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_POP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_REG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_REG_SPLIT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CONST(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CONST_PTR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_FLAG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_ADD(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_SUB(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_SBB(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_AND(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_OR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_XOR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_LSL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_LSR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_ASR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

static ret_val visit_LLIL_MUL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_DIVU_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_DIVS_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_MODU_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_MODS_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_NEG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_SX(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_ZX(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_LOW_PART(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_JUMP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_JUMP_TO(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CALL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_TAILCALL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_RET(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_NORET(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_IF(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_GOTO(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_E(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_NE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_SLT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_ULT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_SLE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_ULE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_SGE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_UGE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_SGT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_CMP_UGT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_TEST_BIT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_INTRINSIC(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};

ret_val visit_LLIL_UNDEF(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("Instruction %d @ 0x%lx is unimplemented", instr->operation, instr->address);
	return ret_val { .discriminator = UNIMPL };
};
}