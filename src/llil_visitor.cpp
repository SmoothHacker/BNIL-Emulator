#include "emulator.hpp"

uint64_t BinaryVisitorHelper(Emulator* emu, const LowLevelILInstruction* instr, const std::function<bool(uint64_t, uint64_t)>& condition)
{
	const auto lhs_instr = instr->GetLeftExpr();
	const auto rhs_instr = instr->GetRightExpr();
	const auto lhs = emu->visit(&lhs_instr);
	const auto rhs = emu->visit(&rhs_instr);
	return condition(lhs, rhs);
}

uint64_t visit_LLIL_SET_REG(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto reg = instr->GetDestRegister<LLIL_SET_REG>();
	const auto src = instr->GetSourceExpr<LLIL_SET_REG>();
	const auto value = emu->visit(&src);
	emu->set_register(reg, value);
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
	return emu->read_memory(address, instr->size);
}

uint64_t visit_LLIL_STORE(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto src = instr->GetSourceExpr<LLIL_STORE>();
	const auto dest = instr->GetDestExpr<LLIL_STORE>();
	const auto value = emu->visit(&src);
	const auto address = emu->visit(&dest);
	emu->write_memory(address, value, instr->size);
	return -1;
}

uint64_t visit_LLIL_PUSH(Emulator* emu, const LowLevelILInstruction* instr)
{
	const stackFrame* sf = emu->peek_callstack();
	const auto srcExpr = instr->GetSourceExpr<LLIL_PUSH>();
	const auto pushVal = emu->visit(&srcExpr);

	// get stack pointer
	const auto arch = emu->get_binary_view()->GetDefaultArchitecture();
	const auto sp_idx = arch->GetStackPointerRegister();
	const auto sp = emu->get_register(sp_idx);
	switch (srcExpr.size) {
		case 1:
			*(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint8_t>(pushVal);
			break;
		case 2:
			*reinterpret_cast<uint16_t*>(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint16_t>(pushVal);
			break;
		case 4:
			*reinterpret_cast<uint32_t*>(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint32_t>(pushVal);
			break;
		case 8:
			*reinterpret_cast<uint64_t*>(sf->stack + (static_cast<uint64_t>(sp) - sf->sf_base)) = static_cast<uint64_t>(pushVal);
			break;
		default:
			break;
	}
	// Adjust SP
	const auto sp_val = emu->get_register(sp_idx);
	emu->set_register(sp_idx, sp_val - srcExpr.size);
	return 0;
}

uint64_t visit_LLIL_POP(Emulator* emu, const LowLevelILInstruction* instr)
{
	const stackFrame* sf = emu->peek_callstack();

	// get stack pointer
	const auto arch = emu->get_binary_view()->GetDefaultArchitecture();
	const auto sp_idx = arch->GetStackPointerRegister();

	uint64_t ret_val = 0;
	switch (instr->size) {
		case 1: {
			const auto stack_ptr = sf->stack;
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		case 2: {
			const auto stack_ptr = reinterpret_cast<uint16_t*>(sf->stack);
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		case 4: {
			const auto stack_ptr = reinterpret_cast<uint32_t*>(sf->stack);
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		case 8: {
			const auto stack_ptr = reinterpret_cast<uint64_t*>(sf->stack);
			ret_val = *stack_ptr;
			*stack_ptr = 0;
		} break;
		default:
			break;
	}
	// Adjust SP
	const auto sp_val = emu->get_register(sp_idx);
	emu->set_register(sp_idx, sp_val + instr->size);
	return ret_val;
}

uint64_t visit_LLIL_REG(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto regIdx = instr->GetSourceRegister<LLIL_REG>();
	return emu->get_register(regIdx);
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
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs + rhs;
	});
}

uint64_t visit_LLIL_SUB(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs - rhs;
	});
}

uint64_t visit_LLIL_SBB(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_SBB @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_AND(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs & rhs;
	});
}

uint64_t visit_LLIL_OR(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs | rhs;
	});
}

uint64_t visit_LLIL_XOR(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs ^ rhs;
	});
}

uint64_t visit_LLIL_LSL(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs << rhs;
	});
}

