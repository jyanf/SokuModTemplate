#pragma once
#include <Windows.h>
#include <filesystem>
#include <string>

#define MY_STRINGFY_IMPL(name) #name
#define MY_STRINGFY(name) MY_STRINGFY_IMPL(name)
// Mod metadata injected by CMake; fallback values for standalone inclusion.
#ifndef MY_MOD_NAME
#define MY_MOD_NAME MyMod
#endif
#ifndef MY_MOD_VERSION
#define MY_MOD_VERSION 0.1.0
#endif

#define S_MOD_NAME MY_STRINGFY(MY_MOD_NAME)
#define S_MOD_VERSION MY_STRINGFY(MY_MOD_VERSION)

namespace mod {
	extern HMODULE hModule;

	// Called from the exported Initialize(); register hooks and load resources here.
	void Initialize();

	// Directory containing this DLL, i.e. <game>/modules/<mod name>/
	const std::filesystem::path& GetModuleFolder();

	// Default ini path: <module folder>/<mod name>.ini
	std::filesystem::path GetIniPath();
}
