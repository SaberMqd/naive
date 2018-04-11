//
// Created by mqd on 2016/11/24.
//

#include "log.h"
#include "safe_wait.h"
#include "work_thread/work_thread.h"
#include <stdarg.h>

namespace naive {

NaiveLog::NaiveLog() :
	_wtd(nullptr),
	_inFile(nullptr){
	
}

NaiveLog::~NaiveLog() {
	if (_inFile) {
		fclose(_inFile);
		_inFile = nullptr;
	}
}

int NaiveLog::Init(const Config& config) {

	_config.CopyFrom(config);

	if (_config._path != "") {
		if (!(_inFile = fopen(_config._path.c_str(), "wb"))) {
			return -1;
		}
	}

	if (_config._isAsync && !_wtd) {
		auto wid = WPM_CREATE_WORK();
		_wtd = WPM_GET_WORK(wid);
		_wtd->SetMaxTaskNum(64);
		_wtd->Run();
	}
	return 0;
}

void NaiveLog::Print(Level level, const char* location, const char* format, ...) {
	if (level < _config._level) {
		return;
	}

	char buf[DEBUG_BUFFER_MAX_LENGTH]{ 0 };
	char allBuf[DEBUG_BUFFER_MAX_LENGTH]{ 0 };

	va_list arg;
	va_start(arg, format);
	vsnprintf(buf, DEBUG_CONTENT_MAX_LENGTH, format, arg);
	va_end(arg);

	FormatStr(level, location, buf, allBuf);

	if (_config._isAsync) {
		if (!_wtd) {
			return;
		}
		std::unique_ptr<Task> task(new Task());
		task->_str = allBuf;
		task->_log = this;
		_wtd->PostTask(std::move(task));
	}
	else {
		if (_config._path != "") {
			fwrite(allBuf, strlen(allBuf) + 1, 1, _inFile);
		}
		if (_config._cb) {
			_config._cb(allBuf);
		}
		else {
			PlatePrint(allBuf);
		}
	}

}

void NaiveLog::FormatStr(Level level, const char* location, char * _inBuf, char* _outBuf) {

	std::string levelStr;
	switch (level)
	{
	case NaiveLog::NAIVE_INFO:
		levelStr = "NAIVE::INFO: ";
		break;
	case NaiveLog::NAIVE_WARNING:
		levelStr = "NAIVE::WARNING: ";
		break;
	case NaiveLog::NAIVE_ERROR:
		levelStr = "NAIVE::ERROR: ";
		break;
	case NaiveLog::NAIVE_TRACK:
		levelStr = "NAIVE::TRACK: ";
		break;
	case NaiveLog::NAIVE_DEBUG:
		levelStr = "NAIVE::DEBUG: ";
		break;
	default:
		break;
	}

	PlateFormat(levelStr,location, _inBuf, _outBuf);
}

void NaiveLog::Flush() {
	if (_inFile) {
		fflush(_inFile);
	}
}

void NaiveLog::PlatePrint(const char* buf) {
	return;
}
void NaiveLog::PlateFormat(std::string levelStr, const char* location, char * _inBuf, char* _outBuf) {
	return;
}

}
