#ifndef SIMPLE_LANGUAGE_VM_H
#define SIMPLE_LANGUAGE_VM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vm_object.h"
#include "node.h"
#include <stddef.h>

// 前向声明
typedef struct IRMetaClass IRMetaClass;
typedef struct IRMetaVariable IRMetaVariable;
typedef struct IRMetaFunction IRMetaFunction;

// 虚拟机栈帧结构
typedef struct StackFrame {
    struct StackFrame* previous;           // 上一个栈帧
    IRMetaFunction* function;              // 当前函数
    void** local_variables;               // 局部变量
    void** parameters;                    // 参数
    int local_count;                      // 局部变量数量
    int param_count;                      // 参数数量
    size_t return_address;                // 返回地址
    void* return_value;                   // 返回值
} StackFrame;

// 虚拟机结构
typedef struct VM {
    StackFrame* current_frame;             // 当前栈帧
    StackFrame* call_stack;                // 调用栈
    int stack_depth;                       // 栈深度
    int max_stack_depth;                   // 最大栈深度
    
    void** global_variables;               // 全局变量
    int global_count;                      // 全局变量数量
    
    IRMetaClass** class_table;             // 类表
    int class_count;                       // 类数量
    
    IRMetaFunction** function_table;       // 函数表
    int function_count;                     // 函数数量
    
    void* heap_memory;                     // 堆内存
    size_t heap_size;                      // 堆大小
    size_t heap_used;                      // 已使用堆大小
    
    bool is_running;                       // 是否正在运行
    int error_code;                        // 错误代码
    char* error_message;                   // 错误消息
} VM;

// 虚拟机创建和销毁函数
VM* vm_create(void);
void vm_destroy(VM* vm);

// 虚拟机初始化函数
bool vm_initialize(VM* vm);
void vm_shutdown(VM* vm);

// 虚拟机执行函数
bool vm_execute(VM* vm, Node* ast_root);
bool vm_execute_statement(VM* vm, Node* statement);
void* vm_execute_expression(VM* vm, Node* expression);

// 虚拟机栈操作函数
bool vm_push_frame(VM* vm, IRMetaFunction* function);
bool vm_pop_frame(VM* vm);
StackFrame* vm_get_current_frame(const VM* vm);
int vm_get_stack_depth(const VM* vm);

// 虚拟机变量操作函数
bool vm_set_global_variable(VM* vm, const char* name, void* value);
void* vm_get_global_variable(const VM* vm, const char* name);
bool vm_set_local_variable(VM* vm, int index, void* value);
void* vm_get_local_variable(const VM* vm, int index);
bool vm_set_parameter(VM* vm, int index, void* value);
void* vm_get_parameter(const VM* vm, int index);

// 虚拟机函数调用函数
void* vm_call_function(VM* vm, IRMetaFunction* function, void** args, int arg_count);
void* vm_call_method(VM* vm, void* object, const char* method_name, void** args, int arg_count);

// 虚拟机类操作函数
bool vm_register_class(VM* vm, IRMetaClass* class_info);
IRMetaClass* vm_get_class(const VM* vm, const char* name);
void* vm_create_object(VM* vm, IRMetaClass* class_info);
bool vm_set_object_field(VM* vm, void* object, const char* field_name, void* value);
void* vm_get_object_field(const VM* vm, void* object, const char* field_name);

// 虚拟机内存管理函数
void* vm_allocate(VM* vm, size_t size);
void vm_free(VM* vm, void* ptr);
void vm_garbage_collect(VM* vm);
size_t vm_get_heap_size(const VM* vm);
size_t vm_get_heap_used(const VM* vm);

// 虚拟机错误处理函数
void vm_set_error(VM* vm, int error_code, const char* error_message);
int vm_get_error_code(const VM* vm);
const char* vm_get_error_message(const VM* vm);
bool vm_has_error(const VM* vm);
void vm_clear_error(VM* vm);

// 虚拟机状态查询函数
bool vm_is_running(const VM* vm);
void vm_stop(VM* vm);
void vm_pause(VM* vm);
void vm_resume(VM* vm);

// 虚拟机调试函数
void vm_print_stack_trace(const VM* vm);
void vm_print_memory_usage(const VM* vm);
void vm_print_variable_table(const VM* vm);
void vm_print_function_table(const VM* vm);
void vm_print_class_table(const VM* vm);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_VM_H 