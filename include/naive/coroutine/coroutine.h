#ifndef _NAIVE_COROUTINE_H_
#define _NAIVE_COROUTINE_H_

#include <functional>
#include <memory>

#include "naive/work_task.h"

namespace naive {

class Coroutine {

public:

	static Coroutine* Create();

	virtual void PostTask(std::function<void()> func) = 0;

	virtual void PostTask(std::unique_ptr<WorkTask> task) = 0;

	virtual void Yield() = 0;

	virtual ~Coroutine() {}

protected:

	Coroutine() {}

};

}

#endif // !_NAIVE_COROUTINE_H_
