#include "test.h"
#include <stdio.h>
void TestWorkThread() {
	
	auto wid = GETWORKTHREADMGR()->CreateWrokThread();
	auto wth = GETWORKTHREADMGR()->GetWrokThread(wid);

	if (wth) {

		wth->SetMaxTaskNum(24);
		wth->Run();

		auto task = [wid] { NE("work thread id is %d", wid); };
		for (int i = 0; i < 10; ++i) {
			wth->PostTask(task);
		}

	}

	//delete this workthread nodelay
	//GETWORKTHREADMGR()->DeleteWorkThread(wid); 

	_sleep(2 * 1000);
}