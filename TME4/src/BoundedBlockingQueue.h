#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <cstddef> // for size_t

namespace pr {

template <typename T>
class BoundedBlockingQueue {
public:
    explicit BoundedBlockingQueue(size_t capacity) : capacity_(capacity) {}

    void push(const T& value) {
        { // critical section
            std::unique_lock lock(m);
            cond.wait(lock, [this] { return queue_.size() < capacity_; });
            queue_.push_back(value);
        }
        cond.notify_all(); // notify after releasing lock
    }

    T pop() {
        std::unique_lock lock(m);
        cond.wait(lock, [this] { return !queue_.empty(); });
        T value = queue_.front();
        queue_.pop_front();
        // explicit unlock : the critical section ends here, value stays alive for the return.
        // unique_lock::unlock is handy whenever the end of the critical section is not the end of
        // a block, typically to notify outside the critical section without nesting a block.
        lock.unlock();
        cond.notify_all(); // notify after releasing lock
        return value;
    }

private:
    std::deque<T> queue_;
    size_t capacity_;
    std::mutex m;
    std::condition_variable cond;
};

} // namespace pr