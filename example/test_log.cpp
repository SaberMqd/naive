#include "test.h"

void TestLogDelete() {
	NaiveLog::Config cfg;
	cfg._level = NaiveLog::NAIVE_INFO;
	cfg._isAsync = true;
	int ret = NaiveLog::GetInstance()->Init(cfg);
	if (0 != ret) {
		return;
	}
}

void TestSyncLog() {
	NaiveLog::Config cfg;
	cfg._level = NaiveLog::NAIVE_INFO;
	int ret = NaiveLog::GetInstance()->Init(cfg);
	if (0 != ret) {
		return;
	}

	ND("hello world");
	NE("today is %d/%d/%d", 2018, 3, 23);
}

void TestAsyncLog() {
	NaiveLog::Config cfg;
	cfg._level = NaiveLog::NAIVE_INFO;
	cfg._isAsync = true;
	int ret = NaiveLog::GetInstance()->Init(cfg);
	if (0 != ret) {
		return;
	}

	std::thread t(
		[] {
			for (int i = 0; i < 10; ++i) {
				ND("thread one %d",i);
				_sleep(100);
			}
		}
	);

	t.detach();

	std::thread t1(
		[] {
		for (int i = 0; i < 10; ++i) {
			ND("thread two %d", i);
			_sleep(100);
		}
	}
	);

	t1.detach();

	ND("hello world");
	NE("today is %d/%d/%d", 2018, 3, 23);

	_sleep(10000);
}