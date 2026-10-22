[![Status](https://github.com/jyanf/SokuModTemplate/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/jyanf/SokuModTemplate/actions/workflows/build.yml?branch=master)
[![SokuLib-dev](https://img.shields.io/badge/SokuLib-dev-lightblue?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCA2OC41MSA2OC41MSI+PGRlZnM+PHN0eWxlPi5jbHMtMXtmaWxsOiNmY2VlMjE7fTwvc3R5bGU+PC9kZWZzPjxnIGlkPSLlm77lsYJfMiIgZGF0YS1uYW1lPSLlm77lsYIgMiI+PGcgaWQ9InBsYXRlMiI+PHBhdGggY2xhc3M9ImNscy0xIiBkPSJNMzQuMjUsMCwwLDM0LjI1LDM0LjI1LDY4LjUxLDY4LjUxLDM0LjI1Wk0xNi41NSw0Ny4yN2gxOS43di0xSDE1LjU1bC0xMi0xMkwzNC4yNSwzLjU0LDY1LDM0LjI1LDM0LjI1LDY1WiIvPjxwYXRoIGNsYXNzPSJjbHMtMSIgZD0iTTE5LjI1LDM1Ljc3YTMsMywwLDEsMCwzLDNBMywzLDAsMCwwLDE5LjI1LDM1Ljc3Wm0wLDVhMiwyLDAsMSwxLDItMkEyLDIsMCwwLDEsMTkuMjUsNDAuNzdaIi8+PHBhdGggY2xhc3M9ImNscy0xIiBkPSJNMzQuMzUsNi43NCw3LjA2LDM0bDcuMTksNy4xOS0uNDMtLjU3Yy0uMjEtLjUyLS4yMy0xLjgxLS4xLTYsMC0uNjQsMC0xLjE1LDAtMS4zOSwwLTEuMzksMi4yNS03LDctN2g0YzEuNDQsMCwyLS41NiwyLTJzLTItMi0zLTJ2LTJjMi40OSwwLDUsMS4zNyw1LDRhMy42NCwzLjY0LDAsMCwxLTQsNGgtNGMtMy4zLDAtNSw0LjQtNSw1LDAsLjI1LDAsLjc5LDAsMS40NiwwLC40MSwwLDEtLjA1LDEuNjJsNi41OC02LjU4aDE2di0xLjVIMzkuNVYyMS44NmEyNywyNywwLDAsMCw1LjE5LTEuNjVsNS41Niw1LjU2LDcsN1YzN2MtLjUyLS4xNi0xLjY2LDEuMzEtMy4xMywxLjA3TDU0LDM5LjI3VjMzLjU4bC0uMDktLjE3QzUyLjc4LDMxLjMsNTAsMjksNDcuNTksMjguODVhMy4yNCwzLjI0LDAsMCwwLTMsMS41bC0uMTMuMTl2MTJBNiw2LDAsMCwxLDQzLDQ0LjA2bC40NywxLjQyYy45My0uMzEsMi0xLjgzLDIuMzgtMi4yOEw0Niw0M1YzMWExLjc0LDEuNzQsMCwwLDEsMS41Mi0uNjdjMS43NC4wOSw0LDEuOTUsNSwzLjYydjkuMzFMNTYsMzkuNjEsNjEuNjMsMzRabS01LjEsOCwxLTEsNy43Miw3LjcySDMxLjA2bC0xLjc4LTEuNzgsMCwwWm04Ljc1LDExSDI5LjI1VjIxLjgzTDMwLjQ0LDIzSDM4Wm0xLTUuMzMtNy43LTcuNywzLTMsOS4zMiw5LjMxQTQ0LjksNDQuOSwwLDAsMSwzOSwyMC40NFoiLz48cGF0aCBjbGFzcz0iY2xzLTEiIGQ9Ik0zOC4yNSwzOC43N2EzLDMsMCwxLDAtMywzQTMsMywwLDAsMCwzOC4yNSwzOC43N1ptLTMsMmEyLDIsMCwxLDEsMi0yQTIsMiwwLDAsMSwzNS4yNSw0MC43N1oiLz48cG9seWdvbiBjbGFzcz0iY2xzLTEiIHBvaW50cz0iMzQuMzUgNjEuMzEgNDAuODkgNTQuNzcgMjcuOCA1NC43NyAzNC4zNSA2MS4zMSIvPjwvZz48L2c+PC9zdmc+)](https://github.com/SokuDev/SokuLib/tree/dev)
[![Lua 5.4.8](https://img.shields.io/badge/lua-5.4.8-000080?logo=lua)](https://github.com/marovira/lua/tree/5.4.8)
[![LuaBridge3](https://img.shields.io/badge/LuaBridge-3-ed8663.svg)](https://github.com/kunitoki/LuaBridge3)
[![License](https://img.shields.io/badge/license-MIT-f4c430)](../LICENSE)
![简体中文](https://img.shields.io/badge/lang-zh-black.svg)
[![English](https://img.shields.io/badge/lang-en-blue.svg)](../README.md)

#  Soku 模组模板

东方非想天则（Touhou 12.3 / TH12.3）的通用 DLL 模组模板，整理自同作者的 [ShaderPro](https://github.com/jyanf/SokuShaderPro)、[CharacterTrailFix](https://github.com/jyanf/CharacterTrailFix)、[ReplayInputView--](https://github.com/jyanf/ReplayInputView--) 等项目。

- 平台：CMake+MSVC(Win32/x86) 构建，使用静态 CRT(`/MT`)，最终生成 DLL；
- Lua：可选的 Lua 模块导出（lua 5.4.8 + LuaBridge3），Lua 侧 `require("<模组名>")`（确保 cpath 有效）；
- .dat：可选地用 shady-cli 把 `src/data/` 打包成 `<模组名>.dat`，随 DLL 一起安装；
- 安装：构建后自动复制到 `%TH123%\modules\<模组名>\`，Visual Studio 调试时直接启动 `th123.exe`；
- ABI 导出：`src/export.cpp` + `distribution/export.hpp`；

## 快速开始

1. `git clone`。
2. 修改根目录 `CMakeLists.txt` 顶部的元信息：

   | 变量 | 说明 |
   | --- | --- |
   | `MY_MOD_NAME` | 模组名，同时作为 DLL 文件名，不要包含空格 |
   | `MY_VERSION` | 版本号，如 `0.1.0` |
   | `MY_DESCRIPTION` / `MY_DESCRIPTION_FILE` | 完整 / 简短描述，写入版本信息 |
   | `MY_COMPANY` | 作者或组织名，写入版本信息 |
   | `MY_LUA_EXPORT` | 是否导出 Lua 模块，默认 `OFF` |
   | `MY_BUILD_DAT` | 是否用 shady-cli 将 `src/data/` 打包为 `.dat`，默认 `OFF` |
3. 设置环境变量 `TH123` 指向游戏根目录（包含 `th123.exe`），或者配置时传 `-DMY_GAME_FOLDER=...`。不设置时安装路径会退化。
4. 如果启用了 `.dat` 打包：把资源文件放进 `src/data/`，并确保 CMake 能找到 `shady-cli.exe`（在 `PATH` 中，或配置时传 `-DMY_CLI_EXECUTABLE=...`）。
5. 构建（需要 Visual Studio 2022 的 C++ 工具链）：

   ```bat
   set TH123=D:\Games\Hisoutensoku\th123
   cmake -S . -B out -G "Visual Studio 17 2022" -A Win32 -DMY_BUILD_DAT=ON
   cmake --build out --config Release
   ```

   构建完成后会自动安装到 `%TH123%\modules\<模组名>\<模组名>.dll`；
   Debug 构建会额外安装 PDB；启用 `MY_BUILD_DAT` 时，会以`src/data` 为根打包成 `<模组名>.dat`（shady-cli `pack -m data`）并一并安装。
   
   用 Visual Studio 打开 `out\<模组名>.sln` 时，每次构建会先杀掉正在运行的 `th123.exe`，再以 `%TH123%\th123.exe` 启动调试。
   
   > 注意：安装是构建后的默认行为。如果设置了 `TH123` 环境变量，每次构建（包括 Release）都会直接写入真实游戏目录。

## 目录结构

```text
.
├─ CMakeLists.txt          模组元信息、构建、安装与调试配置
├─ src/
│  ├─ main.cpp             模组初始化逻辑（注册钩子从这里开始）
│  ├─ main.hpp             模组公共声明
│  ├─ export.cpp           加载器入口：CheckVersion / Initialize / getPriority / DllMain，
│  │                       以及 C ABI 导出接口实现
│  ├─ export_lua.cpp       Lua 模块入口 luaopen_<模组名>
│  ├─ data/                可选：.dat 打包的资源目录（MY_BUILD_DAT）
│  └─ version.rc.in        版本资源模板，由 CMake 生成 version.rc
├─ distribution/
│  ├─ export.hpp           对其他 DLL 模组公开的 C ABI 头文件
│  └─ README.md            分发内容说明
└─ thirdparty/
   ├─ CMakeLists.txt       依赖装配
   ├─ SokuLib/             非想天则逆向库（含 DirectX 头文件与导入库）
   ├─ lua/                 lua 5.4.8 源码（静态链接）
   └─ LuaBridge/           LuaBridge3 单头文件发行版
```

## 入口说明

| 符号 | 位置 | 说明 |
| --- | --- | --- |
| `CheckVersion` | `src/export.cpp` | 加载器校验游戏版本（Soku 1.10a） |
| `Initialize` | `src/export.cpp` | 加载器初始化入口，转调 `mod::Initialize()` |
| `getPriority` | `src/export.cpp` | 加载优先级，数值越大越先加载 |
| `DllMain` | `src/export.cpp` | DLL 入口 |
| `mod::Initialize` | `src/main.cpp` | **在这里注册钩子、读取配置、加载资源** |
| `luaopen_<模组名>` | `src/export_lua.cpp` | Lua 侧 `require("<模组名>")` 的模块入口 |

对外导出的示例接口 `GetModName` / `GetModVersion` 声明在 `distribution/export.hpp`，实现在 `src/export.cpp`；Lua 绑定直接复用它们，可以照着这个模式添加自己的接口。

## 依赖与许可

- 本模板使用 MIT License，见 [LICENSE](LICENSE)。
- `thirdparty/SokuLib`：Custom_SokuLib（MIT），见其目录内 LICENSE / README。
- `thirdparty/lua`：lua 5.4.8（MIT），见 `LUA_LICENSE`。
- `thirdparty/LuaBridge`：LuaBridge3（MIT），见 `LICENSE.txt`。
- `document/utils_tramphook.md`：整理自 [CharacterTrailFix](https://github.com/jyanf/CharacterTrailFix)（MIT）中的 `src/utils/tramphook.hpp` 及其在 `main.hpp` / `main.cpp` 中的用法。
- `document/utils_static_config.md`：整理自 [ReplayInputView--](https://github.com/jyanf/ReplayInputView--)（MIT）中的 `src/config.hpp`（含自定义 Field 适配器 `MultiField` 范例）及其在 `main.hpp` / `main.cpp` 中的用法。
