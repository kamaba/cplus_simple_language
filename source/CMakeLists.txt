cmake_minimum_required(VERSION 3.10)
project(SimpleLanguage VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++11 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 收集所有源文件
file(GLOB_RECURSE SOURCES
    "*.cpp"
)

file(GLOB_RECURSE HEADERS
    "*.h"
)

# 创建静态库
add_library(SimpleLanguage STATIC ${SOURCES} ${HEADERS})

# 包含目录
target_include_directories(SimpleLanguage PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 编译选项
if(MSVC)
    target_compile_options(SimpleLanguage PRIVATE /W4)
else()
    target_compile_options(SimpleLanguage PRIVATE -Wall -Wextra -pedantic)
endif()

# 创建可执行文件用于测试
add_executable(SimpleLanguageTest test_compile.cpp)
target_link_libraries(SimpleLanguageTest SimpleLanguage)

# 输出信息
message(STATUS "SimpleLanguage library configured")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

