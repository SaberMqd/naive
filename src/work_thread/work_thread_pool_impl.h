#ifndef _NAIVE_WORK_THREAD_POOL_IMPL_H_
#define _NAIVE_WORK_THREAD_POOL_IMPL_H_

#include "naive/work_thread/work_thread_pool.h"
#include <map>
#include <list>
#include <atomic>
#include <mutex>
#include <shared_mutex>

#include "naive/work_thread/single_work_thread.h"
#include "naive/read_write_lock.h"
#include "naive/ring_object_buffer.h"

namespace naive {

	class WorkThreadPoolImpl : public WorkThreadPool
	{
	public:

		explicit WorkThreadPoolImpl();
		
		void Init(uint32_t defaultProcessorCount) override;

		int CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize) override;
		
		int PostSyncTask(const std::string& name, std::unique_ptr<WorkTask> task) override;

		int PostSyncTask(const std::string& name, std::function<void()> func) override;

		void ReleaseSyncTaskQueue(const std::string& name) override;

		int PostAsyncTask(std::unique_ptr<WorkTask> task) override;

		int PostAsyncTask(std::function<void()> func) override;

		~WorkThreadPoolImpl();

	private:
		
		uint32_t _maxProcessorCount;
		struct WorkQueue {
			enum State {
				FREE = 0,
				BUSY,
			};
			std::atomic<State> _state = FREE;
			RingObjBuf<WorkTask> *_queue = nullptr;
			std::mutex _mtx;
			~WorkQueue() {
				SafeDelete(_queue);
			}
		};

		struct CluserTask : public WorkTask {
			std::function<void()> _f = nullptr;
			bool Process() {
				_f();
				return true;
			}
		};

		std::map<std::string, WorkQueue*> _wps;
		std::list<SingleWorkThread*>		  _tps;
		RingObjBuf<WorkTask> *_asyncTaskQueue;

		std::mutex _pushMtx;
		std::mutex _popMtx;
		std::shared_mutex _shareMtx;
	};

}

#endif // !_NAIVE_WORK_THREAD_POOL_IMPL_H_
