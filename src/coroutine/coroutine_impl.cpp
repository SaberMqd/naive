#include "coroutine_impl.h"

namespace naive {

	Coroutine* Coroutine::Create(){
		return new CoroutineImpl();
	}
	
	CoroutineImpl::CoroutineImpl(){

	}

	CoroutineImpl::~CoroutineImpl(){

	}


}