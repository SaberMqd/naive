//#include "test.h"
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
	WPP_INIT(3);
	WPP_CREATE_DEFAULT_SYNC_TASK("1");
	WPP_CREATE_DEFAULT_SYNC_TASK("2");
	uint32_t count = 0;
	int i = 0;
	for (int k = 0; k < 48*10; ++k) {
		auto ret = WPP_POST_SYNC_TASK("1",[&i] {
			cout << "///// " << i <<endl; 
			_sleep(10); 
		});
		if (ret == 0) {
			count++;
		}
		ret = WPP_POST_SYNC_TASK("2", [&i] {
			cout << "***** " << i <<endl;
			_sleep(10);
		});
		if (ret == 0) {
			count++;
		}
		i++;
		if (k == 200) {
			WPP_RELEASE_SYNC_TASK("1");
			WPP_RELEASE_SYNC_TASK("2");
			naive::WorkProcessorPool::Instance()->CreateSyncTaskQueue("3");
		}
		if (k > 150) {
			WPP_POST_SYNC_TASK("3", [&i] {
				cout << "&&&&&& " << i << endl;
				_sleep(10);
			});
		}
		_sleep(14);
	}
	cout << count << endl;

	_sleep(5000);
	//TestCoroutine();
	//TestSyncLog();
	//TestAsyncLog();
	//TestScopeRef();
	//TestWorkThread();

}