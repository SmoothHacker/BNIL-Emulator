#include "emulator.h"
#include "llil_visitor.h"

EmulatorState::EmulatorState(BinaryNinja::BinaryView* bv)
{
	const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);

	for (const BinaryNinja::Ref<BinaryNinja::Segment>& segment : bv->GetSegments()) {
		this->memory.push_back({ new uint8_t[segment->GetLength()], segment->GetStart(), segment->GetEnd() });
		log->LogDebug("Start: 0x%llx - End: 0x%llx", segment->GetStart(), segment->GetEnd(), segment->GetFlags());
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

	// Add all mlil variables to variable map
	auto result_list = this->bv->GetAnalysisFunctionsForAddress(func_addr);
	if (result_list.size() == 0) {
		log->LogError("Unknown Function @ 0x%llx", func_addr);
	}
	const auto &func = result_list.front();
	auto* vars = new std::unordered_map<uint64_t, emuVariable>;
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
		case LLIL_INTRINSIC: return visit_LLIL_INTRINSIC(this, instr);
		case LLIL_TAILCALL: return visit_LLIL_TAILCALL(this, instr);
		case LLIL_NORET: return visit_LLIL_NORET(this, instr);
		case LLIL_JUMP: return visit_LLIL_JUMP(this, instr);
		case LLIL_CALL: return visit_LLIL_CALL(this, instr);
		case LLIL_RET: return visit_LLIL_RET(this, instr);
		case LLIL_UNDEF: return visit_LLIL_UNDEF(this, instr);
		case LLIL_SET_REG: return visit_LLIL_SET_REG(this, instr);
		case LLIL_IF: return visit_LLIL_IF(this, instr);
		case LLIL_GOTO: return visit_LLIL_GOTO(this, instr);
		case LLIL_JUMP_TO: return visit_LLIL_JUMP_TO(this, instr);
		case LLIL_SET_REG_SPLIT: return visit_LLIL_SET_REG_SPLIT(this, instr);
		case LLIL_SET_FLAG: return visit_LLIL_SET_FLAG(this, instr);
		case LLIL_STORE: return visit_LLIL_STORE(this, instr);
		case LLIL_PUSH: return visit_LLIL_PUSH(this, instr);
		default: {
			const auto log = BinaryNinja::LogRegistry::GetLogger(plugin_name);
			log->LogError("[*] Unhandled LLIL_OP [%d] @ 0x%llx\n", instr->operation, instr->address);
		} break;
	}
}
