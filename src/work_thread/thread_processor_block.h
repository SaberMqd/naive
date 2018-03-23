#ifndef _NAIVE_THREAD_PROCESSOR_BLOCK_H_
#define _NAIVE_THREAD_PROCESSOR_BLOCK_H_

#include "thread_processor.h"
#include "../safe_wait.h"
#include "../base_constructor.h"

#include <thread>

namespace naive {

class BlockThreadProcessorImpl : public BlcokThreadProcessor {

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


#endif // !_NAIVE_THREAD_PROCESSOR_BLOCK_H_
