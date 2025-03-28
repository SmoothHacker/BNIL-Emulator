#include <catch2/catch_test_macros.hpp>
#include <cinttypes>
#include <emulator.hpp>
#include <string>
#include <vector>
#include <unicorn/unicorn.h>

#include "registers.hpp"

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
	return new Emulator(bv);
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

	uc_mem_map(uc, 0x1000, 0x100000, UC_PROT_ALL);
	//uc_mem_map(uc, 0x2000, 0x1000, UC_PROT_READ | UC_PROT_WRITE);

	if (uc_mem_write(uc, 0x0, db.GetData(), db.GetLength())) {
		log->LogError("Failed to write emulation code to memory, quit!\n");
		return nullptr;
	}

	// emulate testcase
	err = uc_emu_start(uc, 0x0, db.GetLength(), 0x0, 0x0);
	if (err) {
		log->LogError("Failed on uc_emu_start() with error returned %u: %s\n", err, uc_strerror(err));
		return nullptr;
	}

	log->LogDebug("Emulation Finished");
	return uc;
}

void CheckResults(Emulator* emu, uc_engine* uc)
{
	const auto bv = emu->getBinaryView();
	const auto arch = bv->GetDefaultArchitecture();
	auto regs = arch->GetFullWidthRegisters();

	uint64_t regValue = 0;
	for (auto &[regName, ucRegIdx] : registers) {
		const auto regId = arch->GetRegisterByName(regName);
		const auto emuVal = static_cast<uint64_t>(emu->getRegister(regId));
		uc_reg_read(uc, ucRegIdx, &regValue);
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

	const auto uc = CreateUnicornInstance(input);
	CheckResults(emu, uc);
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

	const auto uc = CreateUnicornInstance(input);
	CheckResults(emu, uc);
	delete emu;
}
