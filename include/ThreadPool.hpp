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
const auto IDLE_TIMEOUT = std::chrono::seconds(30);
class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex_;
    std::condition_variable cv;
    bool stop_;
    std::atomic<size_t> currentThread;
    std::atomic<size_t> idleThread;
    size_t MAX_THREADS;
    size_t MIN_THREADS;
public:
    ThreadPool(size_t minThread, size_t masThread);
    ~ThreadPool();
    void addThread();
template <class F,class ...Args>
    auto submit(F&&f,Args&&...args)->std::future<decltype(f(args...))>
    {
        if (stop_)
        {
            throw std::runtime_error("submit on stopped ThreadPool");
        }
        using return_type = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>
        (std::bind(std::forward<F>(f),std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks.emplace([task](){(*task)();});
            if(tasks.size()>idleThread&&currentThread<MAX_THREADS)
            {
                addThread();
            }
        }
        cv.notify_one();
        return res;
    }
};
#endif //THREADPOOL_H