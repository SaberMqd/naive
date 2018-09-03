#ifndef _NAIVE_SHARE_LOCK_H_
#define _NAIVE_SHARE_LOCK_H_

#ifdef _WIN32
#include <shared_mutex>
#else
#include <mutex>
#include <condition_variable>
#include <atomic>
#endif

namespace naive {

#ifdef _WIN32
	class ShareLock
	{
	public:
		ShareLock() {

		}

		void Lock() {
			_mtx.lock();
		}

		void UnLock() {
			_mtx.unlock();
		}

		void LockShared() {
			_mtx.lock_shared();
		}

		void UnLockShared() {
			_mtx.unlock_shared();
		}

		~ShareLock() {

		}

	private:
		std::shared_mutex _mtx;
	};
#else
	class ShareLock
	{
	public:
		ShareLock() : _pv(0), _writing(false), _hasWriting(false){ }
		
		void LockShared() {
			std::unique_lock<std::mutex> lock(_mtx);
			while (_writing == true) {
				_cv.wait(lock);
			}	
			++_pv;
		}

		void UnLockShared() {
			std::unique_lock<std::mutex> lock(_mtx);
			--_pv;			
			_cv.notify_one();
		}

		void Lock() {			
			std::unique_lock<std::mutex> lock(_mtx);
			_hasWriting = true;
			while ( _pv != 0 || _writing == true) {	
				_cv.wait(lock);
			}
			_writing = true;		
		}

		void UnLock() {
			std::unique_lock<std::mutex> lock(_mtx);
			_writing = false;
			_hasWriting = false;
			_cv.notify_all();
		}

		~ShareLock() { }

	private:
		std::mutex _mtx;
		std::condition_variable _cv;	
		std::atomic<int> _pv ; 
		bool _writing ;		 
		bool _hasWriting;
};

#endif

}

#endif // !_NAIVE_SHARE_LOCK_H_
