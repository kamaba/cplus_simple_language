# SimpleLanguage C++ 版本

这是从 C# 版本转换而来的 C++11 实现。

## 已完成的核心文件

### 基础定义
- `Define.h/cpp` - 枚举类型和全局定义
- `Core/MetaBase.h/cpp` - 元数据基类
- `Core/MetaNode.h/cpp` - 元数据节点
- `Core/MetaClass.h` - 元数据类（头文件）
- `Core/MetaType.h/cpp` - 元数据类型
- `Core/MetaTemplate.h/cpp` - 模板类
- `Core/MetaModule.h/cpp` - 模块类
- `Core/MetaNamespace.h/cpp` - 命名空间类
- `Core/MetaData.h/cpp` - 数据类
- `Core/MetaEnum.h/cpp` - 枚举类

### 成员相关
- `Core/MetaMemberVariable.h/cpp` - 成员变量
- `Core/MetaMemberFunction.h/cpp` - 成员函数
- `Core/MetaExpressNode.h/cpp` - 表达式节点
- `Core/MetaDefineParam.h` - 参数定义

### 编译相关
- `Compile/Token.h/cpp` - Token 类
- `Compile/CompileManager.h/cpp` - 编译管理器

## 编译说明

使用 CMake 编译：

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

或使用 Visual Studio：
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
```

## 注意事项

1. 由于项目规模较大（100+个文件），当前提供的是核心框架
2. 部分类的实现使用了简化版本（如使用 void* 代替具体类型）
3. 需要根据实际需求补充完整的类实现
4. 建议按模块逐步完善：Core -> Compile -> Parse -> IR -> VM

## 目录结构

```
source/
├── Core/              # 核心元数据类
├── Compile/           # 编译相关类
├── Debug/             # 调试日志类（待实现）
├── Export/            # 导出相关类（待实现）
├── IR/                # 中间表示类（待实现）
├── OtherLanguage/     # 其他语言支持（待实现）
├── Project/           # 项目管理类（待实现）
├── VM/                # 虚拟机类（待实现）
└── CMakeLists.txt     # CMake 构建文件
```

## 待完成的工作

由于源代码包含超过100个文件，完整转换需要：
1. 补充所有Parse相关类（词法分析、语法分析）
2. 补充所有IR相关类（中间表示）
3. 补充所有VM相关类（虚拟机执行）
4. 补充所有Export相关类（代码生成）
5. 实现完整的类型系统和表达式系统

建议采用增量式开发，先实现核心功能后再扩展。

