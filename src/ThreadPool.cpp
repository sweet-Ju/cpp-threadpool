#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThread):stop(false)
{
   for(size_t i =0;i<numThread;++i)
   {
      threads.emplace_back([this](){
        while (true)
        {
            std::function<void()>task;
            {
                std::unique_lock<std::mutex> lock(task_mutex);
                cv.wait(lock,[this](){return stop||!tasks.empty();});
                if(stop&&tasks.empty())break;
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
        std::lock_guard<std::mutex> lock(task_mutex);
        stop = true;
    }
    cv.notify_all();
    for(auto& t:threads)
    {
        t.join();
    }
}
