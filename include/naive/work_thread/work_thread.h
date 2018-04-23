#ifndef _NAIVE_WORK_THREAD_H_
#define _NAIVE_WORK_THREAD_H_

#include "single_work_thread.h"
#include "ring_object_buffer.h"
#include "base_constructor.h"
#include "work_task.h"

#include <map>
#include <shared_mutex>

namespace naive {

#define WPM_CREATE_WORK naive::WorkThreadManager::GetInstance()->CreateWrokThread
#define WPM_GET_WORK(x) naive::WorkThreadManager::GetInstance()->GetWrokThread(x)
#define WPM_DELETE_WORK(x) naive::WorkThreadManager::GetInstance()->DeleteWorkThread(x) 

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

class WorkThreadManager;

class WorkThread
{
public:
		
	explicit WorkThread(uint32_t id) :
		_maxAsyncBufSize(48),
		_tp(SingleWorkThread::Create()),
		_runing(false),
		_id(id),
		_asyncBuf(nullptr){
		
	}

	void SetMaxTaskNum(uint32_t count) {
		_maxAsyncBufSize = count;
	}

	void Reset() {
		_smtx.lock();
		_asyncBuf->Reset();
		_smtx.unlock();
	}

	void Run() {
		if (_runing) {
			return;
		}
		_runing = true;
		_asyncBuf = new RingObjBuf<WorkTask>(_maxAsyncBufSize);

		_tp->Run([this] {
			std::unique_ptr<WorkTask> task;

			_smtx.lock_shared();
			bool ret = _asyncBuf->Pop(task);
			_smtx.unlock_shared();
			if (ret && task.get()) {
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
		_smtx.lock_shared();
		bool ret = _asyncBuf->Push(std::move(task));
		_smtx.unlock_shared();
		if (!ret) {
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

private:

	friend WorkThreadManager;

	~WorkThread() {
		Stop();
		SafeDelete(_tp);
		SafeDelete(_asyncBuf);
	}
	
	std::mutex				_mtx;
	std::shared_mutex		_smtx;
	SingleWorkThread		*_tp;
	RingObjBuf<WorkTask>	*_asyncBuf;
	bool					_runing;
	uint32_t				_maxAsyncBufSize;
	uint32_t				_id;
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
		for (auto &v : _wts) {
			delete v.second;
			v.second = nullptr;
		}
		_wts.clear();
	}

	uint32_t CreateWrokThread() {
		std::unique_lock<std::mutex> lck(_mtx);
		_id++;
		_wts[_id] = new WorkThread(_id);
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
