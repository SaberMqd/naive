#ifndef _NAIVE_BLOCK_SINGLE_WORK_THREAD_IMPL_H_
#define _NAIVE_BLOCK_SINGLE_WORK_THREAD_IMPL_H_

#include "work_thread/block_single_work_thread.h"
#include "safe_wait.h"
#include "base_constructor.h"

#include <thread>

namespace naive {

class BlockThreadProcessorImpl : public BlockSingleWorkThread {

public:
		
	explicit BlockThreadProcessorImpl();

	void RegisterReadFunc(ReadFunc func) override;

	void RegisterInterruptFunc(InterruptFunc func) override;

	void Run() override;

	void Stop() override;

	~BlockThreadProcessorImpl();

private:

	ReadFunc						_func;
	InterruptFunc					_interrFunc;

	std::thread						*_thread;

	bool							_inited;
	SafeConditionVariable			_endScv;

	DISALLOW_COPY_AND_ASSIGN(BlockThreadProcessorImpl);

};

}


#endif // !_NAIVE_BLOCK_SINGLE_WORK_THREAD_IMPL_H_
