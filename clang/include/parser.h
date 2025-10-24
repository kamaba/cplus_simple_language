#ifndef SIMPLE_LANGUAGE_PARSER_H
#define SIMPLE_LANGUAGE_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lexer.h"
#include "node.h"
#include "token.h"

// 语法分析器结构
typedef struct Parser {
    Lexer* lexer;                        // 词法分析器
    Token* current_token;                 // 当前token
    Token* lookahead_token;               // 前瞻token
    Node* root_node;                      // 根节点
    Node* current_node;                   // 当前节点
    int error_count;                      // 错误数量
    int warning_count;                    // 警告数量
    char* error_message;                  // 错误消息
    bool has_error;                       // 是否有错误
} Parser;

// 语法分析器创建和销毁函数
Parser* parser_create(Lexer* lexer);
void parser_destroy(Parser* parser);

// 语法分析器主要函数
Node* parser_parse(Parser* parser);
Node* parser_parse_expression(Parser* parser);
Node* parser_parse_statement(Parser* parser);
Node* parser_parse_block(Parser* parser);
Node* parser_parse_class_declaration(Parser* parser);
Node* parser_parse_function_declaration(Parser* parser);
Node* parser_parse_variable_declaration(Parser* parser);

// 语法分析器表达式解析函数
Node* parser_parse_primary_expression(Parser* parser);
Node* parser_parse_postfix_expression(Parser* parser);
Node* parser_parse_unary_expression(Parser* parser);
Node* parser_parse_multiplicative_expression(Parser* parser);
Node* parser_parse_additive_expression(Parser* parser);
Node* parser_parse_shift_expression(Parser* parser);
Node* parser_parse_relational_expression(Parser* parser);
Node* parser_parse_equality_expression(Parser* parser);
Node* parser_parse_and_expression(Parser* parser);
Node* parser_parse_exclusive_or_expression(Parser* parser);
Node* parser_parse_inclusive_or_expression(Parser* parser);
Node* parser_parse_logical_and_expression(Parser* parser);
Node* parser_parse_logical_or_expression(Parser* parser);
Node* parser_parse_conditional_expression(Parser* parser);
Node* parser_parse_assignment_expression(Parser* parser);

// 语法分析器语句解析函数
Node* parser_parse_expression_statement(Parser* parser);
Node* parser_parse_compound_statement(Parser* parser);
Node* parser_parse_selection_statement(Parser* parser);
Node* parser_parse_iteration_statement(Parser* parser);
Node* parser_parse_jump_statement(Parser* parser);
Node* parser_parse_declaration_statement(Parser* parser);

// 语法分析器声明解析函数
Node* parser_parse_declaration(Parser* parser);
Node* parser_parse_init_declarator_list(Parser* parser);
Node* parser_parse_init_declarator(Parser* parser);
Node* parser_parse_declarator(Parser* parser);
Node* parser_parse_direct_declarator(Parser* parser);
Node* parser_parse_pointer(Parser* parser);
Node* parser_parse_type_specifier(Parser* parser);
Node* parser_parse_struct_declaration(Parser* parser);
Node* parser_parse_enum_declaration(Parser* parser);

// 语法分析器类型解析函数
Node* parser_parse_type_name(Parser* parser);
Node* parser_parse_abstract_declarator(Parser* parser);
Node* parser_parse_direct_abstract_declarator(Parser* parser);

// 语法分析器参数解析函数
Node* parser_parse_parameter_list(Parser* parser);
Node* parser_parse_parameter_declaration(Parser* parser);
Node* parser_parse_argument_expression_list(Parser* parser);

// 语法分析器初始化解析函数
Node* parser_parse_initializer(Parser* parser);
Node* parser_parse_initializer_list(Parser* parser);

// 语法分析器工具函数
Token* parser_get_current_token(const Parser* parser);
Token* parser_get_lookahead_token(const Parser* parser);
void parser_advance_token(Parser* parser);
bool parser_match_token(Parser* parser, ETokenType expected_type);
bool parser_peek_token(Parser* parser, ETokenType expected_type);
bool parser_peek_token_at(Parser* parser, size_t offset, ETokenType expected_type);

// 语法分析器错误处理函数
void parser_report_error(Parser* parser, const char* message);
void parser_report_warning(Parser* parser, const char* message);
void parser_sync_error_recovery(Parser* parser);
bool parser_has_error(const Parser* parser);
int parser_get_error_count(const Parser* parser);
int parser_get_warning_count(const Parser* parser);
const char* parser_get_error_message(const Parser* parser);

// 语法分析器节点创建函数
Node* parser_create_expression_node(Parser* parser, ENodeType node_type, Token* token);
Node* parser_create_statement_node(Parser* parser, ENodeType node_type, Token* token);
Node* parser_create_declaration_node(Parser* parser, ENodeType node_type, Token* token);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_PARSER_H 