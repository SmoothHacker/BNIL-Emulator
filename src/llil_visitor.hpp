#pragma once
#include "emulator.hpp"

uint64_t visit_LLIL_SET_REG(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_SET_REG_SPLIT(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_SET_FLAG(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_LOAD(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_STORE(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_PUSH(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_POP(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_REG(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_REG_SPLIT(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CONST(const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CONST_PTR(const LowLevelILInstruction* instr);
uint64_t visit_LLIL_FLAG(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_ADD(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_SUB(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_SBB(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_AND(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_OR(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_XOR(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_LSL(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_LSR(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_ASR(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_MUL(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_DIVU_DP(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_DIVS_DP(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_MODU_DP(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_MODS_DP(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_NEG(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_SX(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_ZX(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_LOW_PART(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_JUMP(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_JUMP_TO(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CALL(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_TAILCALL(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_RET(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_NORET(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_IF(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_GOTO(const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_E(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_NE(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_SLT(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_ULT(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_SLE(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_ULE(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_SGE(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_UGE(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_SGT(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_CMP_UGT(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_TEST_BIT(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_INTRINSIC(Emulator* emu, const LowLevelILInstruction* instr);
uint64_t visit_LLIL_UNDEF(Emulator* emu, const LowLevelILInstruction* instr);

// Helpers

/*
 * Useful for evaluating arithmetic and comparison instructions that only have a LeftExpr and a RightExpr
 */
uint64_t BinaryVisitorHelper(Emulator* emu, const LowLevelILInstruction* instr, const std::function<bool(uint64_t, uint64_t)>& condition);

constexpr const char* const LLIL_NAME[] = {
	"LLIL_NOP",
	"LLIL_SET_REG",
	"LLIL_SET_REG_SPLIT",
	"LLIL_SET_FLAG",
	"LLIL_SET_REG_STACK_REL",
	"LLIL_REG_STACK_PUSH",
	"LLIL_ASSERT",
	"LLIL_FORCE_VER",
	"LLIL_LOAD",
	"LLIL_STORE",
	"LLIL_PUSH",
	"LLIL_POP",
	"LLIL_REG",
	"LLIL_REG_SPLIT",
	"LLIL_REG_STACK_REL",
	"LLIL_REG_STACK_POP",
	"LLIL_REG_STACK_FREE_REG",
	"LLIL_REG_STACK_FREE_REL",
	"LLIL_CONST",
	"LLIL_CONST_PTR",
	"LLIL_EXTERN_PTR",
	"LLIL_FLOAT_CONST",
	"LLIL_FLAG",
	"LLIL_FLAG_BIT",
	"LLIL_ADD",
	"LLIL_ADC",
	"LLIL_SUB",
	"LLIL_SBB",
	"LLIL_AND",
	"LLIL_OR",
	"LLIL_XOR",
	"LLIL_LSL",
	"LLIL_LSR",
	"LLIL_ASR",
	"LLIL_ROL",
	"LLIL_RLC",
	"LLIL_ROR",
	"LLIL_RRC",
	"LLIL_MUL",
	"LLIL_MULU_DP",
	"LLIL_MULS_DP",
	"LLIL_DIVU",
	"LLIL_DIVU_DP",
	"LLIL_DIVS",
	"LLIL_DIVS_DP",
	"LLIL_MODU",
	"LLIL_MODU_DP",
	"LLIL_MODS",
	"LLIL_MODS_DP",
	"LLIL_NEG",
	"LLIL_NOT",
	"LLIL_SX",
	"LLIL_ZX",
	"LLIL_LOW_PART",
	"LLIL_JUMP",
	"LLIL_JUMP_TO",
	"LLIL_CALL",
	"LLIL_CALL_STACK_ADJUST",
	"LLIL_TAILCALL",
	"LLIL_RET",
	"LLIL_NORET",
	"LLIL_IF",
	"LLIL_GOTO",
	"LLIL_FLAG_COND",
	"LLIL_FLAG_GROUP",
	"LLIL_CMP_E",
	"LLIL_CMP_NE",
	"LLIL_CMP_SLT",
	"LLIL_CMP_ULT",
	"LLIL_CMP_SLE",
	"LLIL_CMP_ULE",
	"LLIL_CMP_SGE",
	"LLIL_CMP_UGE",
	"LLIL_CMP_SGT",
	"LLIL_CMP_UGT",
	"LLIL_TEST_BIT",
	"LLIL_BOOL_TO_INT",
	"LLIL_ADD_OVERFLOW",
	"LLIL_SYSCALL",
	"LLIL_BP",
	"LLIL_TRAP",
	"LLIL_INTRINSIC",
	"LLIL_UNDEF",
	"LLIL_UNIMPL",
	"LLIL_UNIMPL_MEM",
	"LLIL_FADD",
	"LLIL_FSUB",
	"LLIL_FMUL",
	"LLIL_FDIV",
	"LLIL_FSQRT",
	"LLIL_FNEG",
	"LLIL_FABS",
	"LLIL_FLOAT_TO_INT",
	"LLIL_INT_TO_FLOAT",
	"LLIL_FLOAT_CONV",
	"LLIL_ROUND_TO_INT",
	"LLIL_FLOOR",
	"LLIL_CEIL",
	"LLIL_FTRUNC",
	"LLIL_FCMP_E",
	"LLIL_FCMP_NE",
	"LLIL_FCMP_LT",
	"LLIL_FCMP_LE",
	"LLIL_FCMP_GE",
	"LLIL_FCMP_GT",
	"LLIL_FCMP_O",
	"LLIL_FCMP_UO"
};