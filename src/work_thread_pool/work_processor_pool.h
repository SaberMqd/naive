#ifndef _NAIVE_WORK_PROCESSOR_POOL_H_
#define _NAIVE_WORK_PROCESSOR_POOL_H_

#include <memory>
#include <string>
#include <cstdint>

namespace naive {

#define WPSP(x,y) naive::WorkProcessorPool::Instance()->PostSyncTask((x),(y))
#define WPAP(x) naive::WorkProcessorPool::Instance()->PostAsyncTask(x)

#define WPSCRT(x,y) naive::WorkProcessorPool::Instance()->CreateSyncTaskQueue((x),(y))
#define WPSTOP(x) naive::WorkProcessorPool::Instance()->ReleaseSyncTaskQueue(x)

	struct ProcessorTask{
		virtual void Processor() = 0;
		virtual ~ProcessorTask() {}
	};

	class WorkProcessorPool {
	
	public:

		static WorkProcessorPool* Instance();
		
		//default processor count is 1  
		virtual void SetMaxProcessorCount(uint32_t count) = 0;

		virtual int CreateSyncTaskQueue(const std::string& name, uint32_t maxQueueSize) = 0;
		
		virtual int PostSyncTask(const std::string& name, std::unique_ptr<ProcessorTask> task) = 0;
		
		virtual void ReleaseSyncTaskQueue(const std::string& name) = 0;

		virtual int PostAsyncTask(std::unique_ptr<ProcessorTask> task) = 0;

		virtual  ~WorkProcessorPool() {}

	protected:

		WorkProcessorPool(){}

	};

}

#endif // !_NAIVE_WORK_PROCESSOR_POOL_H_
