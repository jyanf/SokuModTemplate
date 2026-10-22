# static_config.hpp 库说明

本文整理自 [`ReplayInputView--`](https://github.com/jyanf/ReplayInputView--) 项目的 `src/config.hpp` 及其 `src/main.hpp`、`src/main.cpp`、`src/ReplayInputView++/riv.cpp` 中的用法，对应当前模板内的 **`src/utils/static_config.hpp`**。

## 1. 概览

`static_config.hpp` 是一个**单头文件、编译期键名检查**的 INI 配置代理库。它把 Windows API `GetPrivateProfileIntA / GetPrivateProfileStringA / WritePrivateProfileStringA` 包装成三层对象：

```text
Config          一个 .ini 文件（路径 + 若干 Section）
 └─ Section     一个分节 [Section]（若干 Field）
     └─ Field   一个键值对 Key=Value（键名是模板字面量参数，类型是值适配器 Adapter）
         └─ Adapter   Integer / String / Point 等，封装实际读写逻辑
```

特性：

- 键名、节名都是**非类型模板参数（字符串字面量）**，`iniProxy["XXX"_l]` 写错节名/键名在编译期报错；
- 节内键重复、Config 内节名重复由 `static_assert` 在编译期拦截；
- 内存中始终持有配置值，INI 缺键时回退到代码里的默认值；
- 读写以 `(path, section, key)` 三元组为接口，适配器自行决定序列化方式。

依赖：`<Windows.h>`、`<filesystem>`、`<tuple>`，以及 SokuLib 的 `Vector2.hpp`（仅 `Point` 适配器使用）。
语法上使用 template literal NTTP、concepts、`consteval` 等，需要 **C++20 及以上**（本模板根 `CMAKE_CXX_STANDARD 23`，满足要求）。

## 2. 内部机制（了解即可）

| 组件 | 作用 |
| --- | --- |
| `_string_literal::_literal_helper<N>`（别名 `Literal`） | 把 `const char[N]` 变成可比较的 constexpr 结构体，作为字面量 NTTP 的载体 |
| `_string_literal::find_index` / `get_field` | 编译期在 `std::tuple` 中按字面量键名线性查找下标，`get<...>` / `operator[]` 由此实现 |
| `_static_checks::keys_are_unique` | 折叠表达式检查 Section/Field 键名唯一，重复即编译错误 |
| `_static_checks::AdapterType / FieldType / SectionType` | concepts，约束三层模板参数只能是对应类型 |
| `_supported_types::ValueBase<Var>` | 适配器基类：持有 `value`、隐式转换为返回值、纯虚 `read/write`；`ValueBase<void>` 作纯接口。 |

访问语法：`_literal<S>` 暴露 `::str`，因此 `iniProxy["ColorProfile"_l]["Hitbox.Melee"_l]` 会依次实例化 `Config::operator[]` → `Section::operator[]` → 返回 `Field&`，`Field` 再隐式转换为目标值类型。

## 3. 内置值适配器

内置三个适配器（定义于 `utils::cfg::_supported_types`，构造别名在 `utils::cfg::ex`）：

| 构造别名 | 适配器 | INI 中的形式 | 读写实现 | 备注 |
| --- | --- | --- | --- | --- |
| `addInteger<"K">` | `Integer : ValueBase<int>` | 十进制整数 | `GetPrivateProfileIntA` / `WritePrivateProfileStringA` | 缺键保留默认值 |
| `addBool<"K">` | 同 `Integer` | `0` / `1` | 同上 | 仅是 `Integer` 的别名，`bool` 经构造函数转成 `int` |
| `addString<"K">` | `String : ValueBase<std::string>` | 原始文本 | `GetPrivateProfileStringA`（缓冲区 1024 字节）/ `Write...` | 值长度受缓冲区限制 |
| `addPoint<"K">` | `Point : ValueBase<SokuLib::Vector2f>` | `x, y` 两个浮点 | 先 `sprintf_s("%f,%f")` 把当前值当默认串再 `GetPrivateProfileStringA` + `sscanf_s` | 写入格式 `%g, %g` |

还可**自定义适配器**：继承 `utils::ConfigValueBase<T>`（模板头文件提供的别名，等价于 `utils::cfg::_supported_types::ValueBase<T>`）并实现 `read/write`，即可作为 Field 的类型参数（`AdapterType` concept 校验）。

## 4. 自定义 Field 类型

`MultiField`（摘自 ReplayInputView-- `src/main.hpp`）演示了最有代表性的一类扩展：**一个 Field 展开成多行 INI 键**——Field 的键名本身是 printf 格式串（`"p%d.Enabled"`），读写时按序号 `i+1` 展开成 `p1.Enabled` … `p4.Enabled`，把 N 个玩家的同类配置收进同一个 Field 统一管理。

它同时展示了适配器的**可组合性**：`MultiField<Integer>`、`MultiField<Point>` 复用内置适配器逐行读写。

### 4.1 定义示例

```cpp
#include <array>
#include "utils/static_config.hpp"

// 原作中元素个数固定取宏 PLAYERS_NUMBER（CMake: add_definitions(-DPLAYERS_NUMBER=2|4)）；
// 这里改写为显式模板参数，更通用
template <typename Adapter, int N>
struct MultiField : utils::ConfigValueBase<bool> {   // 即 utils::cfg::_supported_types::ValueBase<bool>
    std::array<Adapter, N> fields;                   // 嵌套 N 个内层适配器（可组合）
    MultiField(std::initializer_list<typename Adapter::value_type> s) : Base(true) {
        std::copy(s.begin(), s.begin() + N, fields.begin());   // 取前 N 个作默认值
    }
    typename Adapter::value_type& operator[](int i) { return fields[i].value; }

    void read(const cstring& path, const cstring& section, const cstring& key) override {
        for (int i = 0; i < fields.size(); ++i) {
            char buf[24];
            sprintf(buf, key, i + 1);                // 关键：键名当 printf 格式串展开
            fields[i].read(path, section, buf);      // 逐行委托内层适配器
        }
    }
    void write(const cstring& path, const cstring& section, const cstring& key) const override {
        for (int i = 0; i < fields.size(); ++i) {
            char buf[24];
            sprintf(buf, key, i + 1);
            fields[i].write(path, section, buf);
        }
    }
};
```

要点：

- 基类选 `ValueBase<bool>` 只为满足 `AdapterType` concept 与 `Field` 的 `value_type` 要求——真值全部在 `fields` 数组里，基类的 `value` 不使用；
- `read/write` 的签名就是普通适配器契约，展开逻辑（此处为 `sprintf`）完全自定义；
- 原作 `operator[]` 返回类型写作 `Adapter::value_type&`，依赖 MSVC 宽松模式，严格模式下需 `typename` 前缀（上面已改好）。

### 4.2 使用（摘自 ReplayInputView-- `src/main.hpp` 的 `iniProxy`）

```cpp
#include "utils/static_config.hpp"
using namespace utils;        // addSection / addField / Config / addBool ...
using namespace utils::literal;   // "_l"（若 using namespace utils; 已覆盖可省略）

using IT = utils::cfg::_supported_types::Integer;   // 长类型起个短别名
using PT = utils::cfg::_supported_types::Point;

inline auto iniProxy = Config{
    "ReplayInputView++.ini",
    addSection<"Debug">(addBool<"Enabled">(false)
        ,addField<"Hotkey.p%d", MultiField<IT, 4>>({0x4F, 0x50, 0x51, 0x4B})
        ,addInteger<"Hotkey.reset">(0x52)
    ),
    addSection<"InputPanel">(
        addField<"p%d.Enabled", MultiField<IT, 4>>({false, false, false, false})
        ,addField<"p%d.Position", MultiField<PT, 4>>({
            {60, 340}, {410, 340},
            {60, 360}, {410, 360},
        })
    ),
};
```

对应的 INI 长这样（一个 Field ↔ 多行键）：

```ini
[Debug]
Enabled=0
Hotkey.p1=79
Hotkey.p2=80
Hotkey.p3=81
Hotkey.p4=75
Hotkey.reset=82

[InputPanel]
p1.Enabled=0
p2.Enabled=0
...
p4.Position=60, 360
```

### 4.3 读取值（摘自 `src/ReplayInputView++/riv.cpp`）

`operator[]` 检索的是 Field 的**字面键名**（含 `%d`），拿到 Field 后经 `.value`（即 MultiField 对象）再按玩家下标取值：

```cpp
// Field& → .value 得 MultiField& → [].value[i] 取内层值
bool eni = iniProxy["InputPanel"_l]["p%d.Enabled"_l].value[i] != 0;
panels[i]->setPosI(iniProxy["InputPanel"_l]["p%d.Position"_l].value[i]);  // Vector2f

auto& hotkey_value = iniProxy["Debug"_l]["Hotkey.p%d"_l].value;  // MultiField&
if (check_key(hotkey_value[i - 1])) { ... }
```

> 隐式转换对 MultiField 无意义（基类 `value_type` 是恒为 `true` 的占位 bool），**永远用 `.value[i]` 或引用 MultiField 后 `operator[]` 读值**。

## 5. ReplayInputView-- 中的整体用法

模板头文件末尾的便捷别名（照此引用即可，不必写 `cfg::ex::` 全名）：

```cpp
namespace utils {
    using namespace cfg::ex;   // addSection / addField / addBool... / Config / operator""_l
    template<typename Var> using ConfigValueBase = cfg::_supported_types::ValueBase<Var>;
    namespace literal { using cfg::ex::operator""_l; }
}
```

### 5.1 iniProxy 全貌（`src/main.hpp`，节选结构）

| 节 | 键（代表） | 类型 | 说明 |
| --- | --- | --- | --- |
| `Assets` | `File` | string | `.dat` 资源文件名 |
| `Debug` | `Enabled` / `Hotkey.p%d`(MultiField) / `Hotkey.reset` | bool / 4×int / int | 调试面板与逐玩家热键（VK 码，十进制） |
| `InputPanel` | `p%d.Enabled` / `p%d.Position` | 4×int / 4×Point | 输入面板开关与位置 |
| `InputRecord` | 同上 | | 录像输入面板 |
| `BoxDisplay` | `Enabled` / `Floor` / `Hitbox.*` / `p%d.Character` 等 | 混合 | 碰撞盒显示细项（点号仅命名习惯，仍是单键名） |
| `Keys` | `display_boxes` … `framestep` | int | 全局功能键 VK 码 |
| `ColorProfile` | `CollisionBox` / `Hitbox.Melee` … | int(ARGB) | 各色框颜色 |
| `Others` | `ComboMeter.ShowOneHit` | int | 杂项 |

### 5.2 加载流程（摘自 `src/main.cpp` 的 `Initialize`）

```cpp
hDllModule = hMyModule;
GetModulePath(hMyModule, basePath);                    // DLL 所在目录
iniProxy.setPath(toAbsPath(iniProxy.getPath()));       // 相对文件名 → 绝对路径
iniProxy.load();                                       // 一次性读入
cout << "Absolute Ini Path[" << iniProxy.getPath() << "]\n";
```

之后在任意钩子回调里按键读取（内存访问，无 IO）：

```cpp
auto& name = iniProxy["Assets"_l]["File"_l];           // 隐式转换 / .value 皆可
```

要点归纳：

1. **只在 `Initialize` 里 `load()` 一次**，读全是内存访问，每帧钩子放心用；
2. **Config 只写文件名**，`Initialize` 里 `setPath(toAbsPath(...))` 锚定到 DLL 所在目录；
3. MultiField 让逐玩家配置共用一个编译期键名，玩家数由 CMake 宏（`PLAYERS_NUMBER=2|4`）在代码侧统一；
4. 该项目未调用 `save()`——ini 手写发布；若需首次运行生成默认 ini，`load()` 前检测文件不存在时 `save()` 一次。

## 6. 在 MyMod_Template 中使用

头文件已就位：`src/utils/static_config.hpp`。典型接线：

```cpp
// src/main.cpp
#include "main.hpp"
#include "utils/static_config.hpp"
using namespace utils;

namespace {
inline auto iniProxy = Config{
    S_MOD_NAME,                                // 占位文件名，Initialize 时被真实的绝对路径替换
    addSection<"General">(addBool<"Debug">(false))
};
}

namespace mod {
void Initialize() {
    iniProxy.setPath(GetIniPath());            // <模块目录>/<模组名>.ini
    iniProxy.load();
    // if (iniProxy["General"_l]["Debug"_l]) ...
}
}
```

> `Point` 所需的 `SokuLib::Vector2f` 来自 `thirdparty/SokuLib/src/Vector2.hpp`，模板工程 include path 已覆盖，`#include <Vector2.hpp>` 可直接解析。

## 7. 注意事项

- **MultiField 约定**：键名模板参数必须恰含一个 `%d`（`sprintf` 不校验格式与参数个数）；键名+序号展开后不得超过 `char buf[24]`；`initializer_list` 长度不足 N 是 UB（原作注释掉的 assert 可恢复）；改键名或键布局会让老用户 ini 静默失效。
- **Bool 即 Integer**：`addBool` 持久化为 `0`/`1`，INI 中写 `true`/`false` 不会被识别（解析失败时返回默认值）。注意原作 `MultiField<IT, 4>({false, ...})` 传 `bool` 正是依赖 `Integer(bool)` 构造。
- **整数一律十进制**：`Color`/VK 码等写进 INI 都是十进制；`GetPrivateProfileIntA` 不接受 `0x` 前缀。
- **String 上限 1024 字节**：超长值被截断。
- **无自动重载**：`load()` 后外部修改 INI 不会生效，需要再调 `load()`。
- **写盘时机**：改 `iniProxy` 只改内存；`save()` 全量重写 INI（覆盖注释与排版）；或者调用特定 section/field 的write。
- **ANSI 限制**：底层是 `...A` 系列 API，路径与键值按系统 ANSI 代码页处理；键名/节名建议 ASCII。
