#include "emulator.hpp"
#include <cstdio>

using namespace BinaryNinja;

int main(const int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: ./bnil-emulator <binary path> <starting function>\n");
		return 1;
	}

	// In order to initiate the bundled plugins properly, the location
	// of where bundled plugins directory is must be set.
	SetBundledPluginDirectory(GetBundledPluginDirectory());
	InitPlugins(true);
	LogToStdout(InfoLog);

	const Ref<BinaryView> bv = Load(argv[1], true);
	bv->UpdateAnalysisAndWait();

	const auto log = LogRegistry::CreateLogger(plugin_name);

	if (!bv || bv->GetTypeName() == "Raw") {
		log->LogError("Input file does not appear to be an executable\n");
		return -1;
	}

	// Get `main` function
	const Ref<Symbol> sym = bv->GetSymbolByRawName(argv[2]);
	if (!sym) {
		log->LogError("Unable to find \"%s\" in the binary", argv[2]);
	}

	const Ref<Function> func = bv->GetAnalysisFunction(bv->GetDefaultPlatform(), sym->GetAddress());
	const Ref<LowLevelILFunction> llil_func = func->GetLowLevelIL();

	const auto emu_state = new Emulator(bv);
	if (!llil_func) {
		log->LogError("[!] LLIL is not available for %s @ 0x%llx\n", sym->GetFullName().c_str(), sym->GetAddress());
		delete emu_state;
		return -1;
	}

	emu_state->emulate_llil(llil_func);
	emu_state->dump_registers();
	return 0;
}
