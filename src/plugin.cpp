#include "binaryninjaapi.h"
#include "emulator.hpp"

using namespace BinaryNinja;

extern "C"
{
	BN_DECLARE_CORE_ABI_VERSION
	BINARYNINJAPLUGIN bool CorePluginInit()
	{
		// Do plugin init logic here
		return true;
	}
}
