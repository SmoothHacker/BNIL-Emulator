#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <unicorn/unicorn.h>
#include <vector>

inline int regs[] = {
	UC_X86_REG_RAX,
	UC_X86_REG_RBP,
	UC_X86_REG_RBX,
	UC_X86_REG_RCX,
	UC_X86_REG_RDI,
	UC_X86_REG_RDX,
	UC_X86_REG_RIP,
	UC_X86_REG_RSI,
	UC_X86_REG_RSP
};

constexpr char reg_names[9][4] = {
	"RAX",
	"RBP",
	"RBX",
	"RCX",
	"RDI",
	"RDX",
	"RIP",
	"RSI",
	"RSP"
};

inline const std::vector<std::pair<std::string, int>> registers = {
	{ "rax", UC_X86_REG_RAX },
	{ "rbx", UC_X86_REG_RBX },
	{ "rsp", UC_X86_REG_RSP },
	{ "rdi", UC_X86_REG_RDI },
	{ "rdx", UC_X86_REG_RDX },
	{ "rcx", UC_X86_REG_RCX },
	{ "rbp", UC_X86_REG_RBP },
	{ "rsi", UC_X86_REG_RSI }
};

#endif // REGISTERS_HPP
