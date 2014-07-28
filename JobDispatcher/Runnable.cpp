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
	LWorkerThreadId = mThreadId = tid;
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
		/// do task...
		if ( false == Run() )
			break;

		LTimer->DoTimerJob();
	}

	Finalize();
}