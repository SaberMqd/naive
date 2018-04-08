#include "coroutine.h"

#include <future>

namespace naive {

	CoroutineManager::CoroutineManager(){
		_fiber = ConvertThreadToFiber(nullptr);

	}

	CoroutineManager::~CoroutineManager(){
		DeleteFiber(_fiber);
	}

	CoroutineManager* CoroutineManager::Create(){
		return new CoroutineManager();
	}

	void CoroutineManager::CreateCoroutine(std::function<void()> f) {
		auto future = std::async(std::launch::async, f);
		std::future_status status = future.wait_for(std::chrono::milliseconds(0));

		while (status == std::future_status::timeout){
			YieldCoroutine();
			status = future.wait_for(std::chrono::milliseconds(0));
		}
	}

	void CoroutineManager::YieldCoroutine() {
		
	}

}