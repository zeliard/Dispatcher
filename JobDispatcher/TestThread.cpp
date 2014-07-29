#include "Runnable.h"
#include "TestThread.h"


bool TestThread::Run()
{
	static int i = 0;

	////// TEST
	GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsync(&TestObject::TestFunc0);
	GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsync(&TestObject::TestFunc2, double(GetThreadId()) * 100, i);
	GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsync(&TestObject::TestFunc1, 100);


	if (i++ < 30)
	{
		uint32_t after = rand() % 2000;
		GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsyncAfter(after, &TestObject::TestFuncForTimer, (int)after);
	}

	return true;
}