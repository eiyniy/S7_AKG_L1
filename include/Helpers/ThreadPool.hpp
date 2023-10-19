#include <iostream>
#include <queue>
#include <functional>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <random>

class ThreadPool
{
public:
    ThreadPool(unsigned int n);
    ~ThreadPool();

    static ThreadPool &getInstance(unsigned int n = std::thread::hardware_concurrency());

    unsigned int getThreadsCount();
    template <class F>
    void enqueue(F &&f);
    void waitAll();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable taskCV;
    std::condition_variable finishedCV;

    unsigned int busy;
    bool stop;

    void threadFunc();
};

template <class F>
void ThreadPool::enqueue(F &&f)
{
    std::unique_lock<std::mutex> lock(queueMutex);
    tasks.push(std::forward<F>(f));
    taskCV.notify_one();
}
