#include "ThreadPool.h"
ThreadPool::ThreadPool(size_t minThread,size_t maxThread)
:stop_(false),MIN_THREADS(minThread),MAX_THREADS(maxThread),currentThread(0),idleThread(0)
{
   for(size_t i =0;i<MIN_THREADS;++i)
   {
      currentThread++;
      threads.emplace_back([this](){
        LOG_DEBUG("Initialize a thread. ID: "+std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
        while (true)
         {
            std::function<void()>task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                idleThread++;
                auto timeout = !cv.wait_for(lock,IDLE_TIMEOUT,[this](){return stop_||!tasks.empty();});
                idleThread--;
                if(stop_&&tasks.empty())
                {
                    LOG_INFO("Thread exiting: stop signal received. ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
                    currentThread--;
                    break;
                }
                if(timeout&&currentThread>MIN_THREADS)
                {
                    LOG_INFO("Thread timed out and exited. ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
                    currentThread--;
                    break;
                }
                if(tasks.empty())continue;
                task = std::move(tasks.front());
                tasks.pop();
                LOG_DEBUG("Task dequeued by thread ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
            }
            try
            {
                if(task)task();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("Exception in task: " + std::string(e.what()));
            }
        } 
      });
    }
}

ThreadPool::~ThreadPool()
{
    LOG_INFO("ThreadPool is being destructed. This thread: ID = "+std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
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
        LOG_INFO("New thread started. ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
        while (true)
        {
            std::function<void()>task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                idleThread++;
                auto timeout = !cv.wait_for(lock,IDLE_TIMEOUT,[this](){return stop_||!tasks.empty();});
                idleThread--;
                if(stop_&&tasks.empty())
                {
                    LOG_INFO("Thread exiting: stop signal received. ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
                    currentThread--;
                    break;
                }
                if(timeout&&currentThread>MIN_THREADS)
                {
                    LOG_INFO("Thread timed out and exited. ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
                    currentThread--;
                    break;
                }
                if(tasks.empty())continue;
                task = std::move(tasks.front());
                tasks.pop();
                LOG_DEBUG("Task dequeued by thread ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()))); 
            }
            try
            {
                if(task)task();
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("Exception in task: " + std::string(e.what()));
            }
        }   
    });
    
}
