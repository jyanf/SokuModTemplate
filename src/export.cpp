#include "main.hpp"
#include "export.hpp"

#include "SokuLib.hpp"

#include <cstring>

// Called by the mod loader to check the target game version (Soku 1.10a).
extern "C" __declspec(dllexport) bool CheckVersion(const BYTE hash[16])
{
	return std::memcmp(hash, SokuLib::targetHash, sizeof(SokuLib::targetHash)) == 0;
}

// Called once the mod loader is ready; register hooks here.
extern "C" __declspec(dllexport) bool Initialize(HMODULE hMyModule, HMODULE hParentModule)
{
	(void)hParentModule;
	DWORD old;
#ifdef _DEBUG
	FILE* _;

	AllocConsole();
	freopen_s(&_, "CONOUT$", "w", stdout);
	freopen_s(&_, "CONOUT$", "w", stderr);
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(h, &mode);
	SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
	mod::hModule = hMyModule;

	VirtualProtect((LPVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_READWRITE, &old);

	mod::Initialize();

	VirtualProtect((LPVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
	FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
	return true;
}

extern "C" int APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	return TRUE;
}

// Higher values load first; equal priorities have an undefined order.
extern "C" __declspec(dllexport) int getPriority()
{
	return 0;
}

// Exported API declared in distribution/export.hpp; callable from other mods
// through LoadLibrary + GetProcAddress.
MOD_API const char* GetModName()
{
	return S_MOD_NAME;
}

MOD_API const char* GetModVersion()
{
	return S_MOD_VERSION;
}
