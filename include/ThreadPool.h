#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <future>
#include <condition_variable>
#include <functional>
#include <memory>
#include <chrono>
const int MAX_THREADS = 16;
const int CORE_THREADS = 2;
const auto IDLE_TIMEOUT = std::chrono::seconds(30);
class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex task_mutex;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(size_t numThread);
    ~ThreadPool();
template <class F,class ...Args>
    auto summit(F&&f,Args&&...args)->std::future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>
        (std::bind(std::forward<F>(f),std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::lock_guard<std::mutex> lock(task_mutex);
            tasks.emplace([task](){(*task);});
        }
        cv.notify_one();
        return res;
    }
};
#endif //THREADPOOL_H