#ifndef _NAIVE_BLOCK_SINGLE_WORK_THREAD_H_
#define _NAIVE_BLOCK_SINGLE_WORK_THREAD_H_

#include <functional>
#include <string>

namespace naive {

	class BlockSingleWorkThread
	{
	public:

		typedef std::function<bool()> ReadFunc;

		typedef std::function<void()> InterruptFunc;

		static BlockSingleWorkThread* Create();

		virtual void Run() = 0;

		virtual void Stop() = 0;

		virtual void RegisterReadFunc(ReadFunc func) = 0;

		virtual void RegisterInterruptFunc(InterruptFunc func) = 0;

		virtual ~BlockSingleWorkThread() {};

	protected:

		BlockSingleWorkThread() {}

	};

}

#endif // !_NAIVE_BLOCK_SINGLE_WORK_THREAD_H_
