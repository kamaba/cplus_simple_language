#include "../include/node.h"
#include <stdlib.h>
#include <string.h>

// Node创建函数
Node* node_create(void) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    
    // 初始化所有字段
    node->priority = -1;
    node->is_del = false;
    node->token = NULL;
    node->end_token = NULL;
    node->parent = NULL;
    node->par_node = NULL;
    node->block_node = NULL;
    node->bracket_node = NULL;
    node->angle_node = NULL;
    node->link_token = NULL;
    node->at_token = NULL;
    node->last_node = NULL;
    node->node_type = NODE_NONE;
    node->extend_link_node_list = NULL;
    node->child_list = NULL;
    node->parse_index = 0;
    
    return node;
}

// Node销毁函数
void node_destroy(Node* node) {
    if (!node) {
        return;
    }
    
    // 销毁子节点
    NodeList* child_iter = node->child_list;
    while (child_iter) {
        NodeList* next = child_iter->next;
        node_destroy(child_iter->node);
        free(child_iter);
        child_iter = next;
    }
    
    // 销毁扩展链接节点
    NodeList* extend_iter = node->extend_link_node_list;
    while (extend_iter) {
        NodeList* next = extend_iter->next;
        free(extend_iter);
        extend_iter = next;
    }
    
    free(node);
}

// Node属性设置函数
void node_set_priority(Node* node, int priority) {
    if (node) {
        node->priority = priority;
    }
}

void node_set_is_del(Node* node, bool is_del) {
    if (node) {
        node->is_del = is_del;
    }
}

void node_set_token(Node* node, Token* token) {
    if (node) {
        node->token = token;
    }
}

void node_set_end_token(Node* node, Token* end_token) {
    if (node) {
        node->end_token = end_token;
    }
}

void node_set_parent(Node* node, Node* parent) {
    if (node) {
        node->parent = parent;
    }
}

void node_set_par_node(Node* node, Node* par_node) {
    if (node) {
        node->par_node = par_node;
    }
}

void node_set_block_node(Node* node, Node* block_node) {
    if (node) {
        node->block_node = block_node;
    }
}

void node_set_bracket_node(Node* node, Node* bracket_node) {
    if (node) {
        node->bracket_node = bracket_node;
    }
}

void node_set_angle_node(Node* node, Node* angle_node) {
    if (node) {
        node->angle_node = angle_node;
    }
}

void node_set_link_token(Node* node, Token* link_token) {
    if (node) {
        node->link_token = link_token;
    }
}

void node_set_at_token(Node* node, Token* at_token) {
    if (node) {
        node->at_token = at_token;
    }
}

void node_set_last_node(Node* node, Node* last_node) {
    if (node) {
        node->last_node = last_node;
    }
}

void node_set_node_type(Node* node, ENodeType node_type) {
    if (node) {
        node->node_type = node_type;
    }
}

void node_set_parse_index(Node* node, int parse_index) {
    if (node) {
        node->parse_index = parse_index;
    }
}

// Node子节点操作
void node_add_child(Node* node, Node* child) {
    if (!node || !child) {
        return;
    }
    
    NodeList* new_child = node_list_create(child);
    if (!new_child) {
        return;
    }
    
    if (!node->child_list) {
        node->child_list = new_child;
    } else {
        node_list_add(&node->child_list, child);
    }
}

Node* node_get_child(const Node* node, int index) {
    if (!node || index < 0) {
        return NULL;
    }
    
    return node_list_get(node->child_list, index);
}

int node_get_child_count(const Node* node) {
    if (!node) {
        return 0;
    }
    
    return node_list_get_count(node->child_list);
}

Node* node_get_parse_current(const Node* node) {
    if (!node || node->parse_index >= node_get_child_count(node)) {
        return NULL;
    }
    
    return node_get_child(node, node->parse_index);
}

Node* node_get_final_node(const Node* node) {
    if (!node) {
        return NULL;
    }
    
    if (node->extend_link_node_list) {
        NodeList* last = node->extend_link_node_list;
        while (last->next) {
            last = last->next;
        }
        return last->node;
    }
    
    return node;
}

// Node扩展链接节点操作
void node_add_extend_link_node(Node* node, Node* extend_node) {
    if (!node || !extend_node) {
        return;
    }
    
    NodeList* new_extend = node_list_create(extend_node);
    if (!new_extend) {
        return;
    }
    
    if (!node->extend_link_node_list) {
        node->extend_link_node_list = new_extend;
    } else {
        node_list_add(&node->extend_link_node_list, extend_node);
    }
}

Node* node_get_extend_link_node(const Node* node, int index) {
    if (!node || index < 0) {
        return NULL;
    }
    
    return node_list_get(node->extend_link_node_list, index);
}

int node_get_extend_link_node_count(const Node* node) {
    if (!node) {
        return 0;
    }
    
    return node_list_get_count(node->extend_link_node_list);
}

// Node链表操作
NodeList* node_list_create(Node* node) {
    NodeList* list = (NodeList*)malloc(sizeof(NodeList));
    if (!list) {
        return NULL;
    }
    
    list->node = node;
    list->next = NULL;
    return list;
}

void node_list_destroy(NodeList* list) {
    if (!list) {
        return;
    }
    
    NodeList* current = list;
    while (current) {
        NodeList* next = current->next;
        free(current);
        current = next;
    }
}

void node_list_add(NodeList** list, Node* node) {
    if (!list || !node) {
        return;
    }
    
    NodeList* new_item = node_list_create(node);
    if (!new_item) {
        return;
    }
    
    if (!*list) {
        *list = new_item;
    } else {
        NodeList* current = *list;
        while (current->next) {
            current = current->next;
        }
        current->next = new_item;
    }
}

Node* node_list_get(const NodeList* list, int index) {
    if (!list || index < 0) {
        return NULL;
    }
    
    NodeList* current = (NodeList*)list;
    for (int i = 0; i < index && current; i++) {
        current = current->next;
    }
    
    return current ? current->node : NULL;
}

int node_list_get_count(const NodeList* list) {
    if (!list) {
        return 0;
    }
    
    int count = 0;
    NodeList* current = (NodeList*)list;
    while (current) {
        count++;
        current = current->next;
    }
    
    return count;
} 