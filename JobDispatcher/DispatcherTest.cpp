#include "stdafx.h"

#include "WorkerThreadManager.h"
#include "TestThread.h"


TestObject* GTestObject[TEST_OBJECT_COUNT] = { 0, };

int _tmain(int argc, _TCHAR* argv[])
{
	for (int i = 0; i < TEST_OBJECT_COUNT; ++i)
		GTestObject[i] = new TestObject;

	GWorkerThreadManager = new WorkerThreadManager(4);

	GWorkerThreadManager->RunWorkerThreads<TestThread>(); ///< block here



	delete GWorkerThreadManager;
	return 0;
}
