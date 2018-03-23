//
// Created by mqd on 2016/12/6.
//

#ifndef _NAIVE_RINGBUF_H_
#define _NAIVE_RINGBUF_H_

#include <cstring>
#include <cstdint>

#include "safe_delete.h"
#include "base_constructor.h"

namespace naive {

template <typename T>
class RingBuf {

public:

	explicit RingBuf(uint32_t len):
		_len(len+1),_beg(0),_end(0) {
		_buf = new T[_len]{ 0 };
	}

	void Reset() {
		_beg = 0;
		_end = 0;
		memset(_buf, 0, _len * sizeof(T));
	}

	bool Push(const T* in_buf, uint32_t len) {
		if (((_end + 1 + len) % _len == _beg) || len > _len) {
			return false;
		}
		if (_len - _end >= len) {
			memcpy(_buf + _end, in_buf, len * sizeof(T));
			_end += len;
			_end %= _len;
		} else {
			uint32_t afterLen = _len - _end;
			memcpy(_buf + _end, in_buf, afterLen * sizeof(T));
			uint32_t frontLen = len - afterLen;
			memcpy(_buf, in_buf + afterLen, frontLen * sizeof(T));
			_end = frontLen;
		}
		return true;
	}

	T* Pop(uint32_t len) {
		if ((_beg + len) % _len == _end || len > _len) {
			return nullptr;
		}
		T* tempBuf = new T[len];
		if (_end > _beg){
			memcpy(tempBuf, _buf + _beg, len * sizeof(T));
			_beg += len;
		} else {
			uint32_t afterLen = _len - _beg;
			if (afterLen >= len) {
				memcpy(tempBuf, _buf + _beg, len * sizeof(T));
				_beg += len;
				_beg %= _len;
			} else {
				memcpy(tempBuf, _buf + _beg, afterLen * sizeof(T));
				uint32_t frontLen = len - afterLen;
				memcpy(tempBuf + afterLen, _buf, frontLen * sizeof(T));
				_beg = frontLen;
			}
		}
		return tempBuf;
	}

	bool Pop(T* in_buf, uint32_t len) {
		if (GetContentLen() < len || len > _len){
			return false;
		}
		if (_end > _beg){
			memcpy(in_buf, _buf + _beg, len * sizeof(T));
			_beg += len;
		} else {
			uint32_t afterLen = _len - _beg;
			if (afterLen >= len) {
				memcpy(in_buf, _buf + _beg, len * sizeof(T));
				_beg += len;
				_beg %= _len;
			} else {
				memcpy(in_buf, _buf + _beg, afterLen * sizeof(T));
				uint32_t frontLen = len - afterLen;
				memcpy(in_buf + afterLen, _buf, frontLen * sizeof(T));
				_beg = frontLen;
			}
		}
		return true;
	}

	uint32_t TryPop(T* in_buf, uint32_t len) {
		const uint32_t contentLen = GetContentLen();
		if (contentLen < len) {
			Pop(in_buf, contentLen);
			return contentLen;		
		} 
		Pop(in_buf, len);
		return len;
	}

	bool Front(uint32_t len) {
		if ((_beg + len) % _len == _end || len > _len){
			return false;
		}
		if (_end > _beg){
			_beg += len;
		}
		else {
			uint32_t afterLen = _len - _beg;
			if (afterLen >= len) {
				_beg += len;
				_beg %= _len;
			}
			else {
				uint32_t frontLen = len - afterLen;
				_beg = frontLen;
			}
		}
		return true;
	}

	~RingBuf() {
		SafeDeleteArray(_buf);
	}

private:

	uint32_t _beg;
	uint32_t _end;
	uint32_t _len;
	T*      _buf;

	uint32_t GetContentLen() const {
		if (_beg < _end){
			return (_end - _beg);
		} else if (_beg > _end) {
			return (_len - (_beg - _end));
		}
		return 0;
	}
	DISALLOW_COPY_AND_ASSIGN(RingBuf)
};

}

#endif //_ASYNCBUF_H_
