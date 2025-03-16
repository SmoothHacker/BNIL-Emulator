#include "emulator.hpp"
#include "llil_visitor.hpp"

void Emulator::setRegister(const uint32_t reg, const double value)
{
	this->regs[reg] = value;
}

double Emulator::getRegister(const uint32_t reg)
{
	return this->regs[reg];
}

Ref<BinaryView> Emulator::getBinaryView()
{
	return this->bv;
}

void Emulator::dumpRegisters()
{
	const auto arch = this->bv->GetDefaultArchitecture();
	this->log->LogInfo("Dumping registers:");
	for (const auto& [reg, value] : this->regs) {
		this->log->LogInfo("\t%s : 0x%lx", arch->GetRegisterName(reg).c_str(), value);
	}
}

uint64_t Emulator::readRegister(const uint32_t reg)
{
	return static_cast<uint64_t>(this->regs[reg]);
}

Emulator::Emulator(BinaryView* bv)
{
	this->log = LogRegistry::GetLogger(plugin_name);

	this->log->LogDebug("Emulator Memory Map: ");
	for (const Ref<Segment>& segment : bv->GetSegments()) {
		this->memory.push_back({ new uint8_t[segment->GetLength()], segment->GetStart(), segment->GetEnd() });
		this->log->LogDebug("0x%08llx <-> 0x%08llx", segment->GetStart(), segment->GetEnd());
	}
	this->bv = bv;
}

void Emulator::printCallstack()
{
	const auto log = this->log;
	log->LogDebug("Emulator Callstack Dump\n");

	for (int i = 0; !this->callstack.empty(); i++) {
		const auto& currFrame = this->callstack.top();

		// Get function name
		const std::string funcName = currFrame.llilFunction->GetFunction()->GetSymbol()->GetFullName();

		// Get address
		const uint64_t address = currFrame.llilFunction->GetExpr(currFrame.curInstrIdx).address;

		log->LogInfo("\t[%d] Name: %s | 0x%08llx", i, funcName.c_str(), address);
		this->callstack.pop();
	}
}

Emulator::~Emulator()
{
	for (const auto& segment : this->memory) {
		delete[] segment.mem;
	}
	this->bv->GetFile()->Close();
	BNShutdown();
}

void Emulator::emulate_llil(const Ref<LowLevelILFunction>& llil_func)
{
	const auto& log = this->log;
	this->call_function(llil_func->GetFunction()->GetStart(), 0);

	// Fetch instructions and begin execution
	do {
		// Fetch IL instruction
		const auto& sf = this->callstack.top();
		LowLevelILInstruction const instr = (*sf.llilFunction)[sf.curInstrIdx];

		auto sym = this->bv->GetSymbolByAddress(sf.llilFunction->GetCurrentAddress());
		log->LogDebug("Emulating Instr @ idx [%d] addr: 0x%08llx", sf.curInstrIdx, instr.address);
		//  Start visitor
		this->visit(&instr);

		if (this->callstack.empty())
			break;

		this->callstack.top().curInstrIdx += 1;

	} while (this->callstack.top().curInstrIdx < this->callstack.top().llilFunction->GetInstructionCount());
}

bool Emulator::isFunctionThunk(const uint64_t address) const
{
	// Check if the address belongs to a section in the bv that had read-only permissions
	for (const auto sections = this->bv->GetSectionsAt(address); const auto& section : sections) {
		if (section->GetSemantics() == ReadOnlyDataSectionSemantics)
			return false;
	}
	return true;
}

uint64_t Emulator::readMemory(const uint64_t address, const uint8_t size) const
{
	const mem_segment* seg = nullptr;
	for (const auto& segment : this->memory) {
		if (segment.startAddr <= address && segment.endAddr > address) {
			seg = &segment;
			break;
		}
	}
	if (seg == nullptr) {
		this->log->LogError("Address 0x%08llx not found\n", address);
		exit(-1); // figure out a better way to throw an error
	}

	this->log->LogDebug("Reading value from 0x%08llx", address);
	if (size == 1)
		return *(seg->mem + (address - seg->startAddr));
	if (size == 2)
		return *reinterpret_cast<uint16_t*>(seg->mem + (address - seg->startAddr));
	if (size == 4)
		return *reinterpret_cast<uint32_t*>(seg->mem + (address - seg->startAddr));

	// size is assumed to be uint64_t
	return *reinterpret_cast<uint64_t*>(seg->mem + (address - seg->startAddr));
}

void Emulator::writeMemory(const uint64_t address, const uint64_t value, const uint8_t size) const
{
	const mem_segment* seg = nullptr;
	for (const auto& segment : this->memory) {
		if (segment.startAddr <= address && segment.endAddr > address) {
			seg = &segment;
			break;
		}
	}
	if (seg == nullptr) {
		this->log->LogError("Address 0x%08llx not found\n", address);
		exit(-1); // figure out a better way to throw an error
	}

	this->log->LogDebug("Writing 0x%08llx to 0x%08llx", value, address);
	if (size == 1) {
		*(seg->mem + (address - seg->startAddr)) = value;
	} else if (size == 2) {
		*reinterpret_cast<uint16_t*>(seg->mem + (address - seg->startAddr)) = value;
	} else if (size == 4) {
		*reinterpret_cast<uint32_t*>(seg->mem + (address - seg->startAddr)) = value;
	} else { // size is assumed to be uint64_t
		*reinterpret_cast<uint64_t*>(seg->mem + (address - seg->startAddr)) = value;
	}
}

void Emulator::call_function(const uint64_t func_addr, const uint64_t retInstrIdx)
{
	const auto func = this->bv->GetAnalysisFunction(this->bv->GetDefaultPlatform(), func_addr);
	const auto llil_func = func->GetLowLevelIL();
	this->callstack.emplace(stackFrame { .llilFunction = llil_func, .curInstrIdx = retInstrIdx });
}

void Emulator::return_from_function()
{
	this->callstack.pop();
}

double Emulator::visit(const LowLevelILInstruction* instr)
{
	const auto& log = this->log;
	log->LogDebug("\tVisiting [%s]", LLIL_NAME[instr->operation]);

	double ret = 0.0;
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
			ret = -1;
		}
	}
	return ret;
}
