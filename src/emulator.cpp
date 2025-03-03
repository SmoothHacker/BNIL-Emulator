#include "emulator.hpp"
#include "llil_visitor.hpp"

void EmulatorState::setRegister(const uint32_t reg, const ret_val value)
{
	this->regs[reg] = value.value;
}

Ref<BinaryView> EmulatorState::getBinaryView()
{
	return this->bv;
}

void EmulatorState::dumpRegisters()
{
	const auto arch = this->bv->GetDefaultArchitecture();
	this->log->LogInfo("Dumping registers:");
	for (const auto& [reg, value] : this->regs) {
		this->log->LogInfo("\t%s : 0x%lx", arch->GetRegisterName(reg).c_str(), this->regs[reg]);
	}
}

EmulatorState::EmulatorState(BinaryView* bv)
{
	this->log = LogRegistry::GetLogger(plugin_name);

	for (const Ref<Segment>& segment : bv->GetSegments()) {
		this->memory.push_back({ new uint8_t[segment->GetLength()], segment->GetStart(), segment->GetEnd() });
		this->log->LogDebug("Start: 0x%llx - End: 0x%llx", segment->GetStart(), segment->GetEnd(), segment->GetFlags());
	}
	this->bv = bv;
}

void EmulatorState::printCallstack()
{
	const auto log = this->log;
	log->LogDebug("Emulator Callstack Dump\n");

	for (int i = 0; !this->callstack.empty(); i++) {
		const auto currFrame = this->callstack.top();

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
	const auto log = this->log;
	this->call_function(llil_func->GetFunction()->GetStart(), 0);

	ret_val ret;
	// Fetch instructions and begin execution
	do {
		// Fetch IL instruction
		const auto curr_function = this->callstack.top();
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

bool EmulatorState::isFunctionThunk(const uint64_t address) const
{
	// Check if the address belongs to a section in the bv that had read-only permissions
	const auto sections = this->bv->GetSectionsAt(address);
	for (const auto& section : sections) {
		if (section->GetSemantics() == ReadOnlyDataSectionSemantics)
			return false;
	}
	return true;
}

void EmulatorState::call_function(const uint64_t func_addr, const uint64_t retInstrIdx)
{
	const auto log = this->log;
	const auto func = this->bv->GetAnalysisFunction(this->bv->GetDefaultPlatform(), func_addr);
	const auto llil_func = func->GetLowLevelIL();
	this->callstack.emplace(stackFrame { .current_function = llil_func, .curr_instr_idx = retInstrIdx });
}

void EmulatorState::return_from_function()
{
	const auto log = this->log;

	auto frame = this->callstack.top();
	this->callstack.pop();
}

ret_val EmulatorState::visit(const LowLevelILInstruction* instr)
{
	const auto log = this->log;
	log->LogDebug("\tVisiting LLIL_OP [%d] @ 0x%llx", instr->operation, instr->address);

	ret_val ret;
	switch (instr->operation) {
		case LLIL_SET_REG: ret = visit_LLIL_SET_REG(this, instr); break;
		case LLIL_SET_REG_SPLIT: ret = visit_LLIL_SET_REG_SPLIT(this, instr); break;
		case LLIL_SET_FLAG: ret = visit_LLIL_SET_FLAG(this, instr); break;
		case LLIL_LOAD: ret = visit_LLIL_LOAD(this, instr); break;
		case LLIL_STORE: ret = visit_LLIL_STORE(this, instr); break;
		case LLIL_PUSH: ret = visit_LLIL_PUSH(this, instr); break;
		case LLIL_POP: ret = visit_LLIL_POP(this, instr); break;
		case LLIL_REG: ret = visit_LLIL_REG(this, instr); break;
		case LLIL_REG_SPLIT: ret = visit_LLIL_REG_SPLIT(this, instr); break;
		case LLIL_CONST: ret = visit_LLIL_CONST(this, instr); break;
		case LLIL_CONST_PTR: ret = visit_LLIL_CONST_PTR(this, instr); break;
		case LLIL_FLAG: ret = visit_LLIL_FLAG(this, instr); break;
		case LLIL_ADD: ret = visit_LLIL_ADD(this, instr); break;
		case LLIL_SUB: ret = visit_LLIL_SUB(this, instr); break;
		case LLIL_SBB: ret = visit_LLIL_SBB(this, instr); break;
		case LLIL_AND: ret = visit_LLIL_AND(this, instr); break;
		case LLIL_OR: ret = visit_LLIL_OR(this, instr); break;
		case LLIL_XOR: ret = visit_LLIL_XOR(this, instr); break;
		case LLIL_LSL: ret = visit_LLIL_LSL(this, instr); break;
		case LLIL_LSR: ret = visit_LLIL_LSR(this, instr); break;
		case LLIL_ASR: ret = visit_LLIL_ASR(this, instr); break;
		case LLIL_MUL: ret = visit_LLIL_MUL(this, instr); break;
		case LLIL_DIVU_DP: ret = visit_LLIL_DIVU_DP(this, instr); break;
		case LLIL_DIVS_DP: ret = visit_LLIL_DIVS_DP(this, instr); break;
		case LLIL_MODU_DP: ret = visit_LLIL_MODU_DP(this, instr); break;
		case LLIL_MODS_DP: ret = visit_LLIL_MODS_DP(this, instr); break;
		case LLIL_NEG: ret = visit_LLIL_NEG(this, instr); break;
		case LLIL_SX: ret = visit_LLIL_SX(this, instr); break;
		case LLIL_ZX: ret = visit_LLIL_ZX(this, instr); break;
		case LLIL_LOW_PART: ret = visit_LLIL_LOW_PART(this, instr); break;
		case LLIL_JUMP: ret = visit_LLIL_JUMP(this, instr); break;
		case LLIL_JUMP_TO: ret = visit_LLIL_JUMP_TO(this, instr); break;
		case LLIL_CALL: ret = visit_LLIL_CALL(this, instr); break;
		case LLIL_TAILCALL: ret = visit_LLIL_TAILCALL(this, instr); break;
		case LLIL_RET: ret = visit_LLIL_RET(this, instr); break;
		case LLIL_NORET: ret = visit_LLIL_NORET(this, instr); break;
		case LLIL_IF: ret = visit_LLIL_IF(this, instr); break;
		case LLIL_GOTO: ret = visit_LLIL_GOTO(this, instr); break;
		case LLIL_CMP_E: ret = visit_LLIL_CMP_E(this, instr); break;
		case LLIL_CMP_NE: ret = visit_LLIL_CMP_NE(this, instr); break;
		case LLIL_CMP_SLT: ret = visit_LLIL_CMP_SLT(this, instr); break;
		case LLIL_CMP_ULT: ret = visit_LLIL_CMP_ULT(this, instr); break;
		case LLIL_CMP_SLE: ret = visit_LLIL_CMP_SLE(this, instr); break;
		case LLIL_CMP_ULE: ret = visit_LLIL_CMP_ULE(this, instr); break;
		case LLIL_CMP_SGE: ret = visit_LLIL_CMP_SGE(this, instr); break;
		case LLIL_CMP_UGE: ret = visit_LLIL_CMP_UGE(this, instr); break;
		case LLIL_CMP_SGT: ret = visit_LLIL_CMP_SGT(this, instr); break;
		case LLIL_CMP_UGT: ret = visit_LLIL_CMP_UGT(this, instr); break;
		case LLIL_TEST_BIT: ret = visit_LLIL_TEST_BIT(this, instr); break;
		case LLIL_INTRINSIC: ret = visit_LLIL_INTRINSIC(this, instr); break;
		case LLIL_UNDEF: ret = visit_LLIL_UNDEF(this, instr); break;
		default: {
			ret = { .discriminator = UNIMPL };
		}
	}
	if (ret.discriminator == UNIMPL) {
		log->LogError("[*] Unhandled LLIL_OP [%d] @ 0x%llx\n", instr->operation, instr->address);
		exit(-1);
	}
	return ret;
}
