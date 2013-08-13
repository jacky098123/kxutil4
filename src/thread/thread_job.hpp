#ifndef __THREAD_JOB_H__
#define __THREAD_JOB_H__

#include "mutex.h"
#include <deque>

namespace kxutil3 {

    class ThreadJob {
	public:
	    ThreadJob() {
	    }
	    virtual ~ThreadJob() {
	    }
	    virtual int Execute() {
		return 0;
	    }
    };
} // namespace kxutil3
#endif // __THREAD_JOB_H__
