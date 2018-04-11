#ifndef _NAIVE_THREAD_PROCESSOR_RELIABLE_H_
#define _NAIVE_THREAD_PROCESSOR_RELIABLE_H_

#include "work_thread/single_work_thread.h"

#include "base_constructor.h"
#include "safe_wait.h"

#include <mutex>
#include <thread>
#include <atomic>

namespace naive {

class SingleWorkThreadImpl : public SingleWorkThread {

public:
		
	explicit SingleWorkThreadImpl();
		
	void Run(std::function<bool()> func) override;

	void Notify() override;

	void Stop() override;

	void SetID(const std::string &id) override;

	const std::string& GetID() const override;

	~SingleWorkThreadImpl();

private:

	std::thread						*_thread;
	std::function<bool()>			_func;

	bool							_shouldStop;
	bool							_threadOver;
	bool							_inited;
	std::atomic<bool>				_isRuning;
	std::atomic<bool>				_isBusy;
	std::mutex						_mtx;
	SafeConditionVariable			_scv;
	SafeConditionVariable			_endScv;

	std::string						_id;

	DISALLOW_COPY_AND_ASSIGN(SingleWorkThreadImpl);

};

}


#endif // !_NAIVE_THREAD_PROCESSOR_RELIABLE_H_
