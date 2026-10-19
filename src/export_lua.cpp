#ifdef MY_LUA_EXPORT

#include "main.hpp"
#include "export.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#define MY_CONCAT_IMPL(a, b) a##b
#define MY_CONCAT(a, b) MY_CONCAT_IMPL(a, b)
#define LUA_OPEN_NAME MY_CONCAT(luaopen_, MY_MOD_NAME)

extern "C" __declspec(dllexport) int LUA_OPEN_NAME(lua_State* L)
{
	if (mod::hModule == NULL) {
		return luaL_error(L, "Failed to load %s: DLL not initialized.", S_MOD_NAME);
	}

	luabridge::getGlobalNamespace(L)
		.beginNamespace(S_MOD_NAME)
			.addFunction("GetModName", &GetModName)
			.addFunction("GetModVersion", &GetModVersion)
		.endNamespace();

	lua_getglobal(L, S_MOD_NAME);
	return 1;
}

#endif // MY_LUA_EXPORT
