#include "emulator.h"
#include "llil_visitor.h"

EmulatorState::EmulatorState(BinaryNinja::BinaryView* bv)
{
	this->log = BinaryNinja::LogRegistry::GetLogger(plugin_name);

	for (const BinaryNinja::Ref<BinaryNinja::Segment>& segment : bv->GetSegments()) {
		this->memory.push_back({ new uint8_t[segment->GetLength()], segment->GetStart(), segment->GetEnd() });
		this->log->LogDebug("Start: 0x%llx - End: 0x%llx", segment->GetStart(), segment->GetEnd(), segment->GetFlags());
	}
	this->bv = bv;
};

void EmulatorState::printCallstack()
{
	const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);
	log->LogDebug("Emulator Callstack Dump\n");

	for (int i = 0; !this->callstack.empty(); i++) {
		auto currFrame = this->callstack.top();

		// Get function name
		const std::string funcName = currFrame.current_function->GetFunction()->GetSymbol()->GetFullName();

		// Get address
		const uint64_t address = currFrame.current_function->GetExpr(currFrame.curr_instr_idx).address;

		log->LogInfo("\t[%d] Name: %s | 0x%llx", i, funcName.c_str(), address);
		this->callstack.pop();
	}
}

EmulatorState::~EmulatorState()
{
	for (const auto& segment : this->memory) {
		delete[] segment.mem;
	}
	this->bv->GetFile()->Close();
	BNShutdown();
}

void EmulatorState::emulate_llil(const Ref<LowLevelILFunction>& llil_func)
{
	const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);
	this->call_function(llil_func->GetFunction()->GetStart(), -1);

	ret_val ret;
	// Fetch instructions and begin execution
	do {
		// Fetch IL instruction
		auto curr_function = this->callstack.top();
		const uint64_t instr_idx = curr_function.curr_instr_idx;
		const LowLevelILInstruction instr = curr_function.current_function->GetInstruction(instr_idx);

		auto sym = this->bv->GetSymbolByAddress(curr_function.current_function->GetCurrentAddress());
		log->LogDebug("Emulating Instr @ 0x%llx", instr.address);
		//  Start visitor
		ret = this->visit(&instr);

		if (this->callstack.empty())
			break;

		this->callstack.top().curr_instr_idx += 1;

	} while (this->callstack.top().curr_instr_idx < this->callstack.top().current_function->GetInstructionCount());
	log->LogInfo("Return value is %d", ret.value);
}

bool EmulatorState::isFunctionThunk(uint64_t address) const
{
	// Check if the address belongs to a section in the bv that had read-only permissions
	auto sections = this->bv->GetSectionsAt(address);
	for (const auto& section : sections) {
		if (section->GetSemantics() == ReadOnlyCodeSectionSemantics)
			return false;
	}
	return true;
}

void EmulatorState::call_function(uint64_t func_addr, uint64_t retInstrIdx)
{
	const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);
	const auto func = this->bv->GetAnalysisFunction(this->bv->GetDefaultPlatform(), func_addr);
	auto llil_func = func->GetLowLevelIL();
	this->callstack.emplace(stackFrame { .current_function = llil_func, .curr_instr_idx = retInstrIdx });
}

void EmulatorState::return_from_function()
{
	const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);

	auto frame = this->callstack.top();
	this->callstack.pop();
	return;
}

