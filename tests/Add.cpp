#include <catch2/catch_test_macros.hpp>

#include <cinttypes>
#include <emulator.hpp>
#include <string>
#include <vector>

static Emulator* CreateInstance(const std::string& input, Ref<LowLevelILFunction>& llil_func)
{
	SetBundledPluginDirectory(GetBundledPluginDirectory());
	InitPlugins(true);
	const auto log = LogRegistry::CreateLogger(plugin_name);
	const Ref<Architecture> arch = Architecture::GetByName("x86_64");
	const Ref<Platform> plat = Platform::GetByName("linux-x86_64");
	DataBuffer db {};
	std::string errors;
	arch->Assemble(input, 0x0, db, errors);

	const Ref<BinaryView> bv = Load(db, true);
	bv->SetDefaultPlatform(plat);
	const Ref<Function> func = bv->AddFunctionForAnalysis(plat, 0x0, false);
	bv->UpdateAnalysisAndWait();
	const Ref<LowLevelILFunction> il = func->GetLowLevelIL();
	LogToStdout(DebugLog);

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

TEST_CASE("BasicProgram1")
{
	const std::string input = "mov rcx, 0xdead;\nmov rdx, 0xbeef;\nadd rdx, rcx;\n retn 0;";
	Ref<LowLevelILFunction> llil_func;
	const auto emu = CreateInstance(input, llil_func);
	emu->emulate_llil(llil_func);

	const auto rdx = llil_func->GetFunction()->GetArchitecture()->GetRegisterByName("rdx");
	const auto rdx_val = emu->getRegister(rdx);
	CHECK(rdx_val == 0x19d9c);
}
