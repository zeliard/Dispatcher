#include "stdafx.h"
#include <time.h>
#include "WorkerThreadManager.h"
#include "TestThread.h"
#include "TestObject.h"

int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned int)time(NULL));

	for (int i = 0; i < MAX_TEST_OBJECTS; ++i)
		GTestObjects[i] = new TestObject;

	GWorkerThreadManager = new WorkerThreadManager(4);

	GWorkerThreadManager->RunWorkerThreads<TestThread>(); ///< block here

	for (int i = 0; i < MAX_TEST_OBJECTS; ++i)
	{
		printf("TestObject %d - TestCount %d\n", i, GTestObjects[i]->GetTestCount());
	}
		
	getchar();

	return 0;
}
