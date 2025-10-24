#!/bin/bash

# Simple Language C Implementation 构建脚本

echo "========================================="
echo "Simple Language C Implementation 构建脚本"
echo "========================================="

# 检查依赖
echo "检查编译依赖..."

# 检查GCC
if ! command -v gcc &> /dev/null; then
    echo "错误: 未找到GCC编译器"
    echo "请安装GCC: sudo apt-get install build-essential"
    exit 1
fi

echo "✓ 找到GCC编译器: $(gcc --version | head -n1)"

# 检查Make
if ! command -v make &> /dev/null; then
    echo "错误: 未找到Make工具"
    echo "请安装Make: sudo apt-get install make"
    exit 1
fi

echo "✓ 找到Make工具: $(make --version | head -n1)"

# 检查libuv
if ! pkg-config --exists libuv; then
    echo "警告: 未找到libuv开发库"
    echo "请安装libuv: sudo apt-get install libuv1-dev"
    echo "或者从源码编译: https://github.com/libuv/libuv"
else
    echo "✓ 找到libuv: $(pkg-config --modversion libuv)"
fi

# 检查eglib
if ! pkg-config --exists eglib; then
    echo "警告: 未找到eglib开发库"
    echo "请安装eglib: sudo apt-get install libeglib-dev"
    echo "或者从源码编译: https://github.com/mono/eglib"
else
    echo "✓ 找到eglib: $(pkg-config --modversion eglib)"
fi

echo ""
echo "开始编译..."

# 清理之前的构建
echo "清理之前的构建..."
make clean

# 编译项目
echo "编译项目..."
if make; then
    echo ""
    echo "✓ 编译成功！"
    echo ""
    echo "可执行文件位置: bin/simple_lang"
    echo ""
    echo "运行测试:"
    echo "  bin/simple_lang"
    echo ""
    echo "运行单元测试:"
    echo "  gcc -std=c99 -I./include -o test/test_basic test/test_basic.c src/*.c -lm"
    echo "  test/test_basic"
else
    echo ""
    echo "✗ 编译失败！"
    echo ""
    echo "可能的解决方案:"
    echo "1. 安装缺失的依赖库"
    echo "2. 检查编译器版本 (需要GCC 4.9+)"
    echo "3. 检查头文件路径"
    echo ""
    exit 1
fi

echo "构建脚本完成！" 