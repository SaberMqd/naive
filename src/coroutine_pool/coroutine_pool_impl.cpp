#include "coroutine_pool_impl.h"

namespace naive {

	CoroutinePool* CoroutinePool::Create() {
		return new CoroutinePoolImpl();
	}

	CoroutinePoolImpl::CoroutinePoolImpl(){
	}

	CoroutinePoolImpl::~CoroutinePoolImpl(){
	}

}