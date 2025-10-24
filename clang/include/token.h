#ifndef SIMPLE_LANGUAGE_TOKEN_H
#define SIMPLE_LANGUAGE_TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "define.h"
#include <stddef.h>

// Token结构体
typedef struct Token {
    char path[MAX_PATH_LENGTH];           // 文件路径
    ETokenType type;                     // 标记类型
    union {
        char* string_value;               // 字符串值
        int64_t int_value;               // 整数值
        double float_value;               // 浮点值
        bool bool_value;                  // 布尔值
        void* ptr_value;                  // 指针值
    } lexeme;                            // 标记值
    void* extend;                        // 辅助标记，可为空
    int source_begin_line;               // 开始所在行
    int source_begin_char;               // 开始所在列
    int source_end_line;                 // 结束所在行
    int source_end_char;                 // 结束所在列
    struct Token* next;                  // 下一个token
} Token;

// Token创建和销毁函数
Token* token_create(const char* path, ETokenType token_type, void* lexeme, 
                    int source_line, int source_char, void* extend);
void token_destroy(Token* token);

// Token设置函数
void token_set_source_end(Token* token, int end_source_line, int end_source_char);
void token_set_type(Token* token, ETokenType type);
void token_set_lexeme(Token* token, void* lexeme, ETokenType token_type);
void token_set_extend(Token* token, void* extend);
void token_set_bind_file_path(Token* token, const char* path);

// Token获取函数
EType token_get_etype(const Token* token);
const char* token_to_string(const Token* token);
const char* token_to_all_string(const Token* token);
const char* token_to_lexeme_all_string(const Token* token);

// Token链表操作
void token_add_next(Token* token, Token* next_token);
Token* token_get_next(const Token* token);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_TOKEN_H 