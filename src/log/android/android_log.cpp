//
// Created by mqd on 2016/11/24.
//
#include <android/log.h>
#include <larkrtc/base/log.h>
#include "../include/android_log.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_ERROR,"LARKRTC", __VA_ARGS__)

namespace lark {

	LarkLog* LarkLog::GetInstance() {
		static AndroidLog l;
		return &l;
	}

	AndroidLog::AndroidLog() {
    }
	AndroidLog::~AndroidLog() {
	}

	void AndroidLog::PlateFormat(std::string levelStr,const char* location, char * _inBuf, char* _outBuf) {
		sprintf(_outBuf, "%s%s%s", levelStr.c_str(), location, _inBuf);
	}

	void AndroidLog::PlatePrint(char tempBuffer[DEBUG_BUFFER_MAX_LENGTH]) {
		LOGD("%s",tempBuffer);
	}

};




