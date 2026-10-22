[![Status](https://github.com/jyanf/SokuModTemplate/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/jyanf/SokuModTemplate/actions/workflows/build.yml?branch=master)
[![SokuLib-dev](https://img.shields.io/badge/SokuLib-dev-lightblue?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCA2OC41MSA2OC41MSI+PGRlZnM+PHN0eWxlPi5jbHMtMXtmaWxsOiNmY2VlMjE7fTwvc3R5bGU+PC9kZWZzPjxnIGlkPSLlm77lsYJfMiIgZGF0YS1uYW1lPSLlm77lsYIgMiI+PGcgaWQ9InBsYXRlMiI+PHBhdGggY2xhc3M9ImNscy0xIiBkPSJNMzQuMjUsMCwwLDM0LjI1LDM0LjI1LDY4LjUxLDY4LjUxLDM0LjI1Wk0xNi41NSw0Ny4yN2gxOS43di0xSDE1LjU1bC0xMi0xMkwzNC4yNSwzLjU0LDY1LDM0LjI1LDM0LjI1LDY1WiIvPjxwYXRoIGNsYXNzPSJjbHMtMSIgZD0iTTE5LjI1LDM1Ljc3YTMsMywwLDEsMCwzLDNBMywzLDAsMCwwLDE5LjI1LDM1Ljc3Wm0wLDVhMiwyLDAsMSwxLDItMkEyLDIsMCwwLDEsMTkuMjUsNDAuNzdaIi8+PHBhdGggY2xhc3M9ImNscy0xIiBkPSJNMzQuMzUsNi43NCw3LjA2LDM0bDcuMTksNy4xOS0uNDMtLjU3Yy0uMjEtLjUyLS4yMy0xLjgxLS4xLTYsMC0uNjQsMC0xLjE1LDAtMS4zOSwwLTEuMzksMi4yNS03LDctN2g0YzEuNDQsMCwyLS41NiwyLTJzLTItMi0zLTJ2LTJjMi40OSwwLDUsMS4zNyw1LDRhMy42NCwzLjY0LDAsMCwxLTQsNGgtNGMtMy4zLDAtNSw0LjQtNSw1LDAsLjI1LDAsLjc5LDAsMS40NiwwLC40MSwwLDEtLjA1LDEuNjJsNi41OC02LjU4aDE2di0xLjVIMzkuNVYyMS44NmEyNywyNywwLDAsMCw1LjE5LTEuNjVsNS41Niw1LjU2LDcsN1YzN2MtLjUyLS4xNi0xLjY2LDEuMzEtMy4xMywxLjA3TDU0LDM5LjI3VjMzLjU4bC0uMDktLjE3QzUyLjc4LDMxLjMsNTAsMjksNDcuNTksMjguODVhMy4yNCwzLjI0LDAsMCwwLTMsMS41bC0uMTMuMTl2MTJBNiw2LDAsMCwxLDQzLDQ0LjA2bC40NywxLjQyYy45My0uMzEsMi0xLjgzLDIuMzgtMi4yOEw0Niw0M1YzMWExLjc0LDEuNzQsMCwwLDEsMS41Mi0uNjdjMS43NC4wOSw0LDEuOTUsNSwzLjYydjkuMzFMNTYsMzkuNjEsNjEuNjMsMzRabS01LjEsOCwxLTEsNy43Miw3LjcySDMxLjA2bC0xLjc4LTEuNzgsMCwwWm04Ljc1LDExSDI5LjI1VjIxLjgzTDMwLjQ0LDIzSDM4Wm0xLTUuMzMtNy43LTcuNywzLTMsOS4zMiw5LjMxQTQ0LjksNDQuOSwwLDAsMSwzOSwyMC40NFoiLz48cGF0aCBjbGFzcz0iY2xzLTEiIGQ9Ik0zOC4yNSwzOC43N2EzLDMsMCwxLDAtMywzQTMsMywwLDAsMCwzOC4yNSwzOC43N1ptLTMsMmEyLDIsMCwxLDEsMi0yQTIsMiwwLDAsMSwzNS4yNSw0MC43N1oiLz48cG9seWdvbiBjbGFzcz0iY2xzLTEiIHBvaW50cz0iMzQuMzUgNjEuMzEgNDAuODkgNTQuNzcgMjcuOCA1NC43NyAzNC4zNSA2MS4zMSIvPjwvZz48L2c+PC9zdmc+)](https://github.com/SokuDev/SokuLib/tree/dev)
[![Lua 5.4.8](https://img.shields.io/badge/lua-5.4.8-000080?logo=lua)](https://github.com/marovira/lua/tree/5.4.8)
[![LuaBridge3](https://img.shields.io/badge/LuaBridge-3-ed8663.svg)](https://github.com/kunitoki/LuaBridge3)
[![License](https://img.shields.io/badge/license-MIT-f4c430)](./LICENSE)
[![简体中文](https://img.shields.io/badge/lang-zh-red.svg)](./document/README_zh.md)
![English](https://img.shields.io/badge/lang-en-black.svg)

# Soku Mod Template

A general-purpose DLL mod template for *Hisoutensoku* (Touhou 12.3 / TH12.3), distilled from the author's other mods: [ShaderPro](https://github.com/jyanf/SokuShaderPro), [CharacterTrailFix](https://github.com/jyanf/CharacterTrailFix), [ReplayInputView--](https://github.com/jyanf/ReplayInputView--), and related projects.

- **Platform**: CMake + MSVC (Win32/x86), static CRT (`/MT`), producing a DLL.
- **Lua**: optional Lua module export (Lua 5.4.8 + LuaBridge3), loaded from Lua as `require("<mod name>")` (requires a valid `cpath`).
- **.dat**: optionally pack `src/data/` into `<mod name>.dat` with shady-cli and install it alongside the DLL.
- **Install**: each build copies the artifacts to `%TH123%\modules\<mod name>\`; Visual Studio debugging launches `th123.exe` directly.
- **ABI export**: `src/export.cpp` + `distribution/export.hpp`.

## Quick start

1. `git clone`.
2. Edit the mod metadata at the top of the root `CMakeLists.txt`:

   | Variable | Description |
   | --- | --- |
   | `MY_MOD_NAME` | Mod name, also the DLL file name. Avoid spaces. |
   | `MY_VERSION` | Version string, e.g. `0.1.0`. |
   | `MY_DESCRIPTION` / `MY_DESCRIPTION_FILE` | Full / short description, embedded in the version resource. |
   | `MY_COMPANY` | Author or organization, embedded in the version resource. |
   | `MY_LUA_EXPORT` | Export a Lua module. Default `OFF`. |
   | `MY_BUILD_DAT` | Pack `src/data/` into a `.dat` with shady-cli. Default `OFF`. |

3. Point the `TH123` environment variable at the game root (the folder containing `th123.exe`), or pass `-DMY_GAME_FOLDER=...` at configure time. Without it, install paths degrade.
4. To enable `.dat` packing, put your assets under `src/data/` and make sure CMake can locate `shady-cli.exe` (on `PATH`, or via `-DMY_CLI_EXECUTABLE=...`).
5. Build (requires the Visual Studio 2022 C++ toolchain):

   ```bat
   set TH123=D:\Games\Hisoutensoku\th123
   cmake -S . -B out -G "Visual Studio 17 2022" -A Win32 -DMY_BUILD_DAT=ON
   cmake --build out --config Release
   ```

   The build installs to `%TH123%\modules\<mod name>\<mod name>.dll`. Debug builds also install the PDB. With `MY_BUILD_DAT`, `src/data` is packed into `<mod name>.dat` (shady-cli `pack -m data`) and installed as well.

   Opening `out\<mod name>.sln` in Visual Studio makes each build kill any running `th123.exe` first, then launch `%TH123%\th123.exe` for debugging.

   > Note: installation is a post-build default. With `TH123` set, every build (Release included) writes straight into the live game folder.

## Layout

```text
.
├─ CMakeLists.txt          Mod metadata, build, install and debug configuration
├─ src/
│  ├─ main.cpp             Mod initialization (attach hooks here)
│  ├─ main.hpp             Shared mod declarations
│  ├─ export.cpp           Loader entry points: CheckVersion / Initialize / getPriority / DllMain,
│  │                       plus the C ABI export implementations
│  ├─ export_lua.cpp       Lua module entry point luaopen_<mod name>
│  ├─ data/                Optional asset root for .dat packaging (MY_BUILD_DAT)
│  └─ version.rc.in        Version resource template, expanded to version.rc by CMake
├─ distribution/
│  ├─ export.hpp           C ABI header exposed to other DLL mods
│  └─ README.md            Notes on the distribution contents
└─ thirdparty/
   ├─ CMakeLists.txt       Dependency wiring
   ├─ SokuLib/             Hisoutensoku reverse-engineering library (DirectX headers and import libs)
   ├─ lua/                 Lua 5.4.8 sources (statically linked)
   └─ LuaBridge/           LuaBridge3 single-header distribution
```

## Entry points

| Symbol | Location | Purpose |
| --- | --- | --- |
| `CheckVersion` | `src/export.cpp` | Loader verifies the game version (Soku 1.10a) |
| `Initialize` | `src/export.cpp` | Loader initialization entry that forwards to `mod::Initialize()` |
| `getPriority` | `src/export.cpp` | Load priority; higher loads first |
| `DllMain` | `src/export.cpp` | DLL entry point |
| `mod::Initialize` | `src/main.cpp` | **Attach hooks, read configuration, and load resources here** |
| `luaopen_<mod name>` | `src/export_lua.cpp` | Module entry for Lua's `require("<mod name>")` |

The sample exports `GetModName` / `GetModVersion` are declared in `distribution/export.hpp` and implemented in `src/export.cpp`. The Lua binding reuses them; follow the same pattern to add your own interfaces.

## Dependencies and licensing

- This template is MIT licensed; see [LICENSE](LICENSE).
- `thirdparty/SokuLib`: Custom_SokuLib (MIT), see the LICENSE / README in that directory.
- `thirdparty/lua`: Lua 5.4.8 (MIT), see `LUA_LICENSE`.
- `thirdparty/LuaBridge`: LuaBridge3 (MIT), see `LICENSE.txt`.
- `document/utils_tramphook.md`: derived from `src/utils/tramphook.hpp` in [CharacterTrailFix](https://github.com/jyanf/CharacterTrailFix) (MIT) and its usage in `main.hpp` / `main.cpp`.
- `document/utils_static_config.md`: derived from `src/config.hpp` in [ReplayInputView--](https://github.com/jyanf/ReplayInputView--) (MIT), including the custom `MultiField` adapter example and its usage in `main.hpp` / `main.cpp`.
