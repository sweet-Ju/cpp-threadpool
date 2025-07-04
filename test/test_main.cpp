#include "ThreadPool.h"
#include <iostream>

int main() {
    ThreadPool pool(2, 6);

    std::vector<std::future<int>> results;
    for (int i = 0; i < 10; ++i) {
        results.emplace_back(pool.submit([i](){
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return i * i;
        }));
    }

    for (auto& f : results) {
        std::cout << "Result: " << f.get() << std::endl;
    }
    return 0;
}