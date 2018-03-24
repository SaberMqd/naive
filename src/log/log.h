//
// Created by mqd on 2016/11/24.
//

#ifndef _NAIVE_LOG_H_
#define _NAIVE_LOG_H_

#include <cstdint>
#include <string>
#include <mutex>
#include <functional>

#include "../base_constructor.h"
#include "../ring_buffer.h"
#include "../work_thread/work_thread.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__) " "
#define TRACE_CMH_2(fmt,...) printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

namespace naive {

#define NAIVE_LOG(LEVEL,FORAMT,...) 		NaiveLog::GetInstance()->Print(LEVEL, AT,		FORAMT,	__VA_ARGS__)
#define NAIVE_LOG_INFO_D(FORAMT,...) 		NAIVE_LOG(NaiveLog::Level::NAIVE_INFO,		FORAMT,	__VA_ARGS__)
#define NAIVE_LOG_DEBUG_D(FORAMT, ...)		NAIVE_LOG(NaiveLog::Level::NAIVE_DEBUG,		FORAMT, __VA_ARGS__)
#define NAIVE_LOG_WARNING_D(FORAMT, ...)	NAIVE_LOG(NaiveLog::Level::NAIVE_WARNING,	FORAMT, __VA_ARGS__)
#define NAIVE_LOG_ERROR_D(FORAMT, ...)		NAIVE_LOG(NaiveLog::Level::NAIVE_ERROR,		FORAMT, __VA_ARGS__)
#define NAIVE_LOG_TRACK_D(FORAMT, ...)		NAIVE_LOG(NaiveLog::Level::NAIVE_TRACK,		FORAMT, __VA_ARGS__)

#define NAIVE_LOG_INFO(...)					NAIVE_LOG_INFO_D(__VA_ARGS__,	"")
#define NAIVE_LOG_DEBUG(...)				NAIVE_LOG_DEBUG_D(__VA_ARGS__,	"")
#define NAIVE_LOG_WARNING(...)				NAIVE_LOG_WARNING_D(__VA_ARGS__,	"")
#define NAIVE_LOG_ERROR(...)				NAIVE_LOG_ERROR_D(__VA_ARGS__,	"")
#define NAIVE_LOG_TRACK(...)				NAIVE_LOG_TRACK_D(__VA_ARGS__,	"")

#define NI NAIVE_LOG_INFO
#define ND NAIVE_LOG_DEBUG
#define NW NAIVE_LOG_WARNING
#define NE NAIVE_LOG_ERROR
#define NT NAIVE_LOG_TRACK

class SafeConditionVariable;
class ThreadProcessor;

constexpr int DEBUG_CONTENT_MAX_LENGTH = 256;
constexpr int DEBUG_BUFFER_MAX_LENGTH = 512;

class NaiveLog {

public:

	typedef std::function<void(const char* msg)> NaiveLogCB;

	enum Level {
		NAIVE_TRACK	= 0,
		NAIVE_INFO,
		NAIVE_DEBUG,
		NAIVE_WARNING,
		NAIVE_ERROR,
	};

	enum Mode {
		WRITE_FILE = 0x00000001,
		ASYNC = 0x00000010,
		EXTERN_RPINT = 0x00000100,
	};

	struct Config{

		std::string		_path		= "";
		NaiveLogCB		_cb			= nullptr;
		Level			_level		= NAIVE_ERROR;
		bool			_isAsync	= false;

		void CopyFrom(const Config& cfg) {
			_path = cfg._path;
			_cb = cfg._cb;
			_level = cfg._level;
			_isAsync = cfg._isAsync;
		}

	};

	static NaiveLog* GetInstance();
	int Init(const Config& config);
	void Print(Level level, const char* location, const char* format, ...);
	void Flush();
	virtual ~NaiveLog();

private:

	FILE 				*_inFile;
	Config 		 		_config;
	WorkThread			*_wtd;

	void FormatStr(Level level, const char* location, char * _inBuf, char* _outBuf);
	virtual void PlatePrint(const char* buf);
	virtual void PlateFormat(std::string levelStr,const char* location, char * _inBuf, char* _outBuf);

	class Task : public WorkTask {
		friend NaiveLog;
		bool Process() override {
			if (_log) {
				_log->PlatePrint(_str.c_str());
			}
			return true;
		}
		std::string _str = "";
		NaiveLog *_log = nullptr;
	};

protected:

	explicit NaiveLog();

};

};



#endif //_NAIVE_LOG_H_
