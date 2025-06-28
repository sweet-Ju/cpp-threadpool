#include "ThreadPool.hpp"
ThreadPool::ThreadPool(size_t minThread,size_t maxThread)
:stop_(false),MIN_THREADS(minThread),MAX_THREADS(maxThread),currentThread(0),idleThread(0)
{
   for(size_t i =0;i<MIN_THREADS;++i)
   {
      currentThread++;
      threads.emplace_back([this](){
        while (true)
         {
            std::function<void()>task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                idleThread++;
                auto timeout = !cv.wait_for(lock,IDLE_TIMEOUT,[this](){return stop_||!tasks.empty();});
                idleThread--;
                if(stop_&&tasks.empty())break;
                if(timeout&&currentThread>MIN_THREADS)
                {
                    currentThread--;
                    break;
                }
                if(tasks.empty())continue;
                task = std::move(tasks.front());
                tasks.pop(); 
            }
            try
            {
                if(task)task();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
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

void ThreadPool::addThread()
{
    currentThread++;
    threads.emplace_back([this](){
        std::cout << "[Thread Started] " << std::this_thread::get_id() << "\n";
        while (true)
        {
            std::function<void()>task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                idleThread++;
                auto timeout = !cv.wait_for(lock,IDLE_TIMEOUT,[this](){return stop_||!tasks.empty();});
                idleThread--;
                if(stop_&&tasks.empty())break;
                if(timeout&&currentThread>MIN_THREADS)
                {
                    currentThread--;
                    break;
                }
                if(tasks.empty())continue;
                task = std::move(tasks.front());
                tasks.pop(); 
            }
            try
            {
                std::cout << "[Executing Task] Thread: " << std::this_thread::get_id() << "\n";
                if(task)task();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }   
    });
    
}
