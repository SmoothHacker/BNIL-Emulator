#include "emulator.h"
#include <cstdio>

using namespace BinaryNinja;

int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: ./bnil-emulator <binary path> <starting function>\n");
		return 1;
	}

	// In order to initiate the bundled plugins properly, the location
	// of where bundled plugins directory is must be set.
	SetBundledPluginDirectory(GetBundledPluginDirectory());
	InitPlugins(true);
	LogToStdout(DebugLog);

	const Ref<BinaryView> bv = BinaryNinja::Load(argv[1], true);
	bv->UpdateAnalysisAndWait();

	const auto log = BinaryNinja::LogRegistry::CreateLogger(plugin_name);

	if (!bv || bv->GetTypeName() == "Raw") {
		log->LogError("Input file does not appear to be an executable\n");
		return -1;
	}

	// Get `main` function
	Ref<Symbol> sym = bv->GetSymbolByRawName(argv[2]);
	if (!sym) {
		sym = bv->GetSymbolByRawName("_main"); // macOS binaries use _main
	}

	const Ref<Function> func = bv->GetAnalysisFunction(bv->GetDefaultPlatform(), sym->GetAddress());
	const Ref<LowLevelILFunction> llil_func = func->GetLowLevelIL();

	EmulatorState* emu_state = new EmulatorState(bv);
	if (!llil_func) {
		log->LogError("[!] MLIL is not available for %s @ 0x%llx\n", sym->GetFullName().c_str(), sym->GetAddress());
		delete emu_state;
		return -1;
	}

	emu_state->emulate_llil(llil_func);
	return 0;
}
