#ifndef SIMPLE_LANGUAGE_H
#define SIMPLE_LANGUAGE_H

#ifdef __cplusplus
extern "C" {
#endif

// 包含所有必要的头文件
#include "define.h"
#include "token.h"
#include "node.h"
#include "meta_base.h"
#include "meta_class.h"
#include "vm_object.h"
#include "lexer.h"
#include "parser.h"
#include "vm.h"

// 编译器结构
typedef struct Compiler {
    Lexer* lexer;                        // 词法分析器
    Parser* parser;                       // 语法分析器
    VM* vm;                              // 虚拟机
    Node* ast_root;                      // 抽象语法树根节点
    bool is_compiled;                     // 是否已编译
    int error_count;                      // 错误数量
    int warning_count;                    // 警告数量
} Compiler;

// 编译器创建和销毁函数
Compiler* compiler_create(void);
void compiler_destroy(Compiler* compiler);

// 编译器主要函数
bool compiler_compile_file(Compiler* compiler, const char* file_path);
bool compiler_compile_string(Compiler* compiler, const char* source_code);
bool compiler_execute(Compiler* compiler);
bool compiler_run_file(Compiler* compiler, const char* file_path);

// 编译器状态查询函数
bool compiler_is_compiled(const Compiler* compiler);
int compiler_get_error_count(const Compiler* compiler);
int compiler_get_warning_count(const Compiler* compiler);
bool compiler_has_errors(const Compiler* compiler);

// 编译器AST访问函数
Node* compiler_get_ast_root(const Compiler* compiler);
void compiler_print_ast(const Compiler* compiler);

// 编译器错误处理函数
void compiler_clear_errors(Compiler* compiler);
const char* compiler_get_last_error(const Compiler* compiler);

// 全局初始化函数
bool simple_language_initialize(void);
void simple_language_shutdown(void);

// 版本信息函数
const char* simple_language_get_version(void);
const char* simple_language_get_build_date(void);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_H 