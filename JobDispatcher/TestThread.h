#pragma once

#include "Runnable.h"

class TestThread : public Runnable
{
public:
	TestThread() {}
	virtual ~TestThread(){}

	virtual bool Run();
};
