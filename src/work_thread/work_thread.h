#ifndef _NAIVE_WORK_THREAD_H_
#define _NAIVE_WORK_THREAD_H_

#include "thread_processor.h"
#include "../ring_object_buffer.h"
#include "../base_constructor.h"
//#include "../log/log.h"

#include <map>

namespace naive {

#define GETWORKTHREADMGR WorkThreadManager::GetInstance

class WorkTask {
public:
	virtual bool Process() = 0;
	virtual ~WorkTask() {}
	explicit WorkTask() {}
private:
	DISALLOW_COPY_AND_ASSIGN(WorkTask);
};

	
template <class Closure>
class ClosureTask : public WorkTask {
public:
	explicit ClosureTask(const Closure& closure) : _closure(closure) {}
	bool Process() override {
		_closure();
		return true;
	}
	Closure _closure;
};
	
class WorkThread
{
public:
		
	WorkThread() :
		_maxAsyncBufSize(48),
		_tp(ThreadProcessor::Create()),
		_asyncBuf(nullptr){
		
	}

	void SetMaxTaskNum(uint32_t count) {
		_maxAsyncBufSize = count;
	}

	void Run() {
		if (_runing) {
			return;
		}
		_runing = true;
		_asyncBuf = new RingObjBuf<WorkTask>(_maxAsyncBufSize);

		_tp->Run([this] {
			std::unique_ptr<WorkTask> task;
			if (_asyncBuf->Pop(task) && task.get()) {
				return task->Process();
			}
			return true;
		});

	}

	template <class WorkTask>
	int PostTask(std::unique_ptr<WorkTask> task) {
		if (!_runing) {
			return -1;
		}
		std::lock_guard<std::mutex> lck(_mtx);
		if (!_asyncBuf->Push(std::move(task))) {
			//ND("buffer is full");
			return -1;
		}
		_tp->Notify();
		return 0;
	}

	template <class Closure>
	void PostTask(const Closure& closure) {
		PostTask(std::move(std::unique_ptr<WorkTask>(new ClosureTask<Closure>(closure))));
	}

	void Stop() {
		if (_tp) {
			_tp->Stop();
		}
	}

	~WorkThread() {
		Stop();
		SafeDelete(_tp);
		SafeDelete(_asyncBuf);
	}

private:
		
	std::mutex				_mtx;
	ThreadProcessor			*_tp;
	RingObjBuf<WorkTask>	*_asyncBuf;
	bool					_runing;
	uint32_t				_maxAsyncBufSize;
	DISALLOW_COPY_AND_ASSIGN(WorkThread);

};

class WorkThreadManager
{
public:

	static WorkThreadManager* GetInstance() {
		static WorkThreadManager t;
		return &t;
	};

	WorkThreadManager() : _id(0) {}

	~WorkThreadManager() {
		SafeDeleteStlMap(_wts);
	}

	uint32_t CreateWrokThread() {
		std::unique_lock<std::mutex> lck(_mtx);
		_id++;
		_wts[_id] = new WorkThread();
		return _id;
	}

	void DeleteWorkThread(uint32_t id) {
		std::unique_lock<std::mutex> lck(_mtx);
		auto th = _wts.find(id);
		if (th == _wts.end()) {
			return;
		}
		delete th->second;
		_wts.erase(id);
	}

	WorkThread* GetWrokThread(uint32_t id) {
		std::unique_lock<std::mutex> lck(_mtx);
		if (_wts.find(id) != _wts.end()) {
			return _wts[id];
		}
		return nullptr;
	}

private:

	uint32_t			_id;
	std::mutex			_mtx;
	std::map<uint32_t, WorkThread*> _wts;

	DISALLOW_COPY_AND_ASSIGN(WorkThreadManager);
};

}

#endif // !_NAIVE_WORK_THREAD_H_
