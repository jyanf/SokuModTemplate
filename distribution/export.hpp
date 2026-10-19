#pragma once
#include <Windows.h>

// MOD_API selects dllexport while building this mod (MY_MOD_EXPORTS) and
// dllimport for other modules that include this header.
#ifdef MY_MOD_EXPORTS
#define MOD_API extern "C" __declspec(dllexport)
#else
#define MOD_API extern "C" __declspec(dllimport)
#endif

// Sample C ABI: other mods can call these through LoadLibrary/GetModuleHandle + GetProcAddress.
// Add your own exports here and reuse them from the Lua bindings in
// src/export_lua.cpp.
MOD_API const char* GetModName();
MOD_API const char* GetModVersion();
