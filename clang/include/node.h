#ifndef SIMPLE_LANGUAGE_NODE_H
#define SIMPLE_LANGUAGE_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "define.h"
#include "token.h"
#include <stddef.h>

// 前向声明
typedef struct Node Node;
typedef struct NodeList NodeList;

// 节点链表结构
struct NodeList {
    Node* node;
    NodeList* next;
};

// 节点结构体
typedef struct Node {
    int priority;                          // 优先级
    bool is_del;                          // 是否删除
    
    Token* token;                         // 当前token
    Token* end_token;                     // 结束token )}]
    Node* parent;                         // 父节点
    Node* par_node;                       // (小括号的节点
    Node* block_node;                     // {大括号的节点
    Node* bracket_node;                   // [中括号的节点
    Node* angle_node;                     // < > 节点
    Token* link_token;                    // .节点
    Token* at_token;                      // @节点
    Node* last_node;                      // 最后处理的节点
    
    ENodeType node_type;                  // 节点类型
    NodeList* extend_link_node_list;      // 扩展链接节点列表
    NodeList* child_list;                 // 子内容节点
    
    int parse_index;                      // 解析索引
} Node;

// Node创建和销毁函数
Node* node_create(void);
void node_destroy(Node* node);

// Node属性设置函数
void node_set_priority(Node* node, int priority);
void node_set_is_del(Node* node, bool is_del);
void node_set_token(Node* node, Token* token);
void node_set_end_token(Node* node, Token* end_token);
void node_set_parent(Node* node, Node* parent);
void node_set_par_node(Node* node, Node* par_node);
void node_set_block_node(Node* node, Node* block_node);
void node_set_bracket_node(Node* node, Node* bracket_node);
void node_set_angle_node(Node* node, Node* angle_node);
void node_set_link_token(Node* node, Token* link_token);
void node_set_at_token(Node* node, Token* at_token);
void node_set_last_node(Node* node, Node* last_node);
void node_set_node_type(Node* node, ENodeType node_type);
void node_set_parse_index(Node* node, int parse_index);

// Node子节点操作
void node_add_child(Node* node, Node* child);
Node* node_get_child(const Node* node, int index);
int node_get_child_count(const Node* node);
Node* node_get_parse_current(const Node* node);
Node* node_get_final_node(const Node* node);

// Node扩展链接节点操作
void node_add_extend_link_node(Node* node, Node* extend_node);
Node* node_get_extend_link_node(const Node* node, int index);
int node_get_extend_link_node_count(const Node* node);

// Node链表操作
NodeList* node_list_create(Node* node);
void node_list_destroy(NodeList* list);
void node_list_add(NodeList** list, Node* node);
Node* node_list_get(const NodeList* list, int index);
int node_list_get_count(const NodeList* list);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_NODE_H 