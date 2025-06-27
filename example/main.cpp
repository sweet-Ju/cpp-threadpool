#include "ThreadPool.hpp"
#include <iostream>
#include <chrono>
int add(int a, int b) 
{
    return a + b;
}
int main()
{
    ThreadPool pool(2);

    auto future = pool.submit(add, 10, 20);
    std::cout << "Result: " << future.get() << std::endl;

    return 0;

}