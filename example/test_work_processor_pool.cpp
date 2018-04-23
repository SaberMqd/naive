#include "test.h"

#include "naive/work_thread/work_thread_pool.h"
#include <iostream>

using namespace std;

void TestWorkProcessorPool() {

	WPP_INIT(3);
	WPP_CREATE_DEFAULT_SYNC_TASK("1");
	WPP_CREATE_DEFAULT_SYNC_TASK("2");

	uint32_t count = 0;
	int i = 0;
	for (int k = 0; k < 48 * 10; ++k) {
		auto ret = WPP_POST_SYNC_TASK("1", [&i] {
			cout << "///// " << i << endl;
			_sleep(10);
		});
		if (ret == 0) {
			count++;
		}
		ret = WPP_POST_SYNC_TASK("2", [&i] {
			cout << "***** " << i << endl;
			_sleep(10);
		});
		if (ret == 0) {
			count++;
		}
		i++;
		if (k == 200) {
			WPP_RELEASE_SYNC_TASK("1");
			WPP_RELEASE_SYNC_TASK("2");
			WPP_CREATE_DEFAULT_SYNC_TASK("3");
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


}