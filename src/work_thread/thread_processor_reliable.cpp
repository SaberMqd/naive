#include "thread_processor_reliable.h"
#include "../safe_delete.h"

namespace naive {

ThreadProcessor* ThreadProcessor::Create() {
	return new ThreadProcessorReliable();
}

ThreadProcessorReliable::ThreadProcessorReliable():
	_thread(nullptr),
	_func(nullptr),
	_threadOver(false),
	_shouldStop(false),
	_isRuning(false),
	_isBusy(false),
	_inited(false){
}

ThreadProcessorReliable::~ThreadProcessorReliable(){
	Stop();
	SafeDelete(_thread);
}

void ThreadProcessorReliable::Run(std::function<bool()> func) {
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

void ThreadProcessorReliable::Notify() {
	_scv.Notify();
}

void ThreadProcessorReliable::Stop() {
	std::unique_lock<std::mutex> lg(_mtx);
	if (!_isRuning) {
		return;
	}
	_shouldStop = true;
	_scv.Notify();
	_endScv.Wait();
	_isRuning = false;
}

void ThreadProcessorReliable::SetID(const std::string &id) {
	_id = id;
}

const std::string& ThreadProcessorReliable::GetID() const {
	return _id;
}

}