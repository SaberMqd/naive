#ifndef _NAIVE_COROUTINE_POOL_H_
#define _NAIVE_COROUTINE_POOL_H_

namespace naive {

class CoroutinePool {

public:

	static CoroutinePool* Create();

	virtual ~CoroutinePool() {}

protected:

	CoroutinePool() {}

};

}

#endif // !_NAIVE_COROUTINE_POOL_H_
