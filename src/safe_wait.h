#ifndef _NAIVE_SAFE_WAIT_H_
#define _NAIVE_SAFE_WAIT_H_

#include "base_constructor.h"

#include <mutex>
#include <condition_variable>

namespace naive {

class SafeConditionVariable {

public:

	explicit SafeConditionVariable():_pv(0){}

	void Notify() {
		std::unique_lock<std::mutex> lck(_mtx);
		_pv++;
		_cv.notify_one();
	}

	void Wait() {
		std::unique_lock<std::mutex> lck(_mtx);
		_cv.wait(lck,[this]{ return (_pv > 0); });
		_pv--;
	}
	
	~SafeConditionVariable() {}

private:

	std::mutex				_mtx;
	int						_pv;
	std::condition_variable _cv;

	DISALLOW_COPY_AND_ASSIGN(SafeConditionVariable);
};
}

#endif // !_NAIVE_SAFE_WAIT_H_
