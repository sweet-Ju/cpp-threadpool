cmake_minimum_required(VERSION 3.10)
project(ThreadPoolProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 thread pool 库
add_library(ThreadPool STATIC
    src/ThreadPool.cpp
    src/Logger.cpp
)

target_include_directories(ThreadPool
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# 添加可执行文件
add_executable(main
    example/main.cpp
)

# 链接 thread pool 库
target_link_libraries(main
    PRIVATE ThreadPool
)
