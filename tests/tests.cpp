#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cinttypes>
#include <emulator.hpp>
#include <string>
#include <unicorn/unicorn.h>
#include <vector>

#include "registers.hpp"

static int regs[] = {
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

static void DumpState(uc_engine* uc)
{
	void* ptrs[9];
	uint64_t vals[9] = {};

	const auto log = LogRegistry::GetLogger(plugin_name);
	for (int i = 0; i < 9; i++) {
		ptrs[i] = &vals[i];
	}
	uc_reg_read_batch(uc, regs, ptrs, 9);
	log->LogDebug("Unicorn Emulator State:");
	for (int i = 0; i < 9; i++) {
		log->LogDebug("[%s]: 0x%08x", reg_names[i], vals[i]);
	}
}

static Emulator* CreateEmuInstance(const std::string& input, Ref<LowLevelILFunction>& llil_func)
{
	SetBundledPluginDirectory(GetBundledPluginDirectory());
	InitPlugins(true);
	const auto log = LogRegistry::CreateLogger(plugin_name);
	const Ref<Architecture> arch = Architecture::GetByName("x86_64");
	const Ref<Platform> plat = Platform::GetByName("linux-x86_64");
	DataBuffer db {};
	std::string errors;
	arch->Assemble(input, 0x0, db, errors);
	if (!errors.empty())
		printf("Asm errors: %s", errors.c_str());

	const Ref<BinaryView> bv = Load(db, true);
	bv->SetDefaultPlatform(plat);
	// Add RW segment
	bv->AddUserSegment(0x1000, 0x100, 0x0, 0x0, SegmentReadable | SegmentWritable);
	const Ref<Function> func = bv->AddFunctionForAnalysis(plat, 0x0, false);
	bv->UpdateAnalysisAndWait();
	const Ref<LowLevelILFunction> il = func->GetLowLevelIL();
	LogToStdout(DebugLog);

	log->LogDebug("LLIL Printout:");
	for (const auto& block : il->GetBasicBlocks()) {
		for (size_t instrIndex = block->GetStart(); instrIndex < block->GetEnd(); instrIndex++) {
			const LowLevelILInstruction instr = (*il)[instrIndex];
			std::vector<InstructionTextToken> tokens;
			il->GetInstructionText(func, func->GetArchitecture(), instrIndex, tokens);
			char lineBuf[50] = {};
			snprintf(lineBuf, sizeof(lineBuf), "    %" PRIdPTR " @ 0x%" PRIx64 "  ", instrIndex, instr.address);
			std::string disassLine(lineBuf, strlen(lineBuf));
			for (const auto& token : tokens)
				disassLine += token.text;

			log->LogDebug("%s", disassLine.c_str());
		}
	}
	llil_func = il;
	const auto emu = new Emulator(bv);
	const auto sp_reg_idx = emu->getBinaryView()->GetDefaultArchitecture()->GetStackPointerRegister();
	emu->setRegister(sp_reg_idx, 0x2000);
	return emu;
}

static uc_engine* CreateUnicornInstance(const std::string& input)
{
	uc_engine* uc;

	const auto log = LogRegistry::GetLogger(plugin_name);
	const Ref<Architecture> arch = Architecture::GetByName("x86_64");
	const Ref<Platform> plat = Platform::GetByName("linux-x86_64");
	DataBuffer db {};
	std::string errors;
	arch->Assemble(input, 0x0, db, errors);
	if (!errors.empty())
		log->LogError("Asm errors: %s", errors.c_str());

	log->LogDebug("Emulating test case with unicorn");
	uc_err err = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);
	if (err != UC_ERR_OK) {
		log->LogError("Failed on uc_open() with error returned: %u\n", err);
		return nullptr;
	}

	uc_mem_map(uc, 0x0, 0x1000, UC_PROT_ALL); // CODE
	uc_mem_map(uc, 0x1000, 0x1000, UC_PROT_READ | UC_PROT_WRITE); // STACK

	constexpr uint64_t sp = 0x2000;
	uc_reg_write(uc, UC_X86_REG_SP, &sp);

	if (uc_mem_write(uc, 0x0, db.GetData(), db.GetLength())) {
		log->LogError("Failed to write emulation code to memory, quit!\n");
		return nullptr;
	}

	// emulate testcase
	err = uc_emu_start(uc, 0x0, db.GetLength(), 0x0, std::ranges::count(input, '\n'));
	if (err) {
		log->LogError("Failed on uc_emu_start() with error returned %u: %s\n", err, uc_strerror(err));
		DumpState(uc);
		return nullptr;
	}

	log->LogDebug("Emulation Finished");
	return uc;
}

static void CheckResults(Emulator* emu, uc_engine* uc)
{
	const auto bv = emu->getBinaryView();
	const auto arch = bv->GetDefaultArchitecture();
	auto regs = arch->GetFullWidthRegisters();
	const auto log = LogRegistry::GetLogger(plugin_name);

	uint64_t regValue = 0;
	for (auto& [regName, ucRegIdx] : registers) {
		const auto regId = arch->GetRegisterByName(regName);
		const auto emuVal = static_cast<uint64_t>(emu->getRegister(regId));
		uc_reg_read(uc, ucRegIdx, &regValue);
		log->LogDebug("Checking Register: %s", regName.c_str());
		CHECK(emuVal == regValue);
	}
	uc_close(uc);
}

TEST_CASE("BasicProgram1")
{
	const std::string input = "mov rcx, 0xdead;\n"
							  "mov rdx, 0xbeef;\n"
							  "add rdx, rcx;\n"
							  "retn 0;";
	Ref<LowLevelILFunction> llil_func;
	const auto emu = CreateEmuInstance(input, llil_func);
	emu->emulate_llil(llil_func);

	if (const auto uc = CreateUnicornInstance(input); uc == nullptr) {
		const auto log = LogRegistry::GetLogger(plugin_name);
		log->LogError("Failed to create unicorn instance");
	} else {
		CheckResults(emu, uc);
	}
	delete emu;
}

TEST_CASE("BasicProgram2")
{
	const std::string input = "mov rcx, 0xdead;\n"
							  "mov dword [0x1000], 0xbeef;\n"
							  "mov rax, [0x1000];\n"
							  "retn 0;";
	Ref<LowLevelILFunction> llil_func;
	const auto emu = CreateEmuInstance(input, llil_func);
	emu->emulate_llil(llil_func);

	if (const auto uc = CreateUnicornInstance(input); uc == nullptr) {
		const auto log = LogRegistry::GetLogger(plugin_name);
		log->LogError("Failed to create unicorn instance");
	} else {
		CheckResults(emu, uc);
	}
	delete emu;
}

TEST_CASE("BasicProgram3")
{
	const std::string input = "mov rcx, 0xdead;\n"
							  "push rcx;\n"
							  "pop rax;\n"
							  "retn 0;";
	Ref<LowLevelILFunction> llil_func;
	const auto emu = CreateEmuInstance(input, llil_func);
	emu->emulate_llil(llil_func);

	if (const auto uc = CreateUnicornInstance(input); uc == nullptr) {
		const auto log = LogRegistry::GetLogger(plugin_name);
		log->LogError("Failed to create unicorn instance");
	} else {
		CheckResults(emu, uc);
	}
	delete emu;
}
