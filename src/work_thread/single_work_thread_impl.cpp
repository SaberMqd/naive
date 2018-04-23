#include "single_work_thread_impl.h"
#include "naive/safe_delete.h"

namespace naive {

SingleWorkThread* SingleWorkThread::Create() {
	return new SingleWorkThreadImpl();
}

SingleWorkThreadImpl::SingleWorkThreadImpl():
	_thread(nullptr),
	_func(nullptr),
	_threadOver(false),
	_shouldStop(false),
	_isRuning(false),
	_isBusy(false),
	_inited(false){
}

SingleWorkThreadImpl::~SingleWorkThreadImpl(){
	Stop();
	SafeDelete(_thread);
}

void SingleWorkThreadImpl::Run(std::function<bool()> func) {
	std::unique_lock<std::mutex> lg(_mtx);
	if (_inited) {
		return;
	}
	_inited = true;
	_func = func;
	_isRuning = true;
	_thread = new std::thread([this]() {
		while (true) {
			_scv.Wait();
			if (_shouldStop) {
				break;
			}
			if (!_func()) {
				break;
			}
		}
		_endScv.Notify();
	});
	_thread->detach();
}

void SingleWorkThreadImpl::Notify() {
	_scv.Notify();
}

void SingleWorkThreadImpl::Stop() {
	std::unique_lock<std::mutex> lg(_mtx);
	if (!_isRuning) {
		return;
	}
	_shouldStop = true;
	_scv.Notify();
	_endScv.Wait();
	_isRuning = false;
}

void SingleWorkThreadImpl::SetID(const std::string &id) {
	_id = id;
}

const std::string& SingleWorkThreadImpl::GetID() const {
	return _id;
}

}