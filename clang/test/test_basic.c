#include "../include/simple_language.h"
#include <stdio.h>
#include <assert.h>

// 测试Token创建和销毁
void test_token_creation() {
    printf("测试Token创建和销毁...\n");
    
    // 测试字符串token
    Token* str_token = token_create("test.sl", TOKEN_STRING, "Hello World", 1, 0, NULL);
    assert(str_token != NULL);
    assert(str_token->type == TOKEN_STRING);
    assert(strcmp(token_to_string(str_token), "Hello World") == 0);
    token_destroy(str_token);
    
    // 测试数字token
    int64_t num_value = 42;
    Token* num_token = token_create("test.sl", TOKEN_NUMBER, &num_value, 2, 0, NULL);
    assert(num_token != NULL);
    assert(num_token->type == TOKEN_NUMBER);
    assert(num_token->lexeme.int_value == 42);
    token_destroy(num_token);
    
    // 测试布尔token
    bool bool_value = true;
    Token* bool_token = token_create("test.sl", TOKEN_BOOLEAN, &bool_value, 3, 0, NULL);
    assert(bool_token != NULL);
    assert(bool_token->type == TOKEN_BOOLEAN);
    assert(bool_token->lexeme.bool_value == true);
    token_destroy(bool_token);
    
    printf("Token测试通过\n");
}

// 测试Node创建和销毁
void test_node_creation() {
    printf("测试Node创建和销毁...\n");
    
    Node* node = node_create();
    assert(node != NULL);
    assert(node->priority == -1);
    assert(node->is_del == false);
    assert(node->node_type == NODE_NONE);
    
    // 测试子节点添加
    Node* child1 = node_create();
    Node* child2 = node_create();
    assert(child1 != NULL);
    assert(child2 != NULL);
    
    node_add_child(node, child1);
    node_add_child(node, child2);
    
    assert(node_get_child_count(node) == 2);
    assert(node_get_child(node, 0) == child1);
    assert(node_get_child(node, 1) == child2);
    
    // 销毁节点（会自动销毁子节点）
    node_destroy(node);
    
    printf("Node测试通过\n");
}

// 测试MetaBase创建和销毁
void test_meta_base_creation() {
    printf("测试MetaBase创建和销毁...\n");
    
    MetaBase* mb = meta_base_create();
    assert(mb != NULL);
    assert(mb->permission == PERMISSION_PUBLIC);
    assert(mb->ref_from_type == REF_FROM_LOCAL);
    assert(mb->deep == 0);
    assert(mb->anchor_deep == 0);
    
    // 测试属性设置
    meta_base_set_name(mb, "TestClass");
    meta_base_set_deep(mb, 5);
    meta_base_set_permission(mb, PERMISSION_PRIVATE);
    
    assert(strcmp(meta_base_get_name(mb), "TestClass") == 0);
    assert(meta_base_get_deep(mb) == 5);
    assert(meta_base_get_permission(mb) == PERMISSION_PRIVATE);
    
    meta_base_destroy(mb);
    
    printf("MetaBase测试通过\n");
}

// 测试VM对象创建和销毁
void test_vm_object_creation() {
    printf("测试VM对象创建和销毁...\n");
    
    // 测试SObject
    SObject* obj = s_object_create();
    assert(obj != NULL);
    assert(obj->is_null == false);
    assert(obj->is_void == false);
    assert(obj->ref_count == 1);
    s_object_destroy(obj);
    
    // 测试BoolObject
    BoolObject* bool_obj = bool_object_create(true);
    assert(bool_obj != NULL);
    assert(bool_obj->value == true);
    assert(bool_obj->base.e_type == TYPE_BOOLEAN);
    bool_object_destroy(bool_obj);
    
    // 测试Int32Object
    Int32Object* int_obj = int32_object_create(123);
    assert(int_obj != NULL);
    assert(int_obj->value == 123);
    assert(int_obj->base.e_type == TYPE_INT32);
    int32_object_destroy(int_obj);
    
    // 测试StringObject
    StringObject* str_obj = string_object_create("Test String");
    assert(str_obj != NULL);
    assert(strcmp(str_obj->value, "Test String") == 0);
    assert(str_obj->base.e_type == TYPE_STRING);
    string_object_destroy(str_obj);
    
    // 测试ArrayObject
    ArrayObject* arr_obj = array_object_create(10);
    assert(arr_obj != NULL);
    assert(arr_obj->capacity == 10);
    assert(arr_obj->length == 0);
    array_object_destroy(arr_obj);
    
    printf("VM对象测试通过\n");
}

// 测试数组操作
void test_array_operations() {
    printf("测试数组操作...\n");
    
    ArrayObject* arr = array_object_create(5);
    assert(arr != NULL);
    
    // 添加元素
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    
    array_object_add_element(arr, &value1);
    array_object_add_element(arr, &value2);
    array_object_add_element(arr, &value3);
    
    assert(array_object_get_length(arr) == 3);
    assert(*(int*)array_object_get_element(arr, 0) == 10);
    assert(*(int*)array_object_get_element(arr, 1) == 20);
    assert(*(int*)array_object_get_element(arr, 2) == 30);
    
    // 测试自动扩容
    int value4 = 40;
    int value5 = 50;
    int value6 = 60;
    
    array_object_add_element(arr, &value4);
    array_object_add_element(arr, &value5);
    array_object_add_element(arr, &value6);
    
    assert(array_object_get_length(arr) == 6);
    assert(array_object_get_capacity(arr) >= 6);
    
    array_object_destroy(arr);
    
    printf("数组操作测试通过\n");
}

// 测试字符串操作
void test_string_operations() {
    printf("测试字符串操作...\n");
    
    StringObject* str = string_object_create("Hello");
    assert(str != NULL);
    assert(strcmp(string_object_get_value(str), "Hello") == 0);
    assert(string_object_get_length(str) == 5);
    
    // 测试字符串修改
    string_object_set_value(str, "World");
    assert(strcmp(string_object_get_value(str), "World") == 0);
    assert(string_object_get_length(str) == 5);
    
    // 测试空字符串
    string_object_set_value(str, NULL);
    assert(strcmp(string_object_get_value(str), "") == 0);
    assert(string_object_get_length(str) == 0);
    
    string_object_destroy(str);
    
    printf("字符串操作测试通过\n");
}

// 主测试函数
int main() {
    printf("开始运行Simple Language C实现测试...\n\n");
    
    test_token_creation();
    test_node_creation();
    test_meta_base_creation();
    test_vm_object_creation();
    test_array_operations();
    test_string_operations();
    
    printf("\n所有测试通过！\n");
    return 0;
} 