//
// Created by mqd on 2017/08/15.
//

#ifndef _NAIVE_DEFER_H_
#define _NAIVE_DEFER_H_

#include <functional>

namespace naive {

#define defer _DEFER_ACTION_MAKE /* ([&](){ ... }); */

// auto _defer_action_line???_ = _DeferredActionCtor([&](){ ... })
#define _DEFER_ACTION_MAKE auto \
    _DEFER_ACTION_VAR(_defer_action_line, __LINE__, _) = _DeferredActionCtor
#define _DEFER_ACTION_VAR(a, b, c) _DEFER_TOKEN_CONNECT(a, b, c)
#define _DEFER_TOKEN_CONNECT(a, b, c) a ## b ## c

class _DeferredAction {
private:
	std::function<void()> func_;

	template<typename T>
	friend _DeferredAction _DeferredActionCtor(T&& p);

	template<typename T>
	_DeferredAction(T&& p) : func_(std::bind(std::forward<T>(p))) {

	}

	_DeferredAction();
	_DeferredAction(_DeferredAction const&);
	_DeferredAction& operator=(_DeferredAction const&);
	_DeferredAction& operator=(_DeferredAction&&);

public:
	_DeferredAction(_DeferredAction&& other) :
		func_(std::forward<std::function<void()>>(other.func_)) {
		other.func_ = nullptr;
	}
	~_DeferredAction() {
		if (func_) { func_(); }
	}
};

template<typename T>
_DeferredAction _DeferredActionCtor(T&& p) {
	return _DeferredAction(std::forward<T>(p));
}

}

#endif // !_NAIVE_DEFER_H_