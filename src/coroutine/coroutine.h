#ifndef _NAIVE_COROUTINE_H_
#define _NAIVE_COROUTINE_H_

#include <functional>
#include "base_constructor.h"

#include <Windows.h>

namespace naive {

	class CoroutineManager {

	public:

		static CoroutineManager* Create();

		void CreateCoroutine(std::function<void()> f);

		~CoroutineManager();

	private:
		
		explicit CoroutineManager();

		DISALLOW_COPY_AND_ASSIGN(CoroutineManager);

		LPVOID _fiber;

		void YieldCoroutine();

	};

}

#endif // !_NAIVE_COROUTINE_H_
