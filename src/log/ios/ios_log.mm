//
// Created by mqd on 2016/11/24.
//

#include "../include/ios_log.h"
#import <Foundation/Foundation.h>

namespace lark {

    LarkLog* LarkLog::GetInstance(){
        static IOSLog l;
        return &l;
    }
    
    IOSLog::IOSLog(){
        
    };
 
    IOSLog::~IOSLog(){
        
    };
  
    void IOSLog::PlatePrint(char tempBuffer[DEBUG_BUFFER_MAX_LENGTH]) {
        NSLog(@"IOSLOG:%s",tempBuffer);
    }
    
    void IOSLog::PlateFormat(std::string levelStr,const char* location, char * _inBuf, char* _outBuf){
        sprintf(_outBuf, "%s%s%s", levelStr.c_str(), location, _inBuf);
    }
};



