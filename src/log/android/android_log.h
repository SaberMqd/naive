//
// Created by mqd on 2016/11/24.
//
#ifndef _LARK_ANDROID_LOG_H
#define _LARK_ANDROID_LOG_H

#include "larkrtc/base/log.h"
#include "larkrtc/base/base_constructor.h"


namespace lark {

	class AndroidLog : public LarkLog {
	public:

		explicit AndroidLog();
		~AndroidLog();
	private:
		void PlateFormat(std::string levelStr, const char* location, char * _inBuf, char* _outBuf) override;
		void PlatePrint(char tempBuffer[DEBUG_BUFFER_MAX_LENGTH]) override;
		DISALLOW_COPY_AND_ASSIGN(AndroidLog)
	};


};

#endif //_LARK_LOG_H
