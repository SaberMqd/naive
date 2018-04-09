#include "work_processor_pool_impl.h"
#include "../defer.h"

namespace naive {

	WorkProcessorPool* WorkProcessorPool::Instance() {
		static WorkProcessorPoolImpl w;
		return &w;
	}

	WorkProcessorPoolImpl::WorkProcessorPoolImpl() : 
		_asyncTaskQueue(nullptr),
		_maxPorcessorCount(0){

	}

	WorkProcessorPoolImpl::~WorkProcessorPoolImpl(){
		SafeDeleteStlList(_tps);
		SafeDeleteStlMap(_wps);
		SafeDelete(_asyncTaskQueue);
	}

	void WorkProcessorPoolImpl::Init(uint32_t defaultProcessorCount) {
		_maxPorcessorCount = defaultProcessorCount;
		for (int i = 0; i < _maxPorcessorCount; ++i) {
			auto tp = ThreadProcessor::Create();
			tp->Run([this, tp] {
				{
					_shareMtx.lock_shared();
					defer([this] { _shareMtx.unlock_shared(); });
					for (auto &wp : _wps) {
						if (WorkQueue::FREE != wp.second->_state) {
							continue;
						}
						bool ret = false;
						std::unique_ptr<ProcessorTask> task;
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
								task->Processor();
							}
							wp.second->_state = WorkQueue::FREE;
							wp.second->_mtx.unlock();
							return true;
						}
					}
				}
				std::unique_ptr<ProcessorTask> task1;
				_popMtx.lock();
				bool ret = _asyncTaskQueue->Pop(task1);
				_popMtx.unlock();
				if (ret && task1.get()) {
					task1->Processor();
					return true;
				}
				return true;
			});
			_tps.push_back(tp);
		}
		_asyncTaskQueue = new RingObjBuf<ProcessorTask>(48);
	}

	int WorkProcessorPoolImpl::PostSyncTask(const std::string& name, std::unique_ptr<ProcessorTask> task) {
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

	int WorkProcessorPoolImpl::PostSyncTask(const std::string& name, std::function<void()> func) {
		std::unique_ptr<CluserTask> t(new CluserTask());
		t->_f = func;
		return PostSyncTask(name, std::move(t));
	}

	int WorkProcessorPoolImpl::PostAsyncTask(std::unique_ptr<ProcessorTask> task) {
		bool ret = _asyncTaskQueue->Push(std::move(task));
		for (auto tp : _tps) {
			tp->Notify();
		}
		return ret ? 0:-1;
	}

	int WorkProcessorPoolImpl::PostAsyncTask(std::function<void()> func) {
		std::unique_ptr<CluserTask> t(new CluserTask());
		t->_f = func;
		return PostAsyncTask(std::move(t));
	}

	int WorkProcessorPoolImpl::CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize) {
		_shareMtx.lock();
		defer([this] {_shareMtx.unlock(); });
		auto wp = _wps.find(name);
		if (wp != _wps.end()) {
			return -1;
		}

		_wps[name] = new WorkQueue();
		_wps[name]->_queue = new RingObjBuf<ProcessorTask>(maxQueueSize);

		return 0;
	}

	void WorkProcessorPoolImpl::ReleaseSyncTaskQueue(const std::string& name) {
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