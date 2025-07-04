# C++ ThreadPool with Dynamic Scaling and Logger

## 🚀 项目简介

本项目是一个使用现代 C++17 编写的高性能线程池，具备以下特性：

- 支持任务提交并返回 `std::future` 结果
- 支持线程池 **动态扩容与收缩**（基于任务与空闲线程数量）
- 自研轻量级 **日志系统**，记录线程 ID、时间戳、函数位置等信息
- 使用 `CMake` 构建，支持跨平台编译与调试
- 代码结构清晰，适合作为面试项目或实际工程基础框架

---

## 🔧 技术栈

- **C++17**
- **线程原语**：`std::thread`, `std::mutex`, `std::condition_variable`
- **异步支持**：`std::future`, `std::packaged_task`
- **日志系统**：基于 `std::fstream` 和宏封装
- **构建工具**：`CMake`

---

## 📆 项目结构

```bash
.
├── include/                # 公共头文件
│   └── ThreadPool.h        # 线程池接口
│   └── Logger.h            # 日志接口
├── src/                    # 源码实现
│   └── ThreadPool.cpp      # 线程池实现
│   └── Logger.cpp          # 日志实现
├── example/                # 使用示例
│   └── main.cpp
├── test/                   # 测试文件
│   └── test_main.cpp       # 测试线程池功能
├── CMakeLists.txt          # 构建脚本
└── README.md               # 项目说明
```

---

## 📅 使用方法

### 构建

```bash
git clone https://github.com/your-username/cpp-threadpool.git
cd cpp-threadpool
mkdir build && cd build
cmake ..
make
```

### 运行示例

```bash
./main
```

---

## 🤷 核心API

### 线程池初始化
```cpp
ThreadPool pool(2, 8); // 最小2，最大8线程
```

### 提交任务
```cpp
auto future = pool.submit([](int a, int b) {
    return a + b;
}, 2, 3);

std::cout << future.get(); // 返回 5
```

---

## 💡 特性

- 较实际场景考虑：线程负载大时自动扩容，任务空闲时自动收缩
- 支持异常捕捉，避免线程因任务错误失效
- 日志包含时间戳/线程ID/文件行/函数名，便于调试

---

## 📃 试验设计

```cpp
// test/test_main.cpp
#include "ThreadPool.h"
#include <iostream>

int main() {
    ThreadPool pool(2, 6);

    std::vector<std::future<int>> results;
    for (int i = 0; i < 10; ++i) {
        results.emplace_back(pool.submit([i](){
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return i * i;
        }));
    }

    for (auto& f : results) {
        std::cout << "Result: " << f.get() << std::endl;
    }
    return 0;
}
```

---

## 📖 文档和运行环境

- C++17 或更高
- 系统: Linux / macOS / Windows (WSL)
- G++ >= 7.0 或 Clang >= 6.0
- 建议配合 `htop`, `valgrind`, `gdb` 进行分析

---

## 🌟 本项目适合

- C++ 面试考核项目应用
- 多线程/实时/异步相关代码基础
- 学习 Singleton 模式/日志/封装/异步编程
