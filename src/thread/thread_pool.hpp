#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "thread_job.h"
#include "blocking_queue.h"

#include "log/utillog.h"

namespace kxutil3 {
    class ThreadPool;
    typedef struct {
        pthread_t     thread_id;
        ThreadPool   *parent;
        bool          is_running;
    }ThreadStruct;

    class ThreadPool {
        public:
            ThreadPool(int number) : thread_number_(number), 
                thread_struct_(NULL),
                started_(false),
                stop_running_(false),
                job_pool_(BLOCKING_QUEUE_CAPACITY_DEFAULT){
            }

            ~ThreadPool() {
                if (started_) {
                    Stop();
                }
                if (thread_struct_ != NULL)
                    delete []thread_struct_;
            }

            BlockingQueue<ThreadJob*> &GetBlockingQueue() {
                return job_pool_;
            }

            int Start();
            void Stop();

            static void *ThreadCallBack(void *arg); //pthread_create call back
        private:
            int              thread_number_;
            ThreadStruct    *thread_struct_;
            bool             started_;
            bool             stop_running_;
        private:
            BlockingQueue<ThreadJob*> job_pool_;
    };

    int ThreadPool::Start() {
        assert(!started_);
        started_ = true;

        thread_struct_ = new ThreadStruct[thread_number_];
        if (thread_struct_ == NULL)
            return -1;

        for (int i=0; i<thread_number_; i++) {
            thread_struct_[i].is_running = false;
            thread_struct_[i].parent     = this;
            int ret = pthread_create(&(thread_struct_[i].thread_id), NULL, 
                    ThreadCallBack, (void*)&thread_struct_[i]);
            if (ret != 0) {
                printf("pthread_create err: %d\n", ret);
                return -2;                          
            }
        }
        return 0;
    }

    void ThreadPool::Stop() {
        assert(started_);
        started_ = false;

        stop_running_ = true;
        for (int i=0; i<thread_number_; i++) {
            pthread_join(thread_struct_[i].thread_id, NULL);
        }

    }

    void *ThreadPool::ThreadCallBack(void *arg) {
        assert(arg != NULL);
        ThreadStruct *thread_struct = (ThreadStruct*)arg;
        ThreadPool   *thread_pool   = (ThreadPool*)thread_struct->parent;

        thread_struct->is_running = true;
        while (!thread_pool->stop_running_) {
            ThreadJob* job = thread_pool->job_pool_.TimedPop(3000); //3 seconds
            if (job != NULL) {
                job->Execute();
            }
            else {
                printf("no job\n");
            }
        }
        thread_struct->is_running = false;
        return NULL;
    }

} // namespace kxutil3

#endif // __THREAD_POOL_H__
