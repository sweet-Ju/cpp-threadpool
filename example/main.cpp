#include "ThreadPool.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    ThreadPool pool(2, 6); // 最小2线程，最大6线程

    std::cout << "提交10个任务..." << std::endl;

    std::vector<std::future<int>> results;

    for (int i = 0; i < 10; ++i) {
        results.emplace_back(
            pool.submit([i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::cout << "任务 " << i << " 运行在线程 " << std::this_thread::get_id() << "\n";
                return i * i;
            })
        );
    }

    std::cout << "等待任务完成..." << std::endl;

    for (auto& f : results) {
        std::cout << "任务返回值: " << f.get() << std::endl;
    }

    std::cout << "所有任务已完成，等待线程是否收缩...\n";
    std::this_thread::sleep_for(std::chrono::seconds(70)); // 看是否收缩
    std::cout << "测试结束\n";
    return 0;
}
