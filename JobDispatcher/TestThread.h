#pragma once

#include "Runnable.h"

class TestThread : public Runnable
{
public:
	TestThread() {}
	virtual ~TestThread() {}

	virtual bool Run();
};


//////////////////////////// TEST
#include "JobDispatcher.h"


const int TEST_OBJECT_COUNT = 10;


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

	void TestFunc2(double a, int b);
	

	void TestFuncForTimer(int b);
	


	int GetTestCount() { return mTestCount; }
private:

	int mTestCount;

};

extern TestObject* GTestObject[TEST_OBJECT_COUNT];
