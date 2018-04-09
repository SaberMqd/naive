#ifndef _NAIVE_TEST_H_
#define _NAIVE_TEST_H_

#include "../src/log/log.h"
#include "../src/work_thread/work_thread.h"
#include "../src/scoped_ref_ptr.h"

using namespace naive;

//test log
void TestSyncLog();
void TestAsyncLog();

//test ptr

void TestScopeRef();

//test workthread

void TestWorkThread();


void TestWorkProcessorPool();
//test coroutine
//void TestCoroutine();


#endif // !_NAIVE_TEST_H_
