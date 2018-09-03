//
// Created by mqd on 2016/12/6.
//

#ifndef _NAIVE_RING_OBJECT_BUF_H_
#define _NAIVE_RING_OBJECT_BUF_H_

#include <cstdint>
#include <memory>

#include "safe_delete.h"
#include "base_constructor.h"

namespace naive {

template <typename T>
class RingObjBuf {

public:
		
	explicit RingObjBuf(uint32_t len):
		_len(len+1),_beg(0),_end(0) {
		_buf = new std::unique_ptr<T>[_len];
	}

	bool Push(std::unique_ptr<T> data) {
		if ((_end + 1) % _len == _beg || !data.get()) {
			return false;
		}

		_buf[_end] = std::move(data);
		_end += 1;
		_end %= _len;

		return true;
	}

	bool Pop(std::unique_ptr<T>& data) {
		if (_beg  == _end) {
			return false;
		}
		data = std::move(_buf[_beg]);
		_beg += 1;
		_beg %= _len;
		return true;
	}

	T* Front() {
		if (_beg == _end) {
			return nullptr;
		}
		return _buf[_beg].get();
	}

	void Reset() {
		for (uint32_t i = 0; i < _len; ++i) {
			_buf[i].reset(nullptr);
		}
		_beg = 0;
		_end = 0;
	}

	uint32_t Size() {
		if (_end >= _beg) {
			return _end - _beg;
		} else {
			return _len - 1 - (_beg - _end);
		}
	}

	~RingObjBuf() {
		SafeDeleteArray(_buf);
	}

private:

	uint32_t				_beg;
	uint32_t				_end;
	uint32_t				_len;

	std::unique_ptr<T>		*_buf;

	DISALLOW_COPY_AND_ASSIGN(RingObjBuf)
};

}

#endif //_NAIVE_RING_OBJECT_BUF_H_
