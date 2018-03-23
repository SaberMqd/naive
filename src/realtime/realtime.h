#ifndef _NAIVE_REALTIME_H_
#define _NAIVE_REALTIME_H_

#include <cstdint>

namespace naive {	

	struct RealTime {
		int64_t  _time_micros = 0;
		int64_t  _time_nanos = 0;
		int64_t  _time_millis = 0;
	};

	void GetRealTime(RealTime &time);

}

#endif // !_NAIVE_REALTIME_H_