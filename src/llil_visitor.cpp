#include "emulator.hpp"

double visit_LLIL_SET_REG(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto reg = instr->GetDestRegister<LLIL_SET_REG>();
	const auto src = instr->GetSourceExpr<LLIL_SET_REG>();
	const auto value = emu->visit(&src);
	emu->setRegister(reg, value);
	return -1;
}

double visit_LLIL_SET_REG_SPLIT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SET_REG_SPLIT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_SET_FLAG(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SET_FLAG @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_LOAD(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LOAD @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_STORE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_STORE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_PUSH(Emulator* emu, const LowLevelILInstruction* instr)
{
	const stackFrame sf = emu->getTopCallstack();
	const auto srcExpr = instr->GetSourceExpr<LLIL_PUSH>();
	double const pushVal = emu->visit(&srcExpr);

	// get stack pointer
	const auto arch = emu->getBinaryView()->GetDefaultArchitecture();
	const auto sp = arch->GetStackPointerRegister();
	sf.stack[sp] = pushVal;
	emu->setRegister(sp, sp + arch->GetDefaultIntegerSize());
	return 0;
}

double visit_LLIL_POP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_POP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_REG(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto regIdx = instr->GetSourceRegister<LLIL_REG>();
	return emu->getRegister(regIdx);
}

double visit_LLIL_REG_SPLIT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_REG_SPLIT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CONST(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto constVal = instr->GetConstant();
	return constVal;
}

double visit_LLIL_CONST_PTR(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CONST_PTR @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_FLAG(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_FLAG @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_ADD(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto lhs_instr = instr->GetLeftExpr<LLIL_ADD>();
	const auto rhs_instr = instr->GetRightExpr<LLIL_ADD>();
	const auto lhs = emu->visit(&lhs_instr);
	const auto rhs = emu->visit(&rhs_instr);
	return lhs + rhs;
}

double visit_LLIL_SUB(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SUB @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_SBB(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SBB @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_AND(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_AND @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_OR(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_OR @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_XOR(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_XOR @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_LSL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LSL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_LSR(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LSR @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_ASR(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_ASR @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_MUL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MUL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_DIVU_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_DIVU_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_DIVS_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_DIVS_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_MODU_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MODU_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_MODS_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MODS_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_NEG(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_NEG @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_SX(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SX @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_ZX(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_ZX @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_LOW_PART(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LOW_PART @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_JUMP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_JUMP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_JUMP_TO(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_JUMP_TO @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CALL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CALL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_TAILCALL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TAILCALL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_RET(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->return_from_function();
	return 0;
}

double visit_LLIL_NORET(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_NORET @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_IF(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_IF @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_GOTO(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_GOTO @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_E(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_E @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_NE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_NE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_SLT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SLT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_ULT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_ULT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_SLE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SLE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_ULE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_ULE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_SGE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SGE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_UGE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_UGE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_SGT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SGT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_CMP_UGT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_UGT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_TEST_BIT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TEST_BIT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_INTRINSIC(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_INTRINSIC @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

double visit_LLIL_UNDEF(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_UNDEF @ 0x%08lx is unimplemented", instr->address);
	return -1;
}