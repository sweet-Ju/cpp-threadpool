#include "ThreadPool.hpp"
ThreadPool::ThreadPool(size_t numThread):stop_(false)
{
   for(size_t i =0;i<numThread;++i)
   {
      threads.emplace_back([this](){
        while (true)
        {
            std::function<void()>task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv.wait(lock,[this](){return stop_||!tasks.empty();});
                if(stop_&&tasks.empty())break;
                task = std::move(tasks.front());
                tasks.pop();
                
            }
            task();
        }   
      });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cv.notify_all();
    for(auto& t:threads)
    {
        if(t.joinable())
        t.join();
    }
}
