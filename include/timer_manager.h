//
// Created by mqd on 2017/08/22.
//

#ifndef _LARK_TIMER_MANAGER_H_
#define _LARK_TIMER_MANAGER_H_

#include "larkrtc/base/base_constructor.h"
#include "larkrtc/base/timer.h"

#include <cstdint>
#include <map>
#include <mutex>

namespace lark {

#define LARK_TIMER_NEW(x,y,z) lark::TimerManager::GetInstance()->NewTimer(x,y,z);
#define LARK_TIMER_STOP(x) lark::TimerManager::GetInstance()->StopTimer(x);

	class TimerManager
	{
	public:

		static TimerManager* GetInstance();

		uint32_t NewTimer(Timer::TimerFunc func, int32_t millionseconds = 0, Timer::Mode mode = Timer::Mode::TICK);
		int32_t StopTimer(uint32_t id);

		~TimerManager();

	private:

		static TimerManager* _instance;

		std::map<uint32_t, Timer*> _timerMap;
		std::mutex _mutex;

		uint32_t _timerId;
		TimerManager();
		uint32_t GetTimerId();

		DISALLOW_COPY_AND_ASSIGN(TimerManager);
	};

}

#endif

