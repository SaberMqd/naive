#include "test.h"

void TestWorkThread() {
	
	auto wid = WPM_CREATE_WORK();
	auto wth = WPM_GET_WORK(wid);

	if (wth) {

		wth->SetMaxTaskNum(24);
		wth->Run();

		auto task = [wid] { NE("work thread id is %d", wid); };
		for (int i = 0; i < 10; ++i) {
			wth->PostTask(task);
		}

	}

	//delete this workthread nodelay
	WPM_DELETE_WORK(wid); 

	_sleep(2 * 1000);
}