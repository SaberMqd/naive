#ifndef _NAIVE_WORK_PROCESSOR_POOL_IMPL_H_
#define _NAIVE_WORK_PROCESSOR_POOL_IMPL_H_

#include "work_processor_pool.h"
#include "../ring_object_buffer.h"
#include <map>
#include <list>
#include <atomic>
#include "../work_thread/thread_processor.h"

namespace naive {

	class WorkProcessorPoolImpl : public WorkProcessorPool
	{
	public:

		explicit WorkProcessorPoolImpl();
		
		void SetMaxProcessorCount(uint32_t count) override;

		int CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize = 24) override;
		
		int PostSyncTask(const std::string& name, std::unique_ptr<ProcessorTask> task) override;

		void ReleaseSyncTaskQueue(const std::string& name) override;

		int PostAsyncTask(std::unique_ptr<ProcessorTask> task) override;

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
			~WorkQueue() {
				SafeDelete(_queue);
			}
		};

		std::map<std::string, WorkQueue*> _wps;
		std::list<ThreadProcessor*>		  _tps;
		RingObjBuf<ProcessorTask> *_asyncTaskQueue;

	};

}

#endif // !_NAIVE_WORK_PROCESSOR_POOL_IMPL_H_
