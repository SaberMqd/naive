#ifndef _HRTF_DATA_FACTORY_H_
#define _HRTF_DATA_FACTORY_H_

#include "larkrtc/base/base_constructor.h"

namespace lark {

class HrtfData;

class HrtfDataFactory
{
public:
	
	enum Type {

		MIT_HRTF = 0,
		UNKNOW_HRTF = 1,
	};

	static HrtfData* GetHrtfDataInstance(Type t);

	~HrtfDataFactory();

private:

	HrtfDataFactory();

	DISALLOW_COPY_AND_ASSIGN(HrtfDataFactory);

};

}

#endif // !_HRTF_DATA_FACTORY_H_
