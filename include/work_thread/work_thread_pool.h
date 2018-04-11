#ifndef _NAIVE_WORK_THREAD_POOL_H_
#define _NAIVE_WORK_THREAD_POOL_H_

#include <memory>
#include <string>
#include <cstdint>
#include <functional>

namespace naive {

#define WPP_INIT(x)  naive::WorkThreadPool::Instance()->Init(x)

#define WPP_POST_SYNC_TASK(x,y) naive::WorkThreadPool::Instance()->PostSyncTask((x),(y))
#define WPP_POST_ASYNC_TASK(x) naive::WorkThreadPool::Instance()->PostAsyncTask(x)

#define WPP_CREATE_DEFAULT_SYNC_TASK(x) naive::WorkThreadPool::Instance()->CreateSyncTaskQueue(x)
#define WPP_CREATE_SYNC_TASK(x,y) naive::WorkThreadPool::Instance()->CreateSyncTaskQueue(x,y)

#define WPP_RELEASE_SYNC_TASK(x) naive::WorkThreadPool::Instance()->ReleaseSyncTaskQueue(x)

	struct ProcessorTask{
		virtual void Processor() = 0;
		virtual ~ProcessorTask() {}
	};

	class WorkThreadPool {
	
	public:

		static WorkThreadPool* Instance();
		
		virtual void Init(uint32_t defaultProcessorCount = 2) = 0;

		virtual int CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize = 24) = 0;
		
		virtual int PostSyncTask(const std::string& name, std::unique_ptr<ProcessorTask> task) = 0;
		
		virtual int PostSyncTask(const std::string& name, std::function<void()> func) = 0;

		virtual void ReleaseSyncTaskQueue(const std::string& name) = 0;

		virtual int PostAsyncTask(std::unique_ptr<ProcessorTask> task) = 0;

		virtual int PostAsyncTask(std::function<void()> func) = 0;

		virtual  ~WorkThreadPool() {}

	protected:

		WorkThreadPool(){}

	};

}

#endif // !_NAIVE_WORK_THREAD_POOL_H_
