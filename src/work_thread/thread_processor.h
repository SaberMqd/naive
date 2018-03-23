#ifndef _NAIVE_THREAD_PROCESSOR_H_
#define _NAIVE_THREAD_PROCESSOR_H_

#include <functional>

namespace naive {

class ThreadProcessor {
	
public:

	static ThreadProcessor* Create();

	virtual void Run(std::function<bool()>) = 0;

	virtual void Notify() = 0;

	virtual void Stop() = 0;

	virtual ~ThreadProcessor() {}

protected:

	ThreadProcessor() {}
};

class BlcokThreadProcessor
{
public:

	typedef std::function<bool()> ReadFunc;

	typedef std::function<void()> InterruptFunc;

	static BlcokThreadProcessor* Create();

	virtual void Run() = 0;

	virtual void Stop() = 0;

	virtual void RegisterReadFunc(ReadFunc func) = 0;

	virtual void RegisterInterruptFunc(InterruptFunc func) = 0;

	virtual ~BlcokThreadProcessor() {};

protected:

	BlcokThreadProcessor() {}

};

}

#endif // !_NAIVE_THREAD_PROCESSOR_H_