uint64_t visit_LLIL_LSR(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs >> rhs;
	});
}

uint64_t visit_LLIL_ASR(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return static_cast<uint64_t>(static_cast<int64_t>(lhs) >> static_cast<int64_t>(rhs));
	});
}

uint64_t visit_LLIL_MUL(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs * rhs;
	});
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
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) -> uint64_t {
		return static_cast<int64_t>(lhs) % static_cast<int64_t>(rhs);
	});
}

uint64_t visit_LLIL_NEG(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto source_instr = instr->GetSourceExpr<LLIL_NEG>();
	const auto src = emu->visit(&source_instr);
	return 0 - src;
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
	// Check if we have to do something with size for this
	const auto src_expr = instr->GetSourceExpr<LLIL_LOW_PART>();
	emu->visit(&src_expr);
	return -1;
}

/*
 * Both instructions perform jump operations. Only JUMP_TO has defined destinations
 */
uint64_t visit_LLIL_JUMP(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto dest_instr = instr->GetDestExpr<LLIL_JUMP>();
	auto jump_target = emu->visit(&dest_instr);

	// Check if jump_target is in new function
	return -1;
}

uint64_t visit_LLIL_JUMP_TO(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_JUMP_TO @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_CALL(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto dest_instr = instr->GetDestExpr<LLIL_JUMP>();
	const auto jump_target = emu->visit(&dest_instr);
	emu->call_function(jump_target, instr->exprIndex + 1);
	return -1;
}

uint64_t visit_LLIL_TAILCALL(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TAILCALL @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_RET(Emulator* emu)
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
	const auto condition_instr = instr->GetConditionExpr<LLIL_IF>();
	if (emu->visit(&condition_instr))
		return instr->GetTrueTarget<LLIL_IF>();
	return instr->GetFalseTarget<LLIL_IF>(); // return instruction index
}

uint64_t visit_LLIL_GOTO(const LowLevelILInstruction* instr)
{
	const auto jump_target = instr->GetTarget<LLIL_GOTO>();

	// check if jump target is in the current function
	return jump_target;
}

uint64_t visit_LLIL_CMP_E(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs == rhs;
	});
}

uint64_t visit_LLIL_CMP_NE(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs != rhs;
	});
}

uint64_t visit_LLIL_CMP_SLT(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return static_cast<int64_t>(lhs) < static_cast<int64_t>(rhs);
	});
}

uint64_t visit_LLIL_CMP_ULT(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs < rhs;
	});
}

uint64_t visit_LLIL_CMP_SLE(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return static_cast<int64_t>(lhs) <= static_cast<int64_t>(rhs);
	});
}

uint64_t visit_LLIL_CMP_ULE(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs <= rhs;
	});
}

uint64_t visit_LLIL_CMP_SGE(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return static_cast<int64_t>(lhs) >= static_cast<int64_t>(rhs);
	});
}

uint64_t visit_LLIL_CMP_UGE(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs >= rhs;
	});
}

uint64_t visit_LLIL_CMP_SGT(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return static_cast<int64_t>(lhs) > static_cast<int64_t>(rhs);
	});
}

uint64_t visit_LLIL_CMP_UGT(Emulator* emu, const LowLevelILInstruction* instr)
{
	return BinaryVisitorHelper(emu, instr, [](const auto lhs, const auto rhs) {
		return lhs > rhs;
	});
}

uint64_t visit_LLIL_TEST_BIT(Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_TEST_BIT @ 0x%08lx is unimplemented", instr->address);
	return -1;
}

uint64_t visit_LLIL_INTRINSIC(Emulator* emu, const LowLevelILInstruction* instr)
{
	const auto handler = emu->get_intrinsic_handler(instr->GetIntrinsic<LLIL_INTRINSIC>());
	handler(emu, instr);
	return 0; // Examine if we have to return outputs to work better with the dataflor APIs
}

uint64_t visit_LLIL_UNDEF(const Emulator* emu, const LowLevelILInstruction* instr)
{
	emu->log->LogError("LLIL_UNDEF Encountered. Unknown emulation side effects. Treating instruction as a NOP", instr->address);
	return 0;
}