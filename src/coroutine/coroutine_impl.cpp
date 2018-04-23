#include "coroutine_impl.h"

namespace naive {

	Coroutine* Coroutine::Create(){
		return new CoroutineImpl();
	}
	
	CoroutineImpl::CoroutineImpl(){

	}

	CoroutineImpl::~CoroutineImpl(){

	}

	void CoroutineImpl::PostTask(std::function<void()> func) {

	}

	void CoroutineImpl::PostTask(std::unique_ptr<WorkTask> task) {

	}

	void CoroutineImpl::Yield() {

	}


}