//#include "test.h"
//#include "../src/coroutine.h"
#include "../src/work_thread_pool/work_processor_pool.h"
#include "../src/log/log.h"

#include <iostream>
using namespace std;
using namespace naive;

void main() {

	NaiveLog::Config cfg;
	cfg._level = NaiveLog::NAIVE_INFO;
	cfg._isAsync = true;
	int ret = NaiveLog::GetInstance()->Init(cfg);
	if (0 != ret) {
		return;
	}

	naive::WorkProcessorPool::Instance()->SetMaxProcessorCount(3);

	uint32_t count = 0;
	for (int i = 0; i < 48*10; ++i) {
		auto ret = POST_ASYNC_TASK([] {cout << "lala" << endl;  _sleep(1); });
		if (ret == 0) {
			count++;
		}
	}
	cout << count << endl;

	_sleep(3000);
	//TestCoroutine();
	//TestSyncLog();
	//TestAsyncLog();
	//TestScopeRef();
	//TestWorkThread();

}