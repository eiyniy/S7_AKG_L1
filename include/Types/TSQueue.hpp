#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <optional>

template <typename T>
class TSQueue
{
private:
    std::queue<T> queue;

    std::mutex mutex;
    std::condition_variable cv;

public:
    void push(T item);
    T pop();
    const std::optional<T> tryPop();
};

template <typename T>
inline void TSQueue<T>::push(T item)
{
    std::unique_lock<std::mutex> lock(mutex);

    queue.push(std::move(item));

    cv.notify_one();
}

template <typename T>
inline T TSQueue<T>::pop()
{
    std::unique_lock<std::mutex> lock(mutex);

    cv.wait(lock,
            [this]()
            { return !queue.empty(); });

    T item = queue.front();
    queue.pop();

    return item;
}

template <typename T>
inline const std::optional<T> TSQueue<T>::tryPop()
{
    std::unique_lock<std::mutex> lock(mutex);

    if (queue.empty())
        return std::nullopt;

    T item = std::move(queue.front());
    queue.pop();

    return item;
}
