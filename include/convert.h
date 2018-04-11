#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <cstdint>
#include <cmath>
#include <iostream>

namespace lark {
namespace convert {
void Byte2HexStr(const uint8_t* data, uint32_t len, char* str);

template<class T>
int MonoNormalization(const T **source, uint32_t channel, uint32_t num_samples, T *out, double& f);

template<class T>
int StereoNormalizationPlanar(const T** data, uint32_t channel, uint32_t num_samples, T* out, double& left_f, double& right_f);

template<class T>
int StereoNormalizationNotPlanar(const T** data, uint32_t channel, uint32_t num_samples, T* out, double& left_f, double& right_f);

template<class T>
int LinearAverage(const T** data, uint32_t channel, uint32_t num_samples, T* out);

/*-------------------------------------------------------------------------归一化-------------------------------------------------------------------------------------*/

//每个采样点的混音
template<class T>
void MixRerSample(const T** data, uint32_t channel, int num_sample, T* out, double& f, double MAX, double MIN) {
	double temp = 0;
	uint32_t j = 0;

	for (j = 0; j < channel; j++) {
		temp += data[j][num_sample];
	}

	double output = temp * f;
	if (output > MAX) {
		f = MAX / output;
		output = MAX;
	}
	if (output < MIN) {
		f = MIN / output;
		output = MIN;
	}
	if (f < 1) {
		f += (1 - f) / 3200;
	}
	out[num_sample] = (T)output;
}

//单声道数据处理
template<class T>
int MonoNormalization(const T **data, uint32_t channel, uint32_t num_samples, T *out, double& f) {
	if (data == NULL) {
		return -1;
	}

	double max = pow(2, sizeof(T) * 8);
	double MAX = (max / 2) - 1;
	double MIN = -(max / 2);

	uint32_t num_sample = 0;
	for (num_sample = 0; num_sample < num_samples; num_sample++) {
		MixRerSample(data, channel, num_sample, out, f, MAX, MIN);
	}

	return 0;
}

template<>
inline int MonoNormalization<float>(const float **data, uint32_t channel, uint32_t num_samples, float *out, double& f) {
	if (data == NULL) {
		return -1;
	}

	uint32_t num_sample = 0;

	for (num_sample = 0; num_sample < num_samples; num_sample++) {
		MixRerSample(data, channel, num_sample, out, f, 1, -1);
	}
	return 0;
}

//双声道非平面数据处理           数据格式：L-R-L-R-L-R-L-R-L-R-L-R
template<class T>
int StereoNormalizationPlanar(const T** data, uint32_t channel, uint32_t num_samples, T* out, double& left_f, double& right_f) {
	if (data == NULL) {
		return -1;
	}

	int64_t max = pow(2, sizeof(T) * 8);
	int64_t MAX = (max / 2) - 1;
	int64_t MIN = -(max / 2);

	uint32_t num_sample = 0;
	for (num_sample = 0; num_sample < num_samples; num_sample++) {
		if (num_sample % 2 == 0) {
			MixRerSample(data, channel, num_sample, out, left_f, MAX, MIN);
		}
		else {
			MixRerSample(data, channel, num_sample, out, right_f, MAX, MIN);
		}
	}

	return 0;
}

template<>
inline int StereoNormalizationPlanar<float>(const float** data, uint32_t channel, uint32_t num_samples, float* out, double& left_f, double& right_f) {
	if (data == NULL) {
		return -1;
	}

	uint32_t num_sample = 0;
	for (num_sample = 0; num_sample < num_samples; num_sample++) {
		if (num_sample % 2 == 0) {
			MixRerSample(data, channel, num_sample, out, left_f, 1, -1);
		}
		else {
			MixRerSample(data, channel, num_sample, out, right_f, 1, -1);
		}
	}

	return 0;
}

//双声道平面数据处理         数据格式：L-L-L-L-L-L-R-R-R-R-R-R
template<class T>
int StereoNormalizationNotPlanar(const T** data, uint32_t channel, uint32_t num_samples, T* out, double& left_f, double& right_f) {
	if (data == NULL) {
		return -1;
	}

	int64_t max = pow(2, sizeof(T) * 8);
	int64_t MAX = (max / 2) - 1;
	int64_t MIN = -(max / 2);

	uint32_t num_sample = 0;
	for (num_sample = 0; num_sample < num_samples / 2; num_sample++) {
		MixRerSample(data, channel, num_sample, out, left_f, MAX, MIN);
		MixRerSample(data, channel, num_sample + num_samples / 2, out, right_f, MAX, MIN);
	}
	return 0;
}

template<>
inline int StereoNormalizationNotPlanar<float>(const float** data, uint32_t channel, uint32_t num_samples, float* out, double& left_f, double& right_f) {
	if (data == NULL) {
		return -1;
	}
	uint32_t num_sample = 0;
	for (num_sample = 0; num_sample < num_samples / 2; num_sample++) {
		MixRerSample(data, channel, num_sample, out, left_f, 1, -1);
		MixRerSample(data, channel, num_sample + num_samples / 2, out, right_f, 1, -1);
	}

	return 0;
}

/*-----------------------------------------------------------------------线性平均----------------------------------------------------------------------------------------*/
template<class T>
int LinearAverage(const T** data, uint32_t channel, uint32_t num_samples, T* out) {
	double temp = 0;

	if (!data || !out || 0 == channel) {
		return -1;
	}

	for (uint32_t i = 0; i < num_samples; ++i) {
		for (uint32_t j = 0; j < channel; ++j) {
			temp += data[j][i];
		}
		out[i] = (T)(temp / channel);
	}
	return 0;
}

}
}
#endif // !_LARK_CONVERT_H_
