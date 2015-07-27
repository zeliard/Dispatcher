#include "Runnable.h"
#include "ThreadLocal.h"
#include "STLAllocator.h"
#include "Timer.h"
#include "JobDispatcher.h"

void Runnable::Initialize()
{
	LMemoryPool = new LocalMemoryPool;
	LExecuterList = new ExecuterListType;
	LTimer = new Timer;
}

void Runnable::Finalize()
{
	delete LTimer;
	delete LExecuterList;
	delete LMemoryPool;
}

void Runnable::ThreadRun()
{
	Initialize();

	while (true)
	{
		/// do content-specific tasks...
		if (false == Run())
			break;

		/// do timer tasks
		LTimer->DoTimerJob();

	}

	Finalize();
}