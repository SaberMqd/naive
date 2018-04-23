#include "test.h"

#include <Windows.h>
#include <thread>
#include <iostream>
#include <functional>
#include <list>
#include "naive/safe_wait.h"
//#include "../src/coroutine.h"
using namespace std;

class CoScheduler
{
public:
	
	CoScheduler() {
	}

	~CoScheduler() {
	}

	void CreateCo(std::function<void()> func) {
		auto co = new Coroutine();
		co->_func = func;
		if (_front == nullptr) {
			_front = co;
			_tail = _front;
			return;
		}
		co->pre = _tail->next;
		_tail->next = co;
		_tail = _tail->next;
	}

	void Run() {
		std::thread t([this] {
			LPVOID main_fiber = ConvertThreadToFiber(nullptr);
			Coroutine *tmp = _front;
			while (tmp) {
				tmp->_fiber = CreateFiberEx(4 * 1024, 4 * 1024, FIBER_FLAG_FLOAT_SWITCH, LPFIBER_START_ROUTINE(FiberFunc), &tmp->_func);
				tmp = tmp->next;
			}
			tmp = _front;
			while (tmp) {
				if (_shouldOver) {
					break;
				}
				if (Coroutine::CLOSE != tmp->_state) {
					SwitchToFiber(tmp->_fiber);
					if (tmp->_state == Coroutine::CLOSE) {
						Coroutine *ctmp = tmp->pre;
						tmp->pre->next = tmp->next;
						if (tmp->next) {
							tmp->next->pre = tmp->pre;
						} else {
							
						}
						delete tmp;
						tmp = ctmp;
					}
				}
				tmp = tmp->next;
			}
			_scv.Notify();
		});
		t.detach();
	}

	void YieldCo() {
		CurrentCoroutine()->_state = Coroutine::SUSPEND;

	}

	void Stop() {
		_scv.Wait();
	}

private:
	struct Coroutine {
		std::function<void()> _func = nullptr;
		LPVOID _fiber = nullptr;
		enum {
			INIT = 0,
			SUSPEND,
			RUNING,
			CLOSE
		}_state = INIT;
		Coroutine *pre = nullptr;
		Coroutine *next = nullptr;
		~Coroutine(){
			if (nullptr != _fiber) {
				DeleteFiber(_fiber);
			}
		}
	};
	Coroutine* _front = nullptr;
	Coroutine* _tail = nullptr;
	bool _shouldOver = false;
	naive::SafeConditionVariable _scv;


	static VOID WINAPI FiberFunc(LPVOID param)
	{
		Coroutine *co = (Coroutine*)param;
		if (co->_func) {
			co->_func();
			co->_state = Coroutine::CLOSE;
			co->next->pre = co->pre;
			co->pre->next = co->next;
			delete co;
		}
	};

	Coroutine* CurrentCoroutine() {
		return nullptr;
	}

};

static VOID WINAPI FiberFunc(LPVOID param)
{
	std::function<void()> *f = (std::function<void()>*)param;
	(*f)();
};

void main() {
	/*
	CoScheduler cos;
	cos.CreateCo([&cos] {
		while (true) {
			//getdata to do something
			cos.YieldCo();
		}
	});
	cos.CreateCo([&cos] {
		while (true) {
			//cteate data
			cos.YieldCo();
		}
	});
	cos.Run();
	*/
	///*
	std::thread t([] {
		LPVOID main_fiber = ConvertThreadToFiber(nullptr);
		std::function<void()> func = [&main_fiber] {
			cout << "file000  11   " << main_fiber <<endl;
			SwitchToFiber(main_fiber);
			cout << "file000  22   " << main_fiber << endl;
		};
		LPVOID sub_thread_fiber = CreateFiberEx(4 * 1024, 4 * 1024, FIBER_FLAG_FLOAT_SWITCH, LPFIBER_START_ROUTINE(FiberFunc), &func);
		std::function<void()> func1 = [=,&main_fiber] {
			cout << "filedsfsdf 11   " << main_fiber << endl;
			DeleteFiber(sub_thread_fiber);
			//SwitchToFiber(sub_thread_fiber);
			cout << "filedsfsdf  22   " << main_fiber << endl;
			//SwitchToFiber(main_fiber);
		};
		LPVOID sub_thread_fiber1 = CreateFiberEx(4 * 1024, 4 * 1024, FIBER_FLAG_FLOAT_SWITCH, LPFIBER_START_ROUTINE(FiberFunc), &func1);
		cout << "asd" << endl;
		SwitchToFiber(sub_thread_fiber1);
		//cout <<"??  " << &sub_thread_fiber << endl;
		cout << "asd" << endl;
		//DeleteFiber(sub_thread_fiber1);
		//DeleteFiber(sub_thread_fiber);
		SwitchToFiber(sub_thread_fiber); 
		ConvertFiberToThread();
		cout << "asdasd" << endl;
	});
	t.join();
	//*/
	/*
	std::thread t1([] {
		LPVOID main_fiber = ConvertThreadToFiber(nullptr);
		cout << main_fiber << endl;
		std::function<void()> func = [&main_fiber] {
			cout << "file1 " << main_fiber << endl;
			SwitchToFiber(main_fiber);
		};
		std::function<void()> func1 = [&main_fiber] {
			cout << "file1 " << main_fiber << endl;
			SwitchToFiber(main_fiber);
		};
		PVOID sub_thread_fiber = CreateFiberEx(4 * 1024, 4 * 1024, FIBER_FLAG_FLOAT_SWITCH, LPFIBER_START_ROUTINE(FiberFunc), &func);
		PVOID sub_thread_fiber1 = CreateFiberEx(4 * 1024, 4 * 1024, FIBER_FLAG_FLOAT_SWITCH, LPFIBER_START_ROUTINE(FiberFunc), &func1);
		SwitchToFiber(sub_thread_fiber);
		cout << "asdasd1 " << endl;
		SwitchToFiber(sub_thread_fiber1);
	});
	t1.detach();
	*/
	//_sleep(3000);
	// SwitchToFiber(sub_thread_fiber);

	//TestCoroutine();
	//TestSyncLog();
	//TestAsyncLog();
	//TestScopeRef();
	//TestWorkThread();
	//TestWorkProcessorPool();
}
