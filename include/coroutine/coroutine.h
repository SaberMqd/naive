#ifndef _NAIVE_COROUTINE_H_
#define _NAIVE_COROUTINE_H_

namespace naive {

class Coroutine {

public:

	static Coroutine* Create();

	virtual ~Coroutine() {}

protected:

	Coroutine() {}

};

}

#endif // !_NAIVE_COROUTINE_H_
