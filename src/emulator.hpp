#pragma once

#include "lowlevelilinstruction.h"

#include <stack>
#include <vector>

using namespace BinaryNinja;
constexpr auto plugin_name = "plugin.bnil_emulator";

enum double_type : uint8_t {
	UNIMPL,
	INT
};

typedef struct memSegment {
	uint8_t* mem;
	uint64_t startAddr;
	uint64_t endAddr;
} mem_segment;

typedef struct emuVariable {
	Variable var;
	void* data;
} emuVariable;

typedef struct stackFrame {
	uint8_t* stack;
	Ref<LowLevelILFunction> llilFunction;
	uint64_t curInstrIdx;
} stackFrame;

class Emulator {
	std::vector<mem_segment> memory; // segments->memory_page
	Ref<BinaryView> bv;
	std::stack<stackFrame> callstack;
	std::map<uint32_t, double> regs;

public:
	Ref<Logger> log;
	explicit Emulator(BinaryView* bv);
	~Emulator();

	[[nodiscard]] stackFrame getTopCallstack() const { return callstack.top(); }
	void setRegister(uint32_t reg, double value);
	double getRegister(uint32_t reg);
	Ref<BinaryView> getBinaryView();
	void dumpRegisters();
	double visit(const LowLevelILInstruction* instr);
	void call_function(uint64_t func_addr, uint64_t retInstrIdx);
	void return_from_function();
	void printCallstack();
	bool isFunctionThunk(uint64_t address) const;
	uint64_t readMemory(uint64_t address, uint8_t size) const;
	void writeMemory(uint64_t address, uint64_t value, uint8_t size) const;
	void emulate_llil(const Ref<LowLevelILFunction>& llil_func);
};
