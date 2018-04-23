#ifndef _NAIVE_COROUTINE_IMPL_H_
#define _NAIVE_COROUTINE_IMPL_H_

#include "naive/coroutine/coroutine.h"
#include "naive/base_constructor.h"

namespace naive {

	class CoroutineImpl : public Coroutine {

	public:

		explicit CoroutineImpl();
		
		void PostTask(std::function<void()> func) override;

		void PostTask(std::unique_ptr<WorkTask> task) override;

		void Yield() override;

		~CoroutineImpl();

	private:
		
		DISALLOW_COPY_AND_ASSIGN(CoroutineImpl);

	};

}

#endif // !_NAIVE_COROUTINE_IMPL_H_
