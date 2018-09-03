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
		if (len > GetSpareLen()) {
			return false;
		}
		if (_beg > _end) {
			if (_beg - _end ==1){
				return false;
			}
			_end += len;
			memcpy(_buf + _end, in_buf, len * sizeof(T));
		} else {//_beg <= _end
			uint32_t afterLen = _len - _end;
			if (afterLen > len) {//
				memcpy(_buf + _end, in_buf, len * sizeof(T));
				_end += len;
			} else {
				memcpy(_buf + _end, in_buf, afterLen * sizeof(T));
				uint32_t frontLen = len - afterLen;
				_end = frontLen ;//
				memcpy(_buf, in_buf + afterLen, frontLen * sizeof(T));
			}
		}
		return true;
	}

	T* Pop(uint32_t len) {
		if ((_len -1 - GetSpareLen()) < len) {//
			return nullptr;
		}
		T* tempBuf = new T[len];
		if (_beg > _end) {
			uint32_t afterLen = _len - _beg - 1;
			if (afterLen > len) {//
				memcpy(tempBuf, _buf + _beg, len * sizeof(T));
				_beg += len;
				return tempBuf;
			}
			memcpy(tempBuf, _buf + _beg, afterLen * sizeof(T));
			uint32_t frontLen = len - afterLen;
			memcpy(tempBuf + afterLen, _buf, frontLen * sizeof(T));
			_beg = frontLen;
		} else {
			memcpy(tempBuf, _buf + _beg, len * sizeof(T));
			_beg += len;
		}
		return tempBuf;
	}

	bool Pop(T* in_buf, uint32_t len) {		
		if ((_len - 1 - GetSpareLen()) < len) {//
			return false;
		}
		if (_beg > _end) {
			uint32_t afterLen = _len - _beg;
			if (afterLen > len) {//
				memcpy(in_buf, _buf + _beg, len * sizeof(T));
				_beg += len;
				return true;
			}
			memcpy(in_buf, _buf + _beg, afterLen * sizeof(T));
			uint32_t frontLen = len - afterLen;
			memcpy(in_buf + afterLen, _buf, frontLen * sizeof(T));
			_beg = frontLen;
		}
		else {
			memcpy(in_buf, _buf + _beg, len * sizeof(T));
			_beg += len;
		}
		return true;
	}

	uint32_t TryPop(T* in_buf, uint32_t len) {
		const uint32_t contentLen = (_len - 1 - GetSpareLen());//
		if (contentLen < len) {
			Pop(in_buf, contentLen);
			return contentLen;		
		}
		Pop(in_buf, len);
		return len;
	}

	uint32_t Size() {
		return _len - 1 - GetSpareLen();
	}

	bool PushBack(const T* in_buf, uint32_t len) {
		if (len > GetSpareLen()) {
			return false;
		}
		if (_beg > _end) {
			_beg -= len;
			memcpy(_buf + _beg, in_buf, len * sizeof(T));
		}else {//_beg<=_end
			if (_beg >= len) {
				_beg -= len;
				memcpy(_buf + _beg, in_buf, len * sizeof(T));
			}else {
				uint32_t frontLen = len - _beg;
				memcpy(_buf + (_len - frontLen), in_buf, frontLen * sizeof(T));
				memcpy(_buf, in_buf + _beg, _beg * sizeof(T));//
				_beg = _len - frontLen;
			}
		}
		return true;
	}

	bool PopBack(T* in_buf, uint32_t len) {
		if ((_len - 1 - GetSpareLen()) < len) {//
			return false;
		}
		if (_beg > _end) {
			if (len <= _end) {
				memcpy(in_buf, _buf + (_end - len), len * sizeof(T));
				_end -= len;
				return true;
			}
			uint32_t frontLen = len - _end;
			memcpy(in_buf, _buf + (_len - frontLen), frontLen * sizeof(T));
			memcpy(in_buf + frontLen, _buf, _end * sizeof(T));
			_end = (_len - frontLen);
		}else {
			if (_beg == _end) {
				return false;
			}
			memcpy(in_buf, _buf + (_end - len), len * sizeof(T));//
			_end -= len;
		}
		return true;
	}

	uint32_t TryPopBack(T* in_buf, uint32_t len) {
		const uint32_t contentLen = (_len - 1 - GetSpareLen());//
		if (contentLen < len) {
			PopBack(in_buf, contentLen);
			return contentLen;
		}
		PopBack(in_buf, len);
		return len;
	}

	~RingBuf() {
		SafeDeleteArray(_buf);
	}

private:

	uint32_t _beg;
	uint32_t _end;
	uint32_t _len;
	T*      _buf;

	uint32_t GetSpareLen() const {
		if (_beg < _end){
			return (_len - (_end - _beg) - 1);
		} else if (_beg > _end) {
			return (_beg - _end - 1);
		} else {
			return _len - 1;//
		}
	}
	DISALLOW_COPY_AND_ASSIGN(RingBuf)
};

}

#endif //_ASYNCBUF_H_
