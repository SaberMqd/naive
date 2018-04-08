#include "work_processor_pool_impl.h"


namespace naive {

	WorkProcessorPool* WorkProcessorPool::Instance() {
		static WorkProcessorPoolImpl w;
		return &w;
	}

	WorkProcessorPoolImpl::WorkProcessorPoolImpl() : 
		_asyncTaskQueue(nullptr),
		_maxPorcessorCount(2){
		for (int i = 0; i > _maxPorcessorCount; ++i) {
			auto tp = ThreadProcessor::Create();
			tp->Run([this] {
					std::unique_ptr<ProcessorTask> task1;
					if (_asyncTaskQueue->Pop(task1)) {
						task1->Processor();
						return true;
					}
					for (auto &wp : _wps) {
						if (WorkQueue::FREE == wp.second->_state) {
							wp.second->_state = WorkQueue::BUSY;
							std::unique_ptr<ProcessorTask> task;
							bool ret = wp.second->_queue->Pop(task);
							if (ret && task.get()) {
								task->Processor();
							}
							wp.second->_state = WorkQueue::FREE;
						}
					}
					return true; 
				}
			);
			_tps.push_back(tp);
		}
		_asyncTaskQueue = new RingObjBuf<ProcessorTask>(48);
	}

	WorkProcessorPoolImpl::~WorkProcessorPoolImpl(){
		SafeDeleteStlList(_tps);
		SafeDeleteStlMap(_wps);
		SafeDelete(_asyncTaskQueue);
	}

	int WorkProcessorPoolImpl::PostSyncTask(const std::string& name, std::unique_ptr<ProcessorTask> task) {
		auto wp = _wps.find(name);
		if (wp == _wps.end()) {
			return -1;
		}

		wp->second->_queue->Push(std::move(task));

		if (WorkQueue::FREE == wp->second->_state) {
			for (auto tp : _tps) {
				tp->Notify();
			}
		}

		return 0;
	}

	int WorkProcessorPoolImpl::PostAsyncTask(std::unique_ptr<ProcessorTask> task) {
		_asyncTaskQueue->Push(std::move(task));
		for (auto tp : _tps) {
			tp->Notify();
		}
		return 0;
	}

	int WorkProcessorPoolImpl::CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize) {

		auto wp = _wps.find(name);
		if (wp != _wps.end()) {
			return -1;
		}

		_wps[name]->_queue = new RingObjBuf<ProcessorTask>(maxQueueSize);

		return 0;
	}

	void WorkProcessorPoolImpl::ReleaseSyncTaskQueue(const std::string& name) {
		auto wp = _wps.find(name);
		if (wp == _wps.end()) {
			return;
		}
		while (wp->second->_state != WorkQueue::FREE){
			_sleep(1);
		}
		_wps.erase(wp);
	}

	void WorkProcessorPoolImpl::SetMaxProcessorCount(uint32_t count) {
		_maxPorcessorCount = count;
	}


}