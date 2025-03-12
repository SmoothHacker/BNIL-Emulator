#include <catch2/catch_test_macros.hpp>

#include <vector>
#include <cinttypes>
#include <string>
#include <emulator.hpp>

TEST_CASE("BasicProgram1") {
	SetBundledPluginDirectory(GetBundledPluginDirectory());
	InitPlugins(true);
	LogToStdout(DebugLog);
	const auto log = LogRegistry::CreateLogger(plugin_name);

	const Ref<Architecture> arch = Architecture::GetByName("x86_64");
	const Ref<Platform> plat = Platform::GetByName("linux-x86_64");
	DataBuffer db {};
	std::string errors;
	arch->Assemble("mov rcx, 0xdead;\nmov rdx, 0xbeef;\nadd rdx, rcx;\n retn 0;", 0x0, db, errors);

	const Ref<BinaryView> bv = Load(db, true);
	bv->SetDefaultPlatform(plat);
	const Ref<Function> func = bv->AddFunctionForAnalysis(plat, 0x0, false);
	bv->UpdateAnalysisAndWait();
	const Ref<LowLevelILFunction> il = func->GetLowLevelIL();

	for (const auto& block : il->GetBasicBlocks()) {
		for (size_t instrIndex = block->GetStart(); instrIndex < block->GetEnd(); instrIndex++) {
			const LowLevelILInstruction instr = (*il)[instrIndex];
			std::vector<InstructionTextToken> tokens;
			printf("Arch name %s\n", func->GetArchitecture()->GetName().c_str());
			il->GetInstructionText(func, func->GetArchitecture(), instrIndex, tokens);
			printf("    %" PRIdPTR " @ 0x%" PRIx64 "  ", instrIndex, instr.address);
			for (auto& token : tokens)
				printf("%s", token.text.c_str());
			printf("\n");
		}
	}

	const auto emu = new Emulator(bv);
	emu->emulate_llil(il);

	printf("Address Size: %lu\n", bv->GetAddressSize());
	CHECK(1 == 1);
}
