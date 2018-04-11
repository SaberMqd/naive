#ifndef _NAIVE_SINGLE_WORK_THREAD_H_
#define _NAIVE_SINGLE_WORK_THREAD_H_

#include <functional>
#include <string>

namespace naive {

class SingleWorkThread {
	
public:

	static SingleWorkThread* Create();

	virtual void Run(std::function<bool()>) = 0;

	virtual void Notify() = 0;

	virtual void Stop() = 0;

	virtual void SetID(const std::string &id) = 0;
	
	virtual const std::string& GetID() const = 0;

	virtual ~SingleWorkThread() {}

protected:

	SingleWorkThread() {}
};

}

#endif // !_NAIVE_SINGLE_WORK_THREAD_H_
