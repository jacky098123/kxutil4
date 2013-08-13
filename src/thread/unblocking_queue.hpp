#ifndef __UNBLOCKING_QUEUE_H__
#define __UNBLOCKING_QUEUE_H__
/*
 * Jacky at 2010-10-06
 */

#include "condition.h"
#include "mutex.h"

#include <deque>
#include <assert.h>

namespace kxutil3 {
#define  UNBLOCKING_QUEUE_CAPACITY_DEFAULT 1024*1024
    template<typename T>
        class UnblockingQueue {
            public:
                UnblockingQueue(int capacity) : mutex_(), capacity_(capacity) {
                }

                int Push(const T& x) {
                    MutexLockGuard lock(mutex_);
                    if (queue_.size() >= (size_t)capacity_)
                        return -1;
                    queue_.push_back(x);
                    printf("after push, size():%zu\n", queue_.size());
                    return 0;
                }

                T Pop() {
                    MutexLockGuard lock(mutex_);
                    printf("before pop size: %zu\n", queue_.size());
                    if (queue_.size() <= 0) {
                        return NULL;
                    }
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
                std::deque<T>     queue_;
                int               capacity_;
        };
} // namespace kxutil3
#endif // __UNBLOCKING_QUEUE_H__
