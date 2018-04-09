#ifndef _NAIVE_WORK_PROCESSOR_POOL_IMPL_H_
#define _NAIVE_WORK_PROCESSOR_POOL_IMPL_H_

#include "work_processor_pool.h"
#include "../ring_object_buffer.h"
#include <map>
#include <list>
#include <atomic>
#include "../work_thread/thread_processor.h"
#include <mutex>
#include "../lock/read_write_lock.h"
#include <shared_mutex>

namespace naive {

	class WorkProcessorPoolImpl : public WorkProcessorPool
	{
	public:

		explicit WorkProcessorPoolImpl();
		
		void Init(uint32_t defaultProcessorCount) override;

		int CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize) override;
		
		int PostSyncTask(const std::string& name, std::unique_ptr<ProcessorTask> task) override;

		int PostSyncTask(const std::string& name, std::function<void()> func) override;

		void ReleaseSyncTaskQueue(const std::string& name) override;

		int PostAsyncTask(std::unique_ptr<ProcessorTask> task) override;

		int PostAsyncTask(std::function<void()> func) override;

		~WorkProcessorPoolImpl();

	private:
		
		uint32_t _maxPorcessorCount;
		struct WorkQueue {
			enum State {
				FREE = 0,
				BUSY,
			};
			std::atomic<State> _state = FREE;
			RingObjBuf<ProcessorTask> *_queue = nullptr;
			std::mutex _mtx;
			~WorkQueue() {
				SafeDelete(_queue);
			}
		};

		struct CluserTask : public ProcessorTask {
			std::function<void()> _f = nullptr;
			void Processor() {
				_f();
			}
		};

		std::map<std::string, WorkQueue*> _wps;
		std::list<ThreadProcessor*>		  _tps;
		RingObjBuf<ProcessorTask> *_asyncTaskQueue;

		std::mutex _pushMtx;
		std::mutex _popMtx;
		std::shared_mutex _shareMtx;
	};

}

#endif // !_NAIVE_WORK_PROCESSOR_POOL_IMPL_H_