ret_val EmulatorState::visit(const struct LowLevelILInstruction* instr)
{
	const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);
	log->LogDebug("\tVisiting LLIL_OP [%d] @ 0x%llx", instr->operation, instr->address);

	switch (instr->operation) {
		case LLIL_SET_REG: return visit_LLIL_SET_REG(this, instr);
		case LLIL_SET_REG_SPLIT: return visit_LLIL_SET_REG_SPLIT(this, instr);
		case LLIL_SET_FLAG: return visit_LLIL_SET_FLAG(this, instr);
		case LLIL_LOAD: return visit_LLIL_LOAD(this, instr);
		case LLIL_STORE: return visit_LLIL_STORE(this, instr);
		case LLIL_PUSH: return visit_LLIL_PUSH(this, instr);
		case LLIL_POP: return visit_LLIL_POP(this, instr);
		case LLIL_REG: return visit_LLIL_REG(this, instr);
		case LLIL_REG_SPLIT: return visit_LLIL_REG_SPLIT(this, instr);
		case LLIL_CONST: return visit_LLIL_CONST(this, instr);
		case LLIL_CONST_PTR: return visit_LLIL_CONST_PTR(this, instr);
		case LLIL_FLAG: return visit_LLIL_FLAG(this, instr);
		case LLIL_ADD: return visit_LLIL_ADD(this, instr);
		case LLIL_SUB: return visit_LLIL_SUB(this, instr);
		case LLIL_SBB: return visit_LLIL_SBB(this, instr);
		case LLIL_AND: return visit_LLIL_AND(this, instr);
		case LLIL_OR: return visit_LLIL_OR(this, instr);
		case LLIL_XOR: return visit_LLIL_XOR(this, instr);
		case LLIL_LSL: return visit_LLIL_LSL(this, instr);
		case LLIL_LSR: return visit_LLIL_LSR(this, instr);
		case LLIL_ASR: return visit_LLIL_ASR(this, instr);
		case LLIL_MUL: return visit_LLIL_MUL(this, instr);
		case LLIL_DIVU_DP: return visit_LLIL_DIVU_DP(this, instr);
		case LLIL_DIVS_DP: return visit_LLIL_DIVS_DP(this, instr);
		case LLIL_MODU_DP: return visit_LLIL_MODU_DP(this, instr);
		case LLIL_MODS_DP: return visit_LLIL_MODS_DP(this, instr);
		case LLIL_NEG: return visit_LLIL_NEG(this, instr);
		case LLIL_SX: return visit_LLIL_SX(this, instr);
		case LLIL_ZX: return visit_LLIL_ZX(this, instr);
		case LLIL_LOW_PART: return visit_LLIL_LOW_PART(this, instr);
		case LLIL_JUMP: return visit_LLIL_JUMP(this, instr);
		case LLIL_JUMP_TO: return visit_LLIL_JUMP_TO(this, instr);
		case LLIL_CALL: return visit_LLIL_CALL(this, instr);
		case LLIL_TAILCALL: return visit_LLIL_TAILCALL(this, instr);
		case LLIL_RET: return visit_LLIL_RET(this, instr);
		case LLIL_NORET: return visit_LLIL_NORET(this, instr);
		case LLIL_IF: return visit_LLIL_IF(this, instr);
		case LLIL_GOTO: return visit_LLIL_GOTO(this, instr);
		case LLIL_CMP_E: return visit_LLIL_CMP_E(this, instr);
		case LLIL_CMP_NE: return visit_LLIL_CMP_NE(this, instr);
		case LLIL_CMP_SLT: return visit_LLIL_CMP_SLT(this, instr);
		case LLIL_CMP_ULT: return visit_LLIL_CMP_ULT(this, instr);
		case LLIL_CMP_SLE: return visit_LLIL_CMP_SLE(this, instr);
		case LLIL_CMP_ULE: return visit_LLIL_CMP_ULE(this, instr);
		case LLIL_CMP_SGE: return visit_LLIL_CMP_SGE(this, instr);
		case LLIL_CMP_UGE: return visit_LLIL_CMP_UGE(this, instr);
		case LLIL_CMP_SGT: return visit_LLIL_CMP_SGT(this, instr);
		case LLIL_CMP_UGT: return visit_LLIL_CMP_UGT(this, instr);
		case LLIL_TEST_BIT: return visit_LLIL_TEST_BIT(this, instr);
		case LLIL_INTRINSIC: return visit_LLIL_INTRINSIC(this, instr);
		case LLIL_UNDEF: return visit_LLIL_UNDEF(this, instr);
		default: {
			const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);
			log->LogError("[*] Unhandled LLIL_OP [%d] @ 0x%llx\n", instr->operation, instr->address);
			return { .discriminator = UNIMPL };
		} break;
	}
}
