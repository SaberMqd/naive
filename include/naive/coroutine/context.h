#ifndef _NAIVE_CONTEXT_H_
#define _NAIVE_CONTEXT_H_

namespace naive {

	//struct 


	class Context {

	public:

		static Context* Create();

		//virtual bool Swap() = 0;

		//virtual bool SwapOut() = 0;

		virtual ~Context() {}

	protected:

		explicit Context() {}

	};

}

#endif // !_NAIVE_CONTEXT_H_
