#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "mutex.h"
#include <pthread.h>
#include <time.h>
#include <errno.h>

namespace kxutil3 {
    class Condition {
        public:
            Condition(MutexLock& mutex) : mutex_(mutex) {
                pthread_cond_init(&cond_, NULL);
            }

            ~Condition() {
                pthread_cond_destroy(&cond_);
            }

            void Wait() {
                pthread_cond_wait(&cond_, mutex_.get_mutex());
            }

            void TimedWait(int ms) {
                struct timespec wait_time;
                wait_time.tv_sec = time(NULL) + ms / 1000;
                wait_time.tv_nsec = (ms % 1000) * 1000000;
                int ret = pthread_cond_timedwait(&cond_, mutex_.get_mutex(), 
                        &wait_time);
                printf("timedwait %d\n", ret);
            }

            void Notify() {
                pthread_cond_signal(&cond_);
            }

            void NotifyAll() {
                pthread_cond_broadcast(&cond_);
            }

        private:
            MutexLock& mutex_;
            pthread_cond_t cond_;
    };

} // namespace kxutil3
#endif  // __CONDITION_H__
