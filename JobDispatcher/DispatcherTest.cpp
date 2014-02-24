#include "stdafx.h"

#include "JobDispatcher.h"

#include <vector>
#include <iostream>
#include <thread>



#define WORKER_THREAD	8

class TestObject : public AsyncExecutable
{
public:
	TestObject() : mTestCount(0)
	{}

	void TestFunc0()
	{
		++mTestCount;
		//printf("[%d] TestFunc0 \n", mTestCount);
	}

	void TestFunc1(int b)
	{
		++mTestCount;
		//printf("[%d] TestFunc1 %d\n", mTestCount, b);
	}

	void TestFunc2(double a, int b)
	{
		++mTestCount;
		//printf("[%d] TestFunc2 %f\n", mTestCount, a + b);
	}


	int GetTestCount() { return mTestCount; }
private:

	int mTestCount;

};

TestObject* GTestObject[WORKER_THREAD] = { 0, };

void TestWorkerThread(int tid)
{
	LExecuterList = new std::deque<AsyncExecutable*>;

	for (int i = 0; i < 100000; ++i)
	{
		
 		GTestObject[rand() % WORKER_THREAD]->DoAsync(&TestObject::TestFunc2, double(tid) * 100, i);
 		GTestObject[rand() % WORKER_THREAD]->DoAsync(&TestObject::TestFunc1, 100);
 		GTestObject[rand() % WORKER_THREAD]->DoAsync(&TestObject::TestFunc0);
	}

	delete LExecuterList;
}

int _tmain(int argc, _TCHAR* argv[])
{

	for (int i = 0; i < WORKER_THREAD; ++i)
		GTestObject[i] = new TestObject;

	std::vector<std::thread> threadList;

	for (int i = 0; i <WORKER_THREAD; ++i)
	{
		threadList.push_back(std::thread(TestWorkerThread, i+1));
	}

	
	for (auto& thread : threadList)
	{
		if (thread.joinable())
			thread.join();
	}

	
	int total = 0;
	for (int i = 0; i < WORKER_THREAD; ++i)
		total += GTestObject[i]->GetTestCount();

	printf("TOTAL %d\n", total);


	getchar();

	for (int i = 0; i < WORKER_THREAD; ++i)
		delete GTestObject[i];

	return 0;
}
