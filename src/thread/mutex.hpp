#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>

namespace kxutil3 {

    class MutexLock {
	public:
	    MutexLock() {
		pthread_mutex_init(&mutex_, NULL);
	    }

	    ~MutexLock() {
		pthread_mutex_destroy(&mutex_);
	    }

	    void Lock() {
		pthread_mutex_lock(&mutex_);
	    }

	    void Unlock() {
		pthread_mutex_unlock(&mutex_);
	    }

	    pthread_mutex_t* get_mutex() /* non-const */ {
		return &mutex_;
	    }

	private:
	    pthread_mutex_t mutex_;
    };

    class MutexLockGuard {
	public:
	    explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex) {
		mutex_.Lock();
	    }

	    ~MutexLockGuard() {
		mutex_.Unlock();
	    }

	private:
	    MutexLock& mutex_;
    };

} // namespace kxutil3

/* Prevent misuse like:
   MutexLockGuard(mutex_);
   A tempory object doesn't hold the lock for long! 
 */
#define MutexLockGuard(x) error "Missing guard object name"

#endif // __MUTEX_H__
