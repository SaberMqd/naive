//
// Created by mqd on 2016/11/24.
//

#ifndef _IOS_LOG_H
#define _IOS_LOG_H
#include "larkrtc/base/log.h"
#include "larkrtc/base/base_constructor.h"
#include "larkrtc/base/async_buf.h"

namespace lark {
    
    class IOSLog: public LarkLog{
    public:
        
        explicit IOSLog();
        ~IOSLog();
        
    private:
        void PlatePrint(char tempBuffer[DEBUG_BUFFER_MAX_LENGTH]) override;
        void PlateFormat(std::string levelStr,const char* location, char * _inBuf, char* _outBuf)override;
        DISALLOW_COPY_AND_ASSIGN(IOSLog)
    };
};



#endif //_LARK_LOG_H
