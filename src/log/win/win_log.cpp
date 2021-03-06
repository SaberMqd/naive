//
// Created by mqd on 2016/11/24.
//

#include "win_log.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <windows.h>

namespace naive {

NaiveLog* NaiveLog::GetInstance() {
	static WinLog l;
	return &l;
}

WinLog::~WinLog() {
}

WinLog::WinLog(){
}

void WinLog::PlateFormat(std::string levelStr,const char* location, char * _inBuf, char* _outBuf) {
	char tempbuf[64];
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf(tempbuf, " %4d/%02d/%02d %02d:%02d:%02d.%03d \n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	sprintf(_outBuf, "%s%s%s%s", levelStr.c_str(), location, tempbuf, _inBuf);
}

void WinLog::PlatePrint(const char* buf) {
	std::cout << buf << std::endl;
	std::cout.flush();
}

};



