# Simple Language C Implementation

这是Simple Language编程语言的C99标准重新实现版本。项目使用libuv和eglib库，提供了一个完整的编程语言实现，包括词法分析器、语法分析器、元数据系统和虚拟机。

## 项目结构

```
clang/
├── include/           # 头文件目录
│   ├── define.h      # 定义和枚举
│   ├── token.h       # Token结构定义
│   ├── node.h        # AST节点定义
│   ├── meta_base.h   # 元数据基类
│   ├── meta_class.h  # 元数据类
│   ├── vm_object.h   # 虚拟机对象
│   ├── lexer.h       # 词法分析器
│   ├── parser.h      # 语法分析器
│   ├── vm.h          # 虚拟机
│   └── simple_language.h  # 主头文件
├── src/              # 源文件目录
│   ├── token.c       # Token实现
│   ├── node.c        # Node实现
│   ├── meta_base.c   # MetaBase实现
│   ├── vm_object.c   # VM对象实现
│   └── ...           # 其他源文件
├── lib/              # 库文件目录
├── test/             # 测试文件目录
├── Makefile          # 编译配置
└── README.md         # 项目说明
```

## 特性

- **C99标准**: 完全符合C99标准，确保跨平台兼容性
- **模块化设计**: 清晰的模块分离，便于维护和扩展
- **内存管理**: 手动内存管理，支持引用计数
- **错误处理**: 完善的错误处理和报告机制
- **调试支持**: 内置调试和日志功能

## 依赖库

- **libuv**: 异步I/O和事件循环
- **eglib**: 通用库，提供字符串、集合等基础功能
- **标准C库**: 标准C库函数

## 编译要求

- GCC 4.9+ 或 Clang 3.5+
- Make 工具
- libuv 开发库
- eglib 开发库

## 安装依赖

### Ubuntu/Debian
```bash
make install-deps
```

### CentOS/RHEL
```bash
make install-deps-centos
```

### macOS
```bash
make install-deps-macos
```

## 编译项目

```bash
# 编译项目
make

# 编译调试版本
make debug

# 编译发布版本
make release

# 清理编译文件
make clean
```

## 使用方法

### 基本用法

```c
#include "simple_language.h"

int main() {
    // 初始化Simple Language
    if (!simple_language_initialize()) {
        printf("初始化失败\n");
        return 1;
    }
    
    // 创建编译器
    Compiler* compiler = compiler_create();
    if (!compiler) {
        printf("创建编译器失败\n");
        return 1;
    }
    
    // 编译文件
    if (!compiler_compile_file(compiler, "test.sl")) {
        printf("编译失败: %s\n", compiler_get_last_error(compiler));
        compiler_destroy(compiler);
        return 1;
    }
    
    // 执行代码
    if (!compiler_execute(compiler)) {
        printf("执行失败\n");
    }
    
    // 清理资源
    compiler_destroy(compiler);
    simple_language_shutdown();
    
    return 0;
}
```

### 语言特性示例

```c
// 类定义
class Person {
    string name;
    int age;
    
    function Person(string n, int a) {
        name = n;
        age = a;
    }
    
    function sayHello() {
        print("Hello, I'm " + name);
    }
}

// 主函数
function main() {
    Person p = new Person("Alice", 25);
    p.sayHello();
}
```

## 测试

运行测试套件：

```bash
make test
```

## 调试

启用调试模式：

```bash
make debug
```

## 静态分析

运行静态分析：

```bash
make analyze
```

## 贡献

欢迎提交Issue和Pull Request来改进项目。

## 许可证

本项目采用MIT许可证。

## 联系方式

如有问题或建议，请通过以下方式联系：

- 提交GitHub Issue
- 发送邮件至项目维护者

## 更新日志

### v1.0.0
- 初始版本发布
- 实现基本的词法分析器
- 实现基本的语法分析器
- 实现基本的虚拟机
- 支持基本的语言特性 