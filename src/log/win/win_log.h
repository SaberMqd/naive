//
// Created by mqd on 2016/11/24.
//

#ifndef _NAIVE_WIN_LOG_H_
#define _NAIVE_WIN_LOG_H_

#include "log.h"
#include "base_constructor.h"

namespace naive {

class WinLog : public NaiveLog {

public:
		
	explicit WinLog();
		
	~WinLog();
	
private:
		
	void PlateFormat(std::string levelStr, const char* location, char * _inBuf, char* _outBuf) override;
		
	void PlatePrint(const char* buf) override;
		
	DISALLOW_COPY_AND_ASSIGN(WinLog)
};

};

#endif //_NAIVE_WIN_LOG_H_
