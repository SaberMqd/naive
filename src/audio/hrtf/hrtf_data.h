#ifndef _LARK_HRTF_DATA_H_
#define _LARK_HRTF_DATA_H_

#include "larkrtc/base/base_constructor.h"
#include <string>
#include <vector>

namespace lark {

	class HrtfData
	{
	public:

		virtual int32_t GetHrtfDataLen(uint32_t samplerate) const = 0;

		virtual int GetHrtfData(int32_t azimuth, int32_t elevation, uint32_t samplerate, const int16_t* &left, const int16_t* &right) const = 0;

		virtual bool IsVaild(int32_t azimuth, int32_t elevation, int32_t samplata) const = 0;

		virtual ~HrtfData() {}

	protected:

		HrtfData() {}

	};
}

#endif // !_LARK_HRTF_DATA_H_
