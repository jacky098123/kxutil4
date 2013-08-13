#ifndef __THREAD_H__
#define __THREAD_H__
class Thread {
typedef void *func(void *arg) THREAD_FUNC;
public:
    Thread(THREAD_FUNC func) {
        func_ = func;
    }

    static ThreadCallBack(void *arg) {
        Thread *thread = static_cast<Thread*>(arg);
        if (thread == NULL) {
            return NULL;
        }
        thread->func_;
    }

    int Start() {
        int ret;
        ret = pthread_create(&thread_id_, NULL, ThreadCallBack, this);
        started_ = true;
    }

private:
    THREAD_FUNC func_;
    bool        started_;
    pthread_t   thread_id_;
};
#endif // __THREAD_H__
