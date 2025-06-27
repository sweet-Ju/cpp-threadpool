cmake_minimum_required(VERSION 3.10)
project(ThreadPool)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED No)
# Include header files
#include_directories(include)
# 添加源文件
add_library(threadPool STATIC
    src/ThreadPool.cpp
)
#添加头文件
target_include_directories(threadPool PUBLIC include)
# 示例程序
add_executable(thread_pool_example example/main.cpp)
target_link_libraries(thread_pool_example threadPool)