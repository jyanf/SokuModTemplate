#include "main.hpp"
#include "SokuLib.hpp"

#include "utils/dbgprint.hpp"
#include "utils/vpguard.hpp"

namespace {
	static auto orgSokuSetup = reinterpret_cast<bool(__fastcall*)(void*, DWORD, void*)>(0x407970);
	static bool __fastcall OnSetupInitialize(void* This, DWORD _, void* config) {
#ifdef MY_BUILD_DAT
		const auto& datpath = mod::GetDatPath();
		if (std::filesystem::is_regular_file(datpath))
			try { SokuLib::appendDatFile(datpath.string().c_str()); }
			catch (...) {
				const auto msg = S_MOD_NAME ": failed to load assets package (.dat).";
				MessageBoxA(nullptr, msg, S_MOD_NAME " Error", MB_ICONERROR | MB_OK);
				throw std::runtime_error(msg);
			}
		else {
			const auto msg = S_MOD_NAME ": assets file (.dat) not found.";
			MessageBoxA(nullptr, msg, S_MOD_NAME " Error", MB_ICONERROR | MB_OK);
			throw std::runtime_error(msg);
		}
#endif // MY_BUILD_DAT
		return orgSokuSetup && orgSokuSetup(This, _, config);
	}
}

namespace mod {
	HMODULE mod::hModule = NULL;

	void Initialize() {
		//myprintf("%s Initializing...", S_MOD_NAME);
		pyprint(S_MOD_NAME, "initializing... ");
		// register hooks, read the ini, and load resources here.
		iniProxy.setPath(GetIniPath());
		iniProxy.load();
		{ auto text_section_guard = utils::VpGuard(TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE);
			if (!text_section_guard) {
				myprintln("!! VirtualProtect() failed.");
				//throw std::runtime_error("VirtualProtect failed.");
				return;
			}
			//only for example, hook when needed
			orgSokuSetup = SokuLib::TamperNearCall(0x7fb871, &OnSetupInitialize);
		}
		pyprint("Config; [Example]>Enabled: ", iniProxy["Example"_l]["Enabled"_l]);
		myprintln("Done!");
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
	std::filesystem::path GetDatPath() {
		return GetModuleFolder() / (std::string(S_MOD_NAME) + ".dat");
	}
}
