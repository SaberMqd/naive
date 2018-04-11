#ifndef _NAIVE_COROUTINE_POOL_IMPL_H_
#define _NAIVE_COROUTINE_POOL_IMPL_H_

#include "coroutine_pool/coroutine_pool.h"
#include "base_constructor.h"

namespace naive {

	class CoroutinePoolImpl : public CoroutinePool
	{
	public:

		explicit CoroutinePoolImpl();

		~CoroutinePoolImpl();

	private:

		DISALLOW_COPY_AND_ASSIGN(CoroutinePoolImpl);

	};

}

#endif // !_NAIVE_COROUTINE_POOL_IMPL_H_
