#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <cstddef> // for size_t

namespace pr {

template <typename T>
class BoundedBlockingQueue {
public:
    explicit BoundedBlockingQueue(size_t max_size) : max_size_(max_size) {}

    void push(const T& value) {
        { // critical section
            std::unique_lock lock(mtx_);
            cv_.wait(lock, [this] { return queue_.size() < max_size_; });
            queue_.push_back(value);
        }
        cv_.notify_all(); // notify after releasing lock
    }

    T pop() {
        T value;
        { // critical section
            std::unique_lock lock(mtx_);
            cv_.wait(lock, [this] { return !queue_.empty(); });
            value = queue_.front();
            queue_.pop_front();
        }
        cv_.notify_all(); // notify after releasing lock
        return value;
    }

private:
    std::deque<T> queue_;
    size_t max_size_;
    std::mutex mtx_;
    std::condition_variable cv_;
};

} // namespace pr