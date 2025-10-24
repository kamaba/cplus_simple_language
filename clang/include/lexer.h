#ifndef SIMPLE_LANGUAGE_LEXER_H
#define SIMPLE_LANGUAGE_LEXER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "token.h"
#include <stdio.h>

// 词法分析器状态
typedef enum {
    LEXER_STATE_INITIAL,
    LEXER_STATE_IDENTIFIER,
    LEXER_STATE_NUMBER,
    LEXER_STATE_STRING,
    LEXER_STATE_COMMENT,
    LEXER_STATE_OPERATOR,
    LEXER_STATE_WHITESPACE
} LexerState;

// 词法分析器结构
typedef struct Lexer {
    FILE* source_file;                    // 源文件
    char* source_code;                    // 源代码字符串
    size_t source_length;                 // 源代码长度
    size_t current_pos;                   // 当前位置
    int current_line;                     // 当前行号
    int current_column;                   // 当前列号
    char current_char;                    // 当前字符
    LexerState state;                     // 当前状态
    char* file_path;                     // 文件路径
    Token* current_token;                 // 当前token
    Token* token_list;                    // token列表
    Token* last_token;                    // 最后一个token
} Lexer;

// 词法分析器创建和销毁函数
Lexer* lexer_create_from_file(const char* file_path);
Lexer* lexer_create_from_string(const char* source_code, const char* file_path);
void lexer_destroy(Lexer* lexer);

// 词法分析器主要函数
Token* lexer_get_next_token(Lexer* lexer);
Token* lexer_get_all_tokens(Lexer* lexer);
bool lexer_has_more_tokens(const Lexer* lexer);

// 词法分析器状态函数
void lexer_set_state(Lexer* lexer, LexerState state);
LexerState lexer_get_state(const Lexer* lexer);

// 词法分析器位置函数
int lexer_get_current_line(const Lexer* lexer);
int lexer_get_current_column(const Lexer* lexer);
size_t lexer_get_current_pos(const Lexer* lexer);

// 词法分析器字符函数
char lexer_get_current_char(const Lexer* lexer);
char lexer_peek_next_char(const Lexer* lexer);
char lexer_peek_char_at(const Lexer* lexer, size_t offset);
void lexer_advance_char(Lexer* lexer);
void lexer_skip_whitespace(Lexer* lexer);
void lexer_skip_line_comment(Lexer* lexer);
void lexer_skip_block_comment(Lexer* lexer);

// 词法分析器工具函数
bool lexer_is_identifier_start(char c);
bool lexer_is_identifier_part(char c);
bool lexer_is_digit(char c);
bool lexer_is_hex_digit(char c);
bool lexer_is_octal_digit(char c);
bool lexer_is_whitespace(char c);
bool lexer_is_newline(char c);
bool lexer_is_operator_start(char c);

// 词法分析器错误处理
void lexer_report_error(Lexer* lexer, const char* message);
void lexer_report_warning(Lexer* lexer, const char* message);

// 词法分析器文件操作
bool lexer_open_file(Lexer* lexer, const char* file_path);
void lexer_close_file(Lexer* lexer);
bool lexer_is_file_open(const Lexer* lexer);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_LEXER_H 