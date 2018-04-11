#ifndef _LARK_BASE_MIX_H_
#define _LARK_BASE_MIX_H_

#include <cstdint>

#include "larkrtc/base/commone_define.h"

namespace lark{
namespace base{

	template<class T>
	int NormalizationMix(T **source, uint32_t channel, uint32_t num_samples, T *out) {
		if (source == NULL) {
			return -1;
		}

		constexpr int MAX = 32767;
		constexpr int MIN = -32768;

		double f = 1;
		int output = 0;
		int i = 0;
		int j = 0;

		for (i = 0; i < num_samples; i++) {

			int temp = 0;
			for (j = 0; j < channel; j++) {
				temp += source[j][i];
			}

			output = (int)(temp * f);
			if (output > MAX) {
				f = (double)MAX / (double)(output);
				output = MAX;
			}
			if (output < MIN) {
				f = (double)MIN / (double)(output);
				output = MIN;
			}
			if (f < 1) {
				f += ((double)1 - f) / (double)32;
			}
			out[i] = output;
		}  
		return 0;
	}

	template<class T>
	int NormalizationMixDouble(T **source, uint32_t channel, uint32_t num_samples, T *out) {

		int channnelNum = channel + 1;

		T **tmpBufLeft = new T*[channnelNum*2];
		for (int i = 0; i < channnelNum*2; ++i) {
			tmpBufLeft[i] = new T[num_samples];
		}

		T **tmpBufRight = new T*[channnelNum*2];
		for (int i = 0; i < channnelNum*2; ++i) {
			tmpBufRight[i] = new T[num_samples];
		}

		for (int i = 0; i < num_samples; ++i) {
			for (int q = 0; q < channel * 2; ++q) {
				tmpBufLeft[q][i] = source[q /2][i * 2];
				tmpBufRight[q][i] = source[q /2][i * 2 + 1];
			}
		}

		NormalizationMix(tmpBufLeft, channel, num_samples, tmpBufLeft[channel]);
		NormalizationMix(tmpBufRight, channel, num_samples, tmpBufRight[channel]);

		int j = 0;
		int k = 0;
		for (int i = 0; i < num_samples*2; ++i) {
			if (i % 2 == 0) {
				out[i] = tmpBufLeft[channel][j++];
			} else {
				out[i] = tmpBufRight[channel][k++];
			}
		}

		return 0;
	}

	template<class T>
	int LinearAverageMix(T **source, uint32_t channel, uint32_t per_channel_samples, T *out) {

		int64_t* tmpBuf = new int64_t[per_channel_samples]{0};

		if (!source || !out || 0 == channel) {
			return -1;
		}

		for (int i = 0; i < per_channel_samples; ++i) {
			for (int j = 0; j < channel; ++j) {
				tmpBuf[i] += source[j][i];
			}
		}

		for (int i = 0; i < per_channel_samples; ++i) {
			out[i] = tmpBuf[i] / channel;
		}

		delete[] tmpBuf;

		return 0;
	}

};
};

#endif //_LARK_BASE_MIX_H_

