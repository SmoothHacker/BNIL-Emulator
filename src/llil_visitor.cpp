#include "emulator.hpp"

ret_val visit_LLIL_SET_REG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SET_REG @ 0x%lx is unimplemented", instr->address);
	const auto reg = instr->GetSourceRegister<LLIL_SET_REG>();
	const auto src = instr->GetDestExpr<LLIL_SET_REG>();
	const auto value = emu->visit(&src);
	emu->setRegister(reg, value);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_SET_REG_SPLIT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SET_REG_SPLIT @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_SET_FLAG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SET_FLAG @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_LOAD(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LOAD @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_STORE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_STORE @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_PUSH(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	stackFrame sf = emu->getTopCallstack();
	const auto srcExpr = instr->GetSourceExpr<LLIL_PUSH>();
	ret_val const pushVal = emu->visit(&srcExpr);
	sf.stack.emplace(pushVal.val_u64);
	return { .discriminator = UINT32_T, .value = 0 };
}

ret_val visit_LLIL_POP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_POP @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_REG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_REG @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_REG_SPLIT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_REG_SPLIT @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CONST(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CONST @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CONST_PTR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CONST_PTR @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_FLAG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_FLAG @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_ADD(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_ADD @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_SUB(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SUB @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_SBB(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SBB @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_AND(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_AND @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_OR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_OR @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_XOR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_XOR @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_LSL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LSL @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_LSR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LSR @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_ASR(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_ASR @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_MUL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MUL @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_DIVU_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_DIVU_DP @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_DIVS_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_DIVS_DP @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_MODU_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MODU_DP @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_MODS_DP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MODS_DP @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_NEG(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_NEG @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_SX(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SX @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_ZX(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_ZX @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_LOW_PART(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LOW_PART @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_JUMP(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_JUMP @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_JUMP_TO(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_JUMP_TO @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CALL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CALL @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_TAILCALL(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TAILCALL @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_RET(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_RET @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_NORET(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_NORET @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_IF(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_IF @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_GOTO(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_GOTO @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_E(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_E @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_NE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_NE @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_SLT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SLT @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_ULT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_ULT @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_SLE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SLE @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_ULE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_ULE @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_SGE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SGE @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_UGE(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_UGE @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_SGT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SGT @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_CMP_UGT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_UGT @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_TEST_BIT(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TEST_BIT @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_INTRINSIC(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_INTRINSIC @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}

ret_val visit_LLIL_UNDEF(EmulatorState* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_UNDEF @ 0x%lx is unimplemented", instr->address);
	return ret_val { .discriminator = UNIMPL };
}