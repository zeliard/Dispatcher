#include "Runnable.h"
#include "ThreadLocal.h"
#include "STLAllocator.h"
#include "Timer.h"
#include "JobDispatcher.h"

void Runnable::Initialize(int tid)
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

void Runnable::ThreadRun(int tid)
{
	Initialize(tid);

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