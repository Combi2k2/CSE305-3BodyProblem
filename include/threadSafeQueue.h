#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    bool isFinished_;

public:
    ThreadSafeQueue() : isFinished_(false) {}

    void push(const T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        condition_.notify_one();
    }

    bool pop(T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty() && !isFinished_) {
            condition_.wait(lock);
        }
        if(queue_.empty()){
            return false;
        }
        value = queue_.front();
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    void finish() {
        std::lock_guard<std::mutex> lock(mutex_);
        isFinished_ = true;
        condition_.notify_all();
    }

    bool isFinished() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return isFinished_;
    }
};

#endif