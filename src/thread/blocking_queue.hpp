#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include "condition.h"
#include "mutex.h"

#include <deque>
#include <assert.h>

#define  BLOCKING_QUEUE_CAPACITY_DEFAULT 1024*1024

namespace kxutil3 {
    template<typename T>
        class BlockingQueue {
            public:
                BlockingQueue(int capacity) : mutex_(), cond_(mutex_), capacity_(capacity) {
                }

                int Push(const T& x) {
                    MutexLockGuard lock(mutex_);
                    if (queue_.size() >= (size_t)capacity_)
                        return -1;
                    queue_.push_back(x);
                    cond_.Notify();
                    printf("after push, size():%zu\n", queue_.size());
                    return 0;
                }

                T Pop() {
                    MutexLockGuard lock(mutex_);
                    printf("before pop size: %zu\n", queue_.size());
                    while (queue_.empty())
                    {
                        printf("pop wait\n");
                        cond_.Wait();
                    }
                    assert(!queue_.empty());
                    T front(queue_.front());
                    queue_.pop_front();
                    return front;
                }

                T TimedPop(int ms) {
                    MutexLockGuard lock(mutex_);
                    if (queue_.empty())
                        cond_.TimedWait(ms); 
                    if (queue_.empty())
                        return NULL;

                    T front(queue_.front());
                    queue_.pop_front();
                    return front;
                }

                void set_capacity(int capacity) {
                    capacity_ = capacity;
                }

                size_t Size() const {
                    MutexLockGuard lock(mutex_);
                    return queue_.size();
                }

                bool Empty() const {
                    MutexLockGuard lock(mutex_);
                    return queue_.size() == 0;
                }

            private:
                mutable MutexLock mutex_;
                Condition         cond_;
                std::deque<T>     queue_;
                int               capacity_;
        };
} // namespace kxutil3

#endif // __BLOCKING_QUEUE_H__
