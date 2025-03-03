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
		uint64_t val_u64;
		int64_t value = 0;
	};
} ret_val;

typedef struct memSegment {
	uint8_t* mem;
	uint64_t startaddr;
	uint64_t endAddr;
} mem_segment;

typedef struct emuVairable {
	Variable var;
	void* data;
} emuVariable;

typedef struct stackFrame {
	std::stack<uint64_t> stack;
	Ref<LowLevelILFunction> current_function;
	uint64_t curr_instr_idx;
} stackFrame;

ret_val operator+(const ret_val& lhs, const ret_val& rhs);
ret_val operator-(const ret_val& lhs, const ret_val& rhs);

class EmulatorState {
	std::vector<mem_segment> memory; // segments->memory_page
	Ref<BinaryView> bv;
	std::stack<stackFrame> callstack;
	std::unordered_map<uint32_t, uint64_t> regs;

public:
	[[nodiscard]] stackFrame getTopCallstack() const { return callstack.top(); }
	void setRegister(uint32_t reg, ret_val value);

	Ref<Logger> log;
	explicit EmulatorState(BinaryView* bv);
	~EmulatorState();

	ret_val visit(const LowLevelILInstruction* instr);
	void call_function(uint64_t func_addr, uint64_t retInstrIdx);
	void return_from_function();

	void printCallstack();

	bool isFunctionThunk(uint64_t address) const;
	void emulate_llil(const Ref<LowLevelILFunction>& llil_func);
};
