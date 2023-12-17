#include <ThreadPool.hpp>

ThreadPool::ThreadPool(unsigned int n)
        : busy(), stop() {
    for (unsigned int i = 0; i < n; ++i)
        workers.emplace_back([this] { threadFunc(); });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> latch(queueMutex);
        stop = true;
        taskCV.notify_all();
    }

    for (auto &t: workers)
        t.join();
}

ThreadPool &ThreadPool::getInstance(unsigned int n) {
    static ThreadPool instance{n};
    return instance;
}

unsigned int ThreadPool::getThreadsCount() {
    return workers.size();
}

void ThreadPool::threadFunc() {
    while (true) {
        std::unique_lock<std::mutex> latch(queueMutex);
        taskCV.wait(
                latch,
                [this]() { return stop || !tasks.empty(); });

        if (tasks.empty())
            break;

        ++busy;

        auto fn = tasks.front();
        tasks.pop();

        latch.unlock();

        fn();

        latch.lock();
        --busy;

        finishedCV.notify_one();
    }
}

void ThreadPool::waitAll() {
    std::unique_lock<std::mutex> lock(queueMutex);
    finishedCV.wait(
            lock,
            [this]() { return tasks.empty() && (busy == 0); });
}