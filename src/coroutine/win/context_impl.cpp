#include "context_impl.h"

#include <Windows.h>

namespace naive {

	ContextImpl::ContextImpl(){
	}

	ContextImpl::~ContextImpl(){
	}

	Context* Context::Create() {
		return new ContextImpl();
	}

}