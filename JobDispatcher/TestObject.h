#pragma once
#include "JobDispatcher.h"

class TestObject : public AsyncExecutable
{
public:
	TestObject() : mTestCount(0)
	{}

	void TestFunc0()
	{
		++mTestCount;
	}

	void TestFunc1(int b)
	{
		mTestCount += b;
	}

	void TestFunc2(double a, int b)
	{
		mTestCount += b;

		if (a < 50.0)
			DoAsync(&TestObject::TestFunc1, b);
	}

	void TestFuncForTimer(int b)
	{
		//printf("TestFuncForTimer [%d] \n", b);

		if (rand() % 2 == 0)
			DoAsyncAfter(1000, &TestObject::TestFuncForTimer, -b);
	}

	int GetTestCount() { return mTestCount; }

private:

	int mTestCount;

};

#define MAX_TEST_OBJECTS 10

extern TestObject* GTestObjects[MAX_TEST_OBJECTS];
