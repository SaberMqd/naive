#ifndef _LARK_HRTF_H_
#define _LARK_HRTF_H_

#include <vector>

#include "larkrtc/base/point.h"
#include "larkrtc/base/commone_define.h"

namespace lark {

	class HRTF {

	public:

		static HRTF* Create();
	
		virtual void ClearHistory() = 0;

		virtual void DSP(int16_t* letfChannelData, int16_t* rightChannelData, uint32_t len) = 0;

		virtual void DSP(int16_t* data, uint32_t len) = 0;

		virtual void SetListenerPosition(const Point3d &point) = 0;

		virtual void SetSpeakerPosition(const Point3d &point) = 0;

		virtual void SetFaceVector(const Point3d &point) = 0;

		virtual void SetUpVector(const Point3d &point) = 0;

		virtual commone_define::SampleRate GetHrtfSamplates() const = 0;

		virtual ~HRTF() {}

	protected:

		HRTF() {}

	};
}



#endif // !_LARK_HRTF_H_
