#include "emulator.hpp"

uint64_t visit_LLIL_SET_REG(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto reg = instr->GetDestRegister<LLIL_SET_REG>();
	const auto src = instr->GetSourceExpr<LLIL_SET_REG>();
	const auto value = emu->visit(&src);
	emu->setRegister(reg, value);
	return -1;
}

uint64_t visit_LLIL_SET_REG_SPLIT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SET_REG_SPLIT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_SET_FLAG(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SET_FLAG @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_LOAD(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto src = instr->GetSourceExpr<LLIL_LOAD>();
	const auto address = emu->visit(&src);
	return emu->readMemory(address, instr->size);
}

uint64_t visit_LLIL_STORE(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto src = instr->GetSourceExpr<LLIL_STORE>();
	const auto dest = instr->GetDestExpr<LLIL_STORE>();
	const auto value = emu->visit(&src);
	const auto address = emu->visit(&dest);
	emu->writeMemory(address, value, instr->size);
	return -1;
}

uint64_t visit_LLIL_PUSH(Emulator* emu, const LowLevelILInstruction* instr)
{
	const stackFrame* sf = emu->getTopCallstack();
	const auto srcExpr = instr->GetSourceExpr<LLIL_PUSH>();
	const auto pushVal = emu->visit(&srcExpr);

	// get stack pointer
	const auto arch = emu->getBinaryView()->GetDefaultArchitecture();
	const auto sp_idx = arch->GetStackPointerRegister();
	const auto sp = emu->getRegister(sp_idx);
	switch (srcExpr.size) {
		case 1:
			*(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint8_t>(pushVal);
			break;
		case 2:
			*reinterpret_cast<uint16_t *>(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint16_t>(pushVal);
			break;
		case 4:
			*reinterpret_cast<uint32_t *>(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint32_t>(pushVal);
			break;
		case 8:
			*reinterpret_cast<uint64_t *>(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint64_t>(pushVal);
			break;
		default:
			break;
	}
	// Adjust SP
	const auto sp_val = emu->getRegister(sp_idx);
	emu->setRegister(sp_idx, sp_val - srcExpr.size);
	return 0;
}

uint64_t visit_LLIL_POP(Emulator* emu, const LowLevelILInstruction* instr)
{
	const stackFrame* sf = emu->getTopCallstack();

	// get stack pointer
	const auto arch = emu->getBinaryView()->GetDefaultArchitecture();
	const auto sp_idx = arch->GetStackPointerRegister();

	uint64_t ret_val = 0;
	switch (instr->size) {
		case 1: {
			const auto stack_ptr = sf->stack;
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		case 2: {
			const auto stack_ptr = reinterpret_cast<uint16_t *>(sf->stack);
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		case 4: {
			const auto stack_ptr = reinterpret_cast<uint32_t *>(sf->stack);
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		case 8: {
			const auto stack_ptr = reinterpret_cast<uint64_t *>(sf->stack);
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		default:
			break;
	}
	// Adjust SP
	const auto sp_val = emu->getRegister(sp_idx);
	emu->setRegister(sp_idx, sp_val + instr->size);
	return ret_val;
}

uint64_t visit_LLIL_REG(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto regIdx = instr->GetSourceRegister<LLIL_REG>();
	return emu->getRegister(regIdx);
}

uint64_t visit_LLIL_REG_SPLIT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_REG_SPLIT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CONST(const LowLevelILInstruction* instr)
{
	const auto constVal = instr->GetConstant();
	return static_cast<uint64_t>(constVal);
}

uint64_t visit_LLIL_CONST_PTR(const LowLevelILInstruction* instr)
{
	const auto constant = instr->GetConstant<LLIL_CONST_PTR>();
	return static_cast<uint64_t>(constant);
}

uint64_t visit_LLIL_FLAG(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_FLAG @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_ADD(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto lhs_instr = instr->GetLeftExpr<LLIL_ADD>();
	const auto rhs_instr = instr->GetRightExpr<LLIL_ADD>();
	const auto lhs = emu->visit(&lhs_instr);
	const auto rhs = emu->visit(&rhs_instr);
	return lhs + rhs;
}

uint64_t visit_LLIL_SUB(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto lhs_instr = instr->GetLeftExpr<LLIL_SUB>();
	const auto rhs_instr = instr->GetRightExpr<LLIL_SUB>();
	const auto lhs = emu->visit(&lhs_instr);
	const auto rhs = emu->visit(&rhs_instr);
	return lhs - rhs;
}

uint64_t visit_LLIL_SBB(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SBB @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_AND(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto lhs_instr = instr->GetLeftExpr<LLIL_AND>();
	const auto rhs_instr = instr->GetRightExpr<LLIL_AND>();
	const auto lhs = emu->visit(&lhs_instr);
	const auto rhs = emu->visit(&rhs_instr);
	return lhs & rhs;
}

uint64_t visit_LLIL_OR(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto lhs_instr = instr->GetLeftExpr<LLIL_OR>();
	const auto rhs_instr = instr->GetRightExpr<LLIL_OR>();
	const auto lhs = emu->visit(&lhs_instr);
	const auto rhs = emu->visit(&rhs_instr);
	return lhs | rhs;
}

uint64_t visit_LLIL_XOR(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto lhs_instr = instr->GetLeftExpr<LLIL_OR>();
	const auto rhs_instr = instr->GetRightExpr<LLIL_OR>();
	const auto lhs = emu->visit(&lhs_instr);
	const auto rhs = emu->visit(&rhs_instr);
	return lhs ^ rhs;
}

uint64_t visit_LLIL_LSL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LSL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_LSR(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LSR @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_ASR(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_ASR @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_MUL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MUL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_DIVU_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_DIVU_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_DIVS_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_DIVS_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_MODU_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MODU_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_MODS_DP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_MODS_DP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_NEG(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_NEG @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_SX(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SX @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_ZX(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_ZX @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_LOW_PART(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_LOW_PART @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_JUMP(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_JUMP @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_JUMP_TO(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_JUMP_TO @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CALL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CALL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_TAILCALL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TAILCALL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_RET(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->return_from_function();
	return 0;
}

uint64_t visit_LLIL_NORET(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_NORET @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_IF(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_IF @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_GOTO(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_GOTO @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_E(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_E @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_NE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_NE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_SLT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SLT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_ULT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_ULT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_SLE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SLE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_ULE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_ULE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_SGE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SGE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_UGE(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_UGE @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_SGT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_SGT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CMP_UGT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_CMP_UGT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_TEST_BIT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TEST_BIT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_INTRINSIC(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_INTRINSIC @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_UNDEF(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_UNDEF @ 0x%08lx is unimplemented", instr->address);
	return -1;
}