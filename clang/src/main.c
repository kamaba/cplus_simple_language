#include "../include/simple_language.h"
#include <stdio.h>
#include <stdlib.h>

// 简单的示例程序
void print_banner() {
    printf("========================================\n");
    printf("    Simple Language C Implementation\n");
    printf("========================================\n");
    printf("版本: %s\n", simple_language_get_version());
    printf("构建日期: %s\n", simple_language_get_build_date());
    printf("========================================\n\n");
}

// 演示Token操作
void demo_tokens() {
    printf("=== Token演示 ===\n");
    
    // 创建不同类型的token
    Token* str_token = token_create("demo.sl", TOKEN_STRING, "Hello World", 1, 0, NULL);
    Token* num_token = token_create("demo.sl", TOKEN_NUMBER, NULL, 2, 0, NULL);
    num_token->lexeme.int_value = 42;
    
    printf("字符串Token: %s\n", token_to_string(str_token));
    printf("数字Token: %s\n", token_to_string(num_token));
    printf("Token位置: %s\n", token_to_all_string(str_token));
    
    // 清理
    token_destroy(str_token);
    token_destroy(num_token);
    printf("\n");
}

// 演示Node操作
void demo_nodes() {
    printf("=== AST节点演示 ===\n");
    
    // 创建根节点
    Node* root = node_create();
    node_set_node_type(root, NODE_ROOT);
    node_set_priority(root, 0);
    
    // 创建子节点
    Node* child1 = node_create();
    Node* child2 = node_create();
    node_set_node_type(child1, NODE_SYMBOL);
    node_set_node_type(child2, NODE_SYMBOL);
    
    // 添加到根节点
    node_add_child(root, child1);
    node_add_child(root, child2);
    
    printf("根节点类型: %d\n", root->node_type);
    printf("子节点数量: %d\n", node_get_child_count(root));
    printf("第一个子节点类型: %d\n", node_get_child(root, 0)->node_type);
    
    // 清理
    node_destroy(root);
    printf("\n");
}

// 演示MetaBase操作
void demo_meta_base() {
    printf("=== 元数据基类演示 ===\n");
    
    MetaBase* mb = meta_base_create();
    meta_base_set_name(mb, "DemoClass");
    meta_base_set_deep(mb, 3);
    meta_base_set_permission(mb, PERMISSION_PUBLIC);
    
    printf("类名: %s\n", meta_base_get_name(mb));
    printf("深度: %d\n", meta_base_get_deep(mb));
    printf("权限: %d\n", meta_base_get_permission(mb));
    printf("实际深度: %d\n", meta_base_get_real_deep(mb));
    
    // 清理
    meta_base_destroy(mb);
    printf("\n");
}

// 演示VM对象操作
void demo_vm_objects() {
    printf("=== 虚拟机对象演示 ===\n");
    
    // 创建不同类型的对象
    BoolObject* bool_obj = bool_object_create(true);
    Int32Object* int_obj = int32_object_create(123);
    StringObject* str_obj = string_object_create("Test String");
    ArrayObject* arr_obj = array_object_create(5);
    
    printf("布尔对象: %s\n", bool_obj->value ? "true" : "false");
    printf("整数对象: %d\n", int_obj->value);
    printf("字符串对象: %s\n", string_object_get_value(str_obj));
    printf("数组对象容量: %zu\n", array_object_get_capacity(arr_obj));
    
    // 演示数组操作
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        array_object_add_element(arr_obj, &values[i]);
    }
    printf("数组长度: %zu\n", array_object_get_length(arr_obj));
    
    // 清理
    bool_object_destroy(bool_obj);
    int32_object_destroy(int_obj);
    string_object_destroy(str_obj);
    array_object_destroy(arr_obj);
    printf("\n");
}

// 演示错误处理
void demo_error_handling() {
    printf("=== 错误处理演示 ===\n");
    
    // 尝试创建无效对象
    Token* invalid_token = token_create(NULL, TOKEN_NONE, NULL, -1, -1, NULL);
    if (invalid_token) {
        printf("警告: 创建了无效的Token\n");
        token_destroy(invalid_token);
    } else {
        printf("正确: 拒绝了无效Token的创建\n");
    }
    
    // 测试空指针处理
    token_set_type(NULL, TOKEN_STRING);
    node_set_priority(NULL, 0);
    meta_base_set_name(NULL, "Test");
    
    printf("空指针操作被安全忽略\n");
    printf("\n");
}

// 主函数
int main(int argc, char* argv[]) {
    print_banner();
    
    // 初始化Simple Language
    if (!simple_language_initialize()) {
        printf("错误: Simple Language初始化失败\n");
        return 1;
    }
    
    printf("Simple Language初始化成功\n\n");
    
    // 运行各种演示
    demo_tokens();
    demo_nodes();
    demo_meta_base();
    demo_vm_objects();
    demo_error_handling();
    
    printf("=== 性能测试 ===\n");
    
    // 性能测试：创建大量对象
    printf("创建1000个Token...\n");
    Token** tokens = malloc(1000 * sizeof(Token*));
    for (int i = 0; i < 1000; i++) {
        char num_str[32];
        snprintf(num_str, sizeof(num_str), "token_%d", i);
        tokens[i] = token_create("perf.sl", TOKEN_IDENTIFIER, num_str, i, 0, NULL);
    }
    
    printf("销毁1000个Token...\n");
    for (int i = 0; i < 1000; i++) {
        token_destroy(tokens[i]);
    }
    free(tokens);
    
    printf("性能测试完成\n\n");
    
    // 清理资源
    simple_language_shutdown();
    printf("Simple Language已关闭\n");
    
    printf("\n演示程序运行完成！\n");
    return 0;
} 