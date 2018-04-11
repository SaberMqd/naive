#ifndef _NAIVE_COROUTINE_IMPL_H_
#define _NAIVE_COROUTINE_IMPL_H_

#include "coroutine/coroutine.h"
#include "base_constructor.h"

namespace naive {

	class CoroutineImpl : public Coroutine {

	public:

		explicit CoroutineImpl();
		
		~CoroutineImpl();

	private:
		

		DISALLOW_COPY_AND_ASSIGN(CoroutineImpl);

	};

}

#endif // !_NAIVE_COROUTINE_IMPL_H_
