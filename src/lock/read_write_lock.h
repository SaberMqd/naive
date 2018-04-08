#ifndef _NAIVE_READ_WRITE_LOCK_H_
#define _NAIVE_READ_WRITE_LOCK_H_

#include <mutex>
#include <atomic>
#include <cstdint>

#include "../base_constructor.h"

namespace naive {

	class ReadWriteLock
	{
	public:
		
		explicit ReadWriteLock():_pv(0), _tryWrite(false){}

		void ReadLock() {
			if (_tryWrite) {
				_mtx.lock();
				_mtx.unlock();
			} else {
				_pv++;
			}
		}

		void ReadUnlock() {
			_pv--;
		}

		void WriteLock(){
			_tryWrite = true;
			while (_pv != 0){
			}
			_mtx.lock();
		}

		void WriteUnlock() {
			_tryWrite = false;
			_mtx.unlock();
		}

		~ReadWriteLock() {}

	private:

		std::mutex _mtx;
		std::atomic<uint32_t> _pv;
		std::atomic<bool>	  _tryWrite;

		DISALLOW_COPY_AND_ASSIGN(ReadWriteLock)

	};

}

#endif // !_NAIVE_READ_WRITE_LOCK_H_
