#include "thread_processor_block.h"
#include "../safe_delete.h"

namespace naive {

BlcokThreadProcessor* BlcokThreadProcessor::Create() {
	return new BlockThreadProcessorImpl();
}

BlockThreadProcessorImpl::BlockThreadProcessorImpl():
	_thread(nullptr),
	_func(nullptr),
	_interrFunc(nullptr),
	_inited(false){
}

BlockThreadProcessorImpl::~BlockThreadProcessorImpl(){
	Stop();
	_endScv.Wait();
	SafeDelete(_thread);
}

void BlockThreadProcessorImpl::RegisterReadFunc(ReadFunc func) {
	_func = func;
}

void BlockThreadProcessorImpl::RegisterInterruptFunc(InterruptFunc func) {
	_interrFunc = func;
}

void BlockThreadProcessorImpl::Run() {
	if (_inited) {
		return;
	}
	_inited = true;
	_thread = new std::thread([this]() {
		while (true) {
			if (!_func()) {
				break;
			}
		}
		_endScv.Notify();
	});
	_thread->detach();
}

void BlockThreadProcessorImpl::Stop() {
	if (!_inited) {
		return;
	}
	if (_interrFunc) {
		_interrFunc();
	}
}

}