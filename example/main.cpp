//#include "test.h"
//#include "../src/coroutine.h"
#include "../src/work_thread_pool/work_processor_pool.h"
#include "../src/log/log.h"

void main() {

	naive::WorkProcessorPool::Instance()->SetMaxProcessorCount(3);

	struct Task : public naive::ProcessorTask {
		void Processor() {
			static uint32_t i = 0;
			i++;
			NE("%d", i);
		}
	};

	std::unique_ptr<naive::ProcessorTask> t = std::unique_ptr<naive::ProcessorTask>(new Task());
	WPSP("1",std::move(t));
	_sleep(2000);
	WPSTOP("1");

	//TestCoroutine();
	//TestSyncLog();
	//TestAsyncLog();
	//TestScopeRef();
	//TestWorkThread();

}