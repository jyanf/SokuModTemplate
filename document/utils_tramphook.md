# tramphook.hpp 中转钩子库说明

本文整理自 [CharacterTrailFix](https://github.com/jyanf/CharacterTrailFix) 项目的 `src/utils/tramphook.hpp`，以及该项目 `src/main.hpp`、`src/main.cpp` 中对它的实际用法，作为在本模板中集成"可读写寄存器的内联中转钩子（inline detour）"的参考。

## 1. 概览

`tramphook.hpp` 提供两个部件：`utils` 命名空间里的 `TrampTamper` / `CPUState`，以及 `utils::tamper` 里的内存池（全程序共享单例）：

| 部件 | 作用 |
| --- | --- |
| `utils::tamper::TrampolinePool`，经 `utils::tamper::trampolinePool()` 取共享单例 | 全程序共享的 RWX 蹦床内存池：`VirtualAlloc(PAGE_EXECUTE_READWRITE)` 按页申请、16 字节对齐的 bump 分配器 |
| `utils::TrampTamper<oprSize>` | 在任意函数中途的 N（≥5）个字节处植入中转跳板，调用回调并允许**读写当时全部通用寄存器** |
| `utils::CPUState` | pushad 栈帧布局，回调经由它读写寄存器 |

与 SokuLib 自带的 `TamperNearCall`（只能顶替一个 `E8 call` 的目标、拿到原目标指针）互补：`TrampTamper` 针对的是"指令流中间"的拦截点，不改函数签名，也不需要原函数是可调用指针——适合修改传参寄存器、跳过/观察某条指令前后的状态这类钩法。头文件注释注明 credit enebe shady/memory.cpp。

依赖：`<Windows.h>`、`<list>`、`<algorithm>`、`<cstdint>`、可选 `<mutex>`，以及 SokuLib 的 `<Tamper.hpp>`（用 `TamperNearJmp` / `TamperNearCall` 填 rel32 跳转/调用操作数）。仅适用于 **x86-32**（pushad/popad），与本模板 Win32 目标一致。

## 2. 跳板字节布局

`TrampTamper<oprSize>` 在池里分配 `size = oprSize + 14` 字节，写入如下机器码（`oprSize` 是被覆盖的原指令字节数，`static_assert(oprSize >= 5)`）：

```text
偏移     字节               指令              作用
──────────────────────────────────────────────────────────────────
0x00     60                 pushad            压入 8 个通用寄存器
0x01     89 E1              mov ecx, esp      ECX = pushad 帧地址 = 回调第 1 参数
0x03     E8 rel32           call hook         回调（rel 操作数由 TamperNearCall回填）
0x08     61                 popad             还原寄存器 —— 使用回调改写后的值
0x09     …oprSize 字节      （原指令拷贝）     钩点处被覆盖的原始指令
size-5   E9 rel32           jmp addr+oprSize  回到钩点之后继续原流程（TamperNearJmp 回填）
```

挂钩后的执行流：

```text
原流程:  … ─ [addr 处 N 字节] ─ addr+N …

hook(): addr 写入 jmp shim（其余 NOP），shim 尾部 jmp 回 addr+N

运行时:  addr → shim: pushad → call 回调 → popad → 原 N 字节 → 回到 addr+N
```

原字节在打补丁前被完整拷贝进 shim（这就是 `restore()` 能还原的依据），整个过程只动 `oprSize` 个字节，页保护由 `hook()`/`restore()` 自己用 `VirtualProtect` 处理。

## 3. TrampTamper 生命周期

```cpp
static TrampTamper<5> tamper(0x463173);   // 构造只记录地址，不打补丁

tamper.hook(&callback);   // 分配 shim + 改写目标字节，开始生效
tamper.restore();         // 写回原字节并归还 shim（可再 hook）
// 析构自动 restore()；对象不可拷贝
```

要点：

- **对象必须比钩子活得久**：通常放全局或静态对象。池是经 `inline` 函数取用的 **函数局部静态单例且故意不销毁**，任何 TU、任何顺序下运行的 `~TrampTamper → restore → free` 都能安全访问池（进程退出时由 OS 统一回收 RWX 页），不存在"池先于钩子析构"的问题。不过仍建议让 `TrampTamper` 对象活到进程结束，不要在运行中途销毁——中途 `restore()` 会把钩子真实拆掉。
- `addr == NULL` 时 `hook()` 直接返回；未 `hook()` 时 `restore()` 也是空操作——适合"按 ini 开关决定是否挂钩"的写法（见第 5 节）。
- **重复 `hook()` 不判重**：会对同一 shim 重复挂钩并泄漏前一块 shim，再次挂钩前必须先 `restore()`。

## 4. 回调与 CPUState

回调按 `__fastcall` 被调用（x86 下第 1 参数走 ECX，恰好是 shim 设置好的 pushad 帧指针）：

```cpp
void __fastcall callback(CPUState* state) { ... }
```

`CPUState` 就是 pushad 帧，字段与寄存器一一对应：

| 偏移 | 字段 | 寄存器 | 说明 |
| --- | --- | --- | --- |
| +0x00 | `edi` | EDI |  |
| +0x04 | `esi` | ESI |  |
| +0x08 | `ebp` | EBP |  |
| +0x0C | `esp` | ESP | pushad 存的是压栈前的 ESP；**popad 不还原 ESP，改它无效** |
| +0x10 | `ebx` | EBX |  |
| +0x14 | `edx` | EDX |  |
| +0x18 | `ecx` | ECX |  |
| +0x1C | `eax` | EAX |  |

因为 `call` 在 `popad` **之前**，回调里对 `state->xxx` 的修改会真正生效于后续指令；读则拿到钩点时刻的原值。限制：

- `pushad` 不含 EFLAGS——回调看不到也改不了标志位（需要时可在自己的 shim 变体里加 `pushfd`）；
- 不能跳过原 N 字节或改写返回地址，"拦截并替代原逻辑"仍应使用函数级 tamper（如 `TamperNearCall`）；
- 回调在钩点所在线程、真实 CPU 上下文中直接执行：保持简短、不抛异常、尽量避免 CRT/堆操作，热路径上注意 `pushad + call` 本身就有逐次开销。

## 5. 用法示例（CharacterTrailFix）

### 5.1 引入（`src/main.hpp`）

```cpp
#include "utils/tramphook.hpp"
using utils::TrampTamper, utils::CPUState;
```

### 5.2 定义钩子与回调（`src/main.cpp`）

钩点选在"残影步数匹配"处，覆盖两条指令共 5 字节：

```cpp
#define ADDR_STEP_INDEX_MATCH 0x463173
//00463173 83c1ff  ADD ECX,-0x1
//00463176 3bd1    CMP EDX, ECX
static TrampTamper<5> stepIndexTamper(ADDR_STEP_INDEX_MATCH);
void __fastcall stepIntercept(CPUState* state) {
    auto& trail = *reinterpret_cast<TrailImage*>(state->edi);  // 读：EDI = TrailImage*
    int stepIndex = trail.personal_stepIndex;
    stepIndex %= trail.step;                                   // 插入原程序没有的取模逻辑
    state->ecx = stepIndex + 1;                                // 写：改 ECX，后面的 ADD/CMP 用新值
}
```

### 5.3 按配置挂钩（`src/main.cpp` 的 `Initialize`）

```cpp
VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
// ...若干 TamperNearCall...
if (iniProxy["RenderFix"_l]["StepType"_l]) {
    stepIndexTamper.hook(&stepIntercept);   // ini 关闭时对象静静躺着，永不生效
}
VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
```

要点归纳：

1. **全局定义、Initialize 挂钩**：构造与挂钩分离，是否挂钩由 ini 配置决定——利用"未 hook 时 restore/析构均无害"的特性，无需任何清理代码；
2. **选点即"语义插槽"**：`0x463173` 处 ECX 恰好是步数索引，回调把裸寄存器解读为业务对象字段，在两条无害指令（`ADD`/`CMP`）之间插入逻辑；
3. 外层 `VirtualProtect(TEXT_SECTION)` 是为例行直写补丁保持一致的保护窗口，`hook()` 内部其实自带页保护处理；
4. 与 `TamperNearCall` 系函数级钩子（该项目里的 `RenderFixer`/`AutoTrailer` 等）分工：换整个函数用前者，改中途状态用本库。

## 6. 集成到 MyMod_Template

1. 头文件内 `#include <Tamper.hpp>` 依赖 include path 指向 `thirdparty/SokuLib/src`（模板工程已配置）；
2. 在 `src/main.cpp` 中定义钩子对象与回调，并在 `mod::Initialize()` 挂钩：

```cpp
// src/main.cpp
#include "utils/tramphook.hpp"
using utils::TrampTamper, utils::CPUState;

namespace {
// 示例地址，替换为你的钩点（5 字节指令边界上）
static TrampTamper<5> myTamper(0x407970);

void __fastcall myCallback(CPUState* state) {
    // 读: int v = state->eax;   写: state->ecx = v + 1;
}
}

namespace mod {
void Initialize() {
    myTamper.hook(&myCallback);   // hook/restore 自带 VirtualProtect，无需外层保护窗口
}
}
```

线程控制：默认用 `std::mutex` 保护内存池；单线程模组若在意锁开销，可在包含该头文件之前定义 `TRAMPOLINE_POOL_NO_LOCK`（改用空锁），建议通过 CMake `target_compile_definitions` 全局统一以避免各 TU 宏不一致。共享内存池：`utils::tamper::TrampolinePool` 是命名空间作用域类，单例经 `inline` 函数 `utils::tamper::trampolinePool()` 内的函数局部 `static` 提供（Meyers 单例，`new` 出后**故意不销毁**），因此 `inline` 合并保证全程序**只有一份池**，各 TU 的 shim 从同一地址空间分配。

## 7. 注意事项

- **钩点必须落在指令边界**，且被覆盖的 `oprSize` 字节会被**原样拷贝**到 shim 执行：其中不能含相对转移指令（`E8 call` / `E9 jmp` / `EB·7x jcc`），否则相对偏移在新地址失效；`ADD`/`CMP`/`MOV` 含绝对地址内存操作数在 x86-32 下重放安全。
- **多钩子区间不可重叠**：两个 `TrampTamper`（或与其他手写补丁）覆盖范围相交时，保存的"原字节"会捕获到对方的补丁，行为未定义。
- **打补丁非原子**：`NOP + jmp` 分步写入，游戏多线程运行时存在撕裂执行窗口。遵循加载器约定，在 `Initialize()` 阶段（游戏主线程尚未跑起来）一次性挂完，不要在运行中热挂/热解。
- **ESP 改了白改**，EFLAGS 不可见（见第 4 节）。
- **重复 `hook()` 会泄漏 shim 并二次打补丁**，先 `restore()` 再挂。
- **池的 `free()` 只数数不还页**：bump 分配器，归还未对齐页尾的块只会让页内出现空洞，整页 `allocationCount` 归零才 `VirtualFree`（或调 `shrink()` 回收空页）。模组通常在 `Initialize` 一次性挂钩、进程退出前不解除，影响可忽略。
- **`oprSize` 至少 5**（near jmp 长度），多出的字节在目标处填 `NOP`。
- 回调在 pushad/popad 保护通用寄存器，但**不处理浮点/SSE 状态和栈**：回调里若使用大局部变量或 CRT 功能，钩点剩余栈空间要够用；尽量保持回调极简。
