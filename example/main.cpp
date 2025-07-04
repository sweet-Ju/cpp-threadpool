#include "ThreadPool.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    Logger::open("/home/chen/ThreadPool/log.txt");
    std::cout<<"start\n";
    ThreadPool pool(2, 8);
    auto result = pool.submit([](int a, int b){ return a + b; }, 3, 4);
    std::cout << result.get()<<"\n";  // 7
    Logger::close();   
    return 0;
}
