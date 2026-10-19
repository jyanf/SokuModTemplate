# distribution

存放仓库分发的文件：

- `export.hpp`：对其他 DLL 模组公开的 C ABI 头文件（已在 CMake 的 include 路径中）。
- 之后可以在这里放随模组分发的配置模板、文档、示例脚本等。

`distribution/` 本身不参与编译（`export.hpp` 除外），只作为对外接口与分发内容的集中位置。
