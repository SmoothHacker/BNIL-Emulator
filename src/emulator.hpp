#pragma once

#include "lowlevelilinstruction.h"

#include <stack>
#include <vector>

using namespace BinaryNinja;
constexpr auto plugin_name = "plugin.bnil_emulator";

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
	uint64_t sf_base;
	uint64_t curInstrIdx;
} stackFrame;

class Emulator {
	std::vector<mem_segment> memory; // segments->memory_page
	Ref<BinaryView> bv;
	std::stack<stackFrame> callstack;
	std::unordered_map<uint32_t, uint64_t> regs;
	std::unordered_map<uint32_t, std::function<void(Emulator *emu, const LowLevelILInstruction*)>> intrinsic_handlers;

public:
	Ref<Logger> log;
	explicit Emulator(BinaryView* bv);
	~Emulator();
	[[nodiscard]] const stackFrame* peek_callstack() const { return &callstack.top(); }
	void set_register(uint32_t reg, uint64_t value);
	uint64_t get_register(uint32_t reg);
	Ref<BinaryView> get_binary_view();
	void dump_registers();
	uint64_t read_register(uint32_t reg);
	uint64_t visit(const LowLevelILInstruction* instr);
	void call_function(uint64_t func_addr, uint64_t retInstrIdx);
	void return_from_function();
	void print_callstack();
	bool is_function_thunk(uint64_t address) const;
	uint64_t read_memory(uint64_t address, uint8_t size) const;
	void write_memory(uint64_t address, uint64_t value, uint8_t size) const;

	// Resets registers and memory
	void reset_emulator();
	void emulate_llil(const Ref<LowLevelILFunction>& llil_func);

	// This will override existing handlers
	void register_intrinsic_handler(uint32_t intrinsic_idx, const std::function<void(Emulator* emu, const LowLevelILInstruction*)>& handler);
	std::function<void(Emulator* emu, const LowLevelILInstruction*)> *get_intrinsic_handler(uint32_t intrinsic_idx);
};
