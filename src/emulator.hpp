#pragma once
#include "lowlevelilinstruction.h"

#include <stack>
#include <vector>
using namespace BinaryNinja;

#pragma once

constexpr auto plugin_name = "plugin.bnil_emulator";

enum ret_val_type : uint8_t {
	UNIMPL,
	UINT64_T,
	UINT32_T,
};

// This is needed to return discovered values to the calling visit function
typedef struct ret_val {
	ret_val_type discriminator;
	union {
		uint64_t value = 0;
	};
} ret_val;

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

ret_val operator+(const ret_val& lhs, const ret_val& rhs);
ret_val operator-(const ret_val& lhs, const ret_val& rhs);

class Emulator {
	std::vector<mem_segment> memory; // segments->memory_page
	Ref<BinaryView> bv;
	std::stack<stackFrame> callstack;
	std::map<uint32_t, uint64_t> regs;

public:
	[[nodiscard]] stackFrame getTopCallstack() const { return callstack.top(); }
	void setRegister(uint32_t reg, ret_val value);
	Ref<BinaryView> getBinaryView();
	void dumpRegisters();

	Ref<Logger> log;
	explicit Emulator(BinaryView* bv);
	~Emulator();

	ret_val visit(const LowLevelILInstruction* instr);
	void call_function(uint64_t func_addr, uint64_t retInstrIdx);
	void return_from_function();

	void printCallstack();

	bool isFunctionThunk(uint64_t address) const;
	void emulate_llil(const Ref<LowLevelILFunction>& llil_func);
};
