#ifndef _NAIVE_THREAD_PROCESSOR_RELIABLE_H_
#define _NAIVE_THREAD_PROCESSOR_RELIABLE_H_

#include "thread_processor.h"

#include "../base_constructor.h"
#include "../safe_wait.h"

#include <mutex>
#include <thread>
#include <atomic>

namespace naive {

class ThreadProcessorReliable : public ThreadProcessor{

public:
		
	explicit ThreadProcessorReliable();
		
	void Run(std::function<bool()> func) override;

	void Notify() override;

	void Stop() override;

	~ThreadProcessorReliable();

private:

	std::thread						*_thread;
	std::function<bool()>			_func;

	bool							_shouldStop;
	bool							_threadOver;
	bool							_inited;
	std::atomic<bool>				_isRuning;
	std::mutex						_mtx;
	SafeConditionVariable			_scv;
	SafeConditionVariable			_endScv;

	DISALLOW_COPY_AND_ASSIGN(ThreadProcessorReliable);

};

}


#endif // !_NAIVE_THREAD_PROCESSOR_RELIABLE_H_
