#include "Runnable.h"
#include "TestThread.h"


bool TestThread::Run()
{
	////// TEST
	uint32_t after = rand() % 2000;

	if ( after > 1000)
	{
		GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsync(&TestObject::TestFunc0);
		GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsync(&TestObject::TestFunc2, double(GetThreadId()) * 100, LWorkerThreadId);
		GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsync(&TestObject::TestFunc1, 100);

		GTestObject[rand() % TEST_OBJECT_COUNT]->DoAsyncAfter(after, &TestObject::TestFuncForTimer, (int)after);
	}

	/// 종료조건 테스트
	if (GTestObject[rand() % TEST_OBJECT_COUNT]->GetTestCount() > 10000)
		return false;

	return true;
}


void TestObject::TestFunc2(double a, int b)
{
	++mTestCount;


	if ( mTestCount < 1000)
		DoAsync(&TestObject::TestFunc2, double(LWorkerThreadId) * 100, -33);
}

void TestObject::TestFuncForTimer(int b)
{
	//printf("TestFuncForTimer [%d] \n", b);

	if (rand() % 2 == 0)
		DoAsyncAfter(1000, &TestObject::TestFuncForTimer, (int)-1599);
}