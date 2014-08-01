#include "Runnable.h"
#include "JobDispatcher.h"
#include "TestThread.h"
#include "TestObject.h"


bool TestThread::Run()
{
	/// TEST
	uint32_t after = rand() % 2000;

	if ( after > 1000 )
	{
		GTestObjects[rand() % MAX_TEST_OBJECTS]->DoAsync(&TestObject::TestFunc0);
		GTestObjects[rand() % MAX_TEST_OBJECTS]->DoAsync(&TestObject::TestFunc2, double(rand()%100), 2);
		GTestObjects[rand() % MAX_TEST_OBJECTS]->DoAsync(&TestObject::TestFunc1, 1);

		GTestObjects[rand() % MAX_TEST_OBJECTS]->DoAsyncAfter(after, &TestObject::TestFuncForTimer, (int)after);
	}

	/// exit condition
	if (GTestObjects[rand() % MAX_TEST_OBJECTS]->GetTestCount() > 10000)
	{
		printf("Thread %d end by force\n", LWorkerThreadId);
		return false;
	}

	return true;
}

