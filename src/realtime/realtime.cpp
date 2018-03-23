#include "realtime.h"
#include "../base_sys_define.h"

/*
	cross platform standard C/C++ time() and clock()	Exact to the second
	win api: GetTickCount and timeGetTime				Exact to the millisecond
	win api: QueryPerformanceCounter()					Exact to the Microsecond
	linux api: clock_gettime()							Exact to the nanosecond
	macos api: mach_timebase_info()						Exact to the nanosecond
*/

#ifdef NAIVE_WIN64
	#include <windows.h>
	#include <mmsystem.h>
	#include <sys/timeb.h>
	#pragma comment( lib,"winmm.lib" )
#endif

#ifdef NAIVE_ANDROID

#include <sys/time.h>
#include <time.h>

#endif

#ifdef NAIVE_IOS
	#include <mach/mach_time.h>
#endif

static const int64_t kNumMillisecsPerSec = INT64_C(1000);
static const int64_t kNumMicrosecsPerSec = INT64_C(1000000);
static const int64_t kNumNanosecsPerSec = INT64_C(1000000000);

static const int64_t kNumMicrosecsPerMillisec =
kNumMicrosecsPerSec / kNumMillisecsPerSec;
static const int64_t kNumNanosecsPerMillisec =
kNumNanosecsPerSec / kNumMillisecsPerSec;
static const int64_t kNumNanosecsPerMicrosec =
kNumNanosecsPerSec / kNumMicrosecsPerSec;

namespace naive {

	void GetRealTime(RealTime &time) {
		int64_t ticks;
#ifdef NAIVE_ANDROID
		struct timespec ts;
		// TODO(deadbeef): Do we need to handle the case when CLOCK_MONOTONIC is not
		// supported?
		clock_gettime(CLOCK_MONOTONIC, &ts);
		ticks = kNumNanosecsPerSec * static_cast<int64_t>(ts.tv_sec) +
			static_cast<int64_t>(ts.tv_nsec);
#endif
#ifdef NAIVE_IOS
		static mach_timebase_info_data_t timebase;
		if (timebase.denom == 0) {
			// Get the timebase if this is the first time we run.
			// Recommended by Apple's QA1398.
			if (mach_timebase_info(&timebase) != KERN_SUCCESS) {
				RTC_NOTREACHED();
			}
		}
		// Use timebase to convert absolute time tick units into nanoseconds.
		ticks = mach_absolute_time() * timebase.numer / timebase.denom;
#endif
#ifdef NAIVE_WIN64
		static volatile LONG last_timegettime = 0;
		static volatile int64_t num_wrap_timegettime = 0;
		volatile LONG* last_timegettime_ptr = &last_timegettime;
		DWORD now = timeGetTime();
		// Atomically update the last gotten time
		DWORD old = InterlockedExchange(last_timegettime_ptr, now);
		if (now < old) {
			// If now is earlier than old, there may have been a race between threads.
			// 0x0fffffff ~3.1 days, the code will not take that long to execute
			// so it must have been a wrap around.
			if (old > 0xf0000000 && now < 0x0fffffff) {
				num_wrap_timegettime++;
			}
		}
		ticks = now + (num_wrap_timegettime << 32);
		// TODO(deadbeef): Calculate with nanosecond precision. Otherwise, we're
		// just wasting a multiply and divide when doing Time() on Windows.
		ticks = ticks * kNumNanosecsPerMillisec;
#endif

		time._time_nanos = ticks;
		time._time_millis = time._time_nanos / kNumNanosecsPerMillisec;
		time._time_micros = time._time_nanos / kNumNanosecsPerMicrosec;
	}
	
}