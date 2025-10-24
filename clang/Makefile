# Simple Language C Implementation Makefile
# 使用C99标准，支持libuv和eglib

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -g
INCLUDES = -I./include
LIBS = -luv -leglib -lm

# 源文件目录
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# 源文件列表
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# 目标可执行文件
TARGET = $(BINDIR)/simple_lang

# 默认目标
all: directories $(TARGET)

# 创建必要的目录
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

# 链接目标文件
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "编译完成: $@"

# 编译源文件
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 清理目标
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "清理完成"

# 安装依赖（Ubuntu/Debian）
install-deps:
	sudo apt-get update
	sudo apt-get install -y libuv1-dev libeglib-dev build-essential

# 安装依赖（CentOS/RHEL）
install-deps-centos:
	sudo yum install -y libuv-devel eglib-devel gcc make

# 安装依赖（macOS）
install-deps-macos:
	brew install libuv eglib

# 运行测试
test: $(TARGET)
	@echo "运行测试..."
	@cd test && ./run_tests.sh

# 调试版本
debug: CFLAGS += -DDEBUG -g3 -O0
debug: all

# 发布版本
release: CFLAGS += -DNDEBUG -O3
release: all

# 静态分析
analyze: CFLAGS += -fanalyzer
analyze: clean all

# 帮助信息
help:
	@echo "可用的目标:"
	@echo "  all          - 编译项目"
	@echo "  clean        - 清理编译文件"
	@echo "  install-deps - 安装依赖（Ubuntu/Debian）"
	@echo "  test         - 运行测试"
	@echo "  debug        - 编译调试版本"
	@echo "  release      - 编译发布版本"
	@echo "  analyze      - 静态分析"
	@echo "  help         - 显示此帮助信息"

# 声明伪目标
.PHONY: all clean install-deps install-deps-centos install-deps-macos test debug release analyze help directories 