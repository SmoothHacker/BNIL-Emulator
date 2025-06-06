#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <unicorn/unicorn.h>
#include <vector>

inline std::vector<std::pair<std::string, int32_t>> registers = {
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
