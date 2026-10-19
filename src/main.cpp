#include "main.hpp"

namespace mod {
	HMODULE hModule = NULL;

	void Initialize() {
		// TODO: register hooks, read the ini, and load resources here.
		// Example: auto orgFunc = SokuLib::TamperNearCall(0x00401234, &MyHook);
		// _DEBUG builds attach a console, so std::cout is available.
	}

	const std::filesystem::path& GetModuleFolder() {
		static std::filesystem::path cache{};
		if (cache.empty()) {
			std::wstring buffer;
			int len = MAX_PATH + 1;
			do {
				buffer.resize(len);
				len = GetModuleFileNameW(hModule, buffer.data(), static_cast<DWORD>(buffer.size()));
			} while (len >= static_cast<int>(buffer.size()));
			if (len) cache = std::filesystem::path(buffer.begin(), buffer.end()).parent_path();
		}
		return cache;
	}

	std::filesystem::path GetIniPath() {
		return GetModuleFolder() / (std::string(S_MOD_NAME) + ".ini");
	}
}
