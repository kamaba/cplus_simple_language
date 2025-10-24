#include "../include/token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Token创建函数
Token* token_create(const char* path, ETokenType token_type, void* lexeme, 
                    int source_line, int source_char, void* extend) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (!token) {
        return NULL;
    }
    
    // 初始化路径
    if (path) {
        strncpy(token->path, path, MAX_PATH_LENGTH - 1);
        token->path[MAX_PATH_LENGTH - 1] = '\0';
    } else {
        token->path[0] = '\0';
    }
    
    // 初始化其他字段
    token->type = token_type;
    token->source_begin_line = source_line + 1;
    token->source_begin_char = source_char;
    token->source_end_line = source_line + 1;
    token->source_end_char = source_char;
    token->extend = extend;
    token->next = NULL;
    
    // 根据类型设置lexeme
    switch (token_type) {
        case TOKEN_STRING:
        case TOKEN_IDENTIFIER:
            if (lexeme) {
                token->lexeme.string_value = strdup((char*)lexeme);
            } else {
                token->lexeme.string_value = NULL;
            }
            break;
        case TOKEN_NUMBER:
            if (lexeme) {
                token->lexeme.int_value = *(int64_t*)lexeme;
            } else {
                token->lexeme.int_value = 0;
            }
            break;
        case TOKEN_BOOLEAN:
        case TOKEN_BOOL_VALUE:
            if (lexeme) {
                token->lexeme.bool_value = *(bool*)lexeme;
            } else {
                token->lexeme.bool_value = false;
            }
            break;
        default:
            token->lexeme.ptr_value = lexeme;
            break;
    }
    
    return token;
}

// Token销毁函数
void token_destroy(Token* token) {
    if (!token) {
        return;
    }
    
    // 释放字符串值
    if (token->type == TOKEN_STRING || token->type == TOKEN_IDENTIFIER) {
        if (token->lexeme.string_value) {
            free(token->lexeme.string_value);
        }
    }
    
    free(token);
}

// Token设置源结束位置
void token_set_source_end(Token* token, int end_source_line, int end_source_char) {
    if (token) {
        token->source_end_line = end_source_line;
        token->source_end_char = end_source_char;
    }
}

// Token设置类型
void token_set_type(Token* token, ETokenType type) {
    if (token) {
        token->type = type;
    }
}

// Token设置lexeme
void token_set_lexeme(Token* token, void* lexeme, ETokenType token_type) {
    if (!token) {
        return;
    }
    
    // 释放旧的字符串值
    if (token->type == TOKEN_STRING || token->type == TOKEN_IDENTIFIER) {
        if (token->lexeme.string_value) {
            free(token->lexeme.string_value);
        }
    }
    
    token->type = token_type;
    
    // 根据新类型设置lexeme
    switch (token_type) {
        case TOKEN_STRING:
        case TOKEN_IDENTIFIER:
            if (lexeme) {
                token->lexeme.string_value = strdup((char*)lexeme);
            } else {
                token->lexeme.string_value = NULL;
            }
            break;
        case TOKEN_NUMBER:
            if (lexeme) {
                token->lexeme.int_value = *(int64_t*)lexeme;
            } else {
                token->lexeme.int_value = 0;
            }
            break;
        case TOKEN_BOOLEAN:
        case TOKEN_BOOL_VALUE:
            if (lexeme) {
                token->lexeme.bool_value = *(bool*)lexeme;
            } else {
                token->lexeme.bool_value = false;
            }
            break;
        default:
            token->lexeme.ptr_value = lexeme;
            break;
    }
}

// Token设置扩展信息
void token_set_extend(Token* token, void* extend) {
    if (token) {
        token->extend = extend;
    }
}

// Token设置绑定文件路径
void token_set_bind_file_path(Token* token, const char* path) {
    if (token && path) {
        strncpy(token->path, path, MAX_PATH_LENGTH - 1);
        token->path[MAX_PATH_LENGTH - 1] = '\0';
    }
}

// Token获取EType
EType token_get_etype(const Token* token) {
    if (!token) {
        return TYPE_NONE;
    }
    
    switch (token->type) {
        case TOKEN_NUMBER:
            if (token->extend) {
                return *(EType*)token->extend;
            }
            return TYPE_INT32;
        case TOKEN_TYPE:
            if (token->extend) {
                return *(EType*)token->extend;
            }
            return TYPE_NONE;
        case TOKEN_BOOLEAN:
        case TOKEN_BOOL_VALUE:
            return TYPE_BOOLEAN;
        case TOKEN_STRING:
            return TYPE_STRING;
        case TOKEN_NULL:
            return TYPE_NULL;
        case TOKEN_VOID:
            return TYPE_VOID;
        default:
            return TYPE_NONE;
    }
}

// Token转换为字符串
const char* token_to_string(const Token* token) {
    if (!token) {
        return "NULL";
    }
    
    static char buffer[1024];
    switch (token->type) {
        case TOKEN_STRING:
        case TOKEN_IDENTIFIER:
            if (token->lexeme.string_value) {
                return token->lexeme.string_value;
            }
            return "";
        case TOKEN_NUMBER:
            snprintf(buffer, sizeof(buffer), "%lld", token->lexeme.int_value);
            return buffer;
        case TOKEN_BOOLEAN:
        case TOKEN_BOOL_VALUE:
            return token->lexeme.bool_value ? "true" : "false";
        default:
            return "unknown";
    }
}

// Token转换为完整信息字符串
const char* token_to_all_string(const Token* token) {
    if (!token) {
        return "NULL";
    }
    
    static char buffer[2048];
    snprintf(buffer, sizeof(buffer), "Path:%s Line: %d Pos: %d", 
             token->path, token->source_begin_line, token->source_begin_char);
    return buffer;
}

// Token转换为词素完整信息字符串
const char* token_to_lexeme_all_string(const Token* token) {
    if (!token) {
        return "NULL";
    }
    
    static char buffer[2048];
    snprintf(buffer, sizeof(buffer), "Lex: %s Path:%s Line: %d Pos: %d", 
             token_to_string(token), token->path, token->source_begin_line, token->source_begin_char);
    return buffer;
}

// Token添加下一个token
void token_add_next(Token* token, Token* next_token) {
    if (token && next_token) {
        token->next = next_token;
    }
}

// Token获取下一个token
Token* token_get_next(const Token* token) {
    return token ? token->next : NULL;
} 