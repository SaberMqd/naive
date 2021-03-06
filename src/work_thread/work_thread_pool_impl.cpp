#include "work_thread_pool_impl.h"
#include "naive/defer.h"

namespace naive {

WorkThreadPool* WorkThreadPool::Instance() {
	static WorkThreadPoolImpl w;
	return &w;
}

WorkThreadPoolImpl::WorkThreadPoolImpl() :
	_asyncTaskQueue(nullptr),
	_maxProcessorCount(0){

}

WorkThreadPoolImpl::~WorkThreadPoolImpl(){
	SafeDeleteStlList(_tps);
	SafeDeleteStlMap(_wps);
	SafeDelete(_asyncTaskQueue);
}

void WorkThreadPoolImpl::Init(uint32_t defaultProcessorCount) {
	_maxProcessorCount = defaultProcessorCount;
	for (uint32_t i = 0; i < _maxProcessorCount; ++i) {
		auto tp = SingleWorkThread::Create();
		tp->Run([this, tp] {
			{
				_shareMtx.lock_shared();
				defer([this] { _shareMtx.unlock_shared(); });
				for (auto &wp : _wps) {
					if (WorkQueue::FREE != wp.second->_state) {
						continue;
					}
					bool ret = false;
					std::unique_ptr<WorkTask> task;
					{
						wp.second->_mtx.lock();
						if (WorkQueue::FREE == wp.second->_state) {
							wp.second->_state = WorkQueue::BUSY;
							ret = wp.second->_queue->Pop(task);
						}
						wp.second->_mtx.unlock();
					}

					{
						wp.second->_mtx.lock();
						if (ret && task.get()) {
							task->Process();
						}
						wp.second->_state = WorkQueue::FREE;
						wp.second->_mtx.unlock();
						return true;
					}
				}
			}
			std::unique_ptr<WorkTask> task1;
			_popMtx.lock();
			bool ret = _asyncTaskQueue->Pop(task1);
			_popMtx.unlock();
			if (ret && task1.get()) {
				task1->Process();
				return true;
			}
			return true;
		});
		_tps.push_back(tp);
	}
	_asyncTaskQueue = new RingObjBuf<WorkTask>(48);
}

int WorkThreadPoolImpl::PostSyncTask(const std::string& name, std::unique_ptr<WorkTask> task) {
	_shareMtx.lock_shared();
	defer([this] { _shareMtx.unlock_shared(); });
	auto wp = _wps.find(name);
	if (wp == _wps.end()) {
		return -1;
	}

	bool ret = wp->second->_queue->Push(std::move(task));
	if (!ret) {
		return -1;
	}
	for (auto tp : _tps) {
		tp->Notify();
	}
		
	return 0;
}

int WorkThreadPoolImpl::PostSyncTask(const std::string& name, std::function<void()> func) {
	std::unique_ptr<CluserTask> t(new CluserTask());
	t->_f = func;
	return PostSyncTask(name, std::move(t));
}

int WorkThreadPoolImpl::PostAsyncTask(std::unique_ptr<WorkTask> task) {
	bool ret = _asyncTaskQueue->Push(std::move(task));
	for (auto tp : _tps) {
		tp->Notify();
	}
	return ret ? 0:-1;
}

int WorkThreadPoolImpl::PostAsyncTask(std::function<void()> func) {
	std::unique_ptr<CluserTask> t(new CluserTask());
	t->_f = func;
	return PostAsyncTask(std::move(t));
}

int WorkThreadPoolImpl::CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize) {
	_shareMtx.lock();
	defer([this] {_shareMtx.unlock(); });
	auto wp = _wps.find(name);
	if (wp != _wps.end()) {
		return -1;
	}

	_wps[name] = new WorkQueue();
	_wps[name]->_queue = new RingObjBuf<WorkTask>(maxQueueSize);

	return 0;
}

void WorkThreadPoolImpl::ReleaseSyncTaskQueue(const std::string& name) {
	_shareMtx.lock();
	defer([this] {_shareMtx.unlock(); });
	auto wp = _wps.find(name);
	if (wp == _wps.end()) {
		return;
	}
	while (wp->second->_state != WorkQueue::FREE){}
	_wps.erase(wp);
}

}