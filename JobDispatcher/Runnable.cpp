#include "Runnable.h"
#include "ThreadLocal.h"
#include "STLAllocator.h"
#include "Timer.h"
#include "JobDispatcher.h"
#include "LoadBalancer.h"

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


void Runnable::ThreadRun(int tid, LoadBalancer* lb)
{
	Initialize(tid); 
	
	while (true)
	{
		/// do content-specific tasks...
		if ( false == Run() )
			break;

		/// thread tick update
		int64_t currTick = LTimer->GetCurrentTick();
		lb->SetRecentTickElapsed(currTick - LTickCount);
		LTickCount = currTick;

		/// do timer tasks
		LTimer->DoTimerJob();

		/// invokes all tasks of other dispatchers registered in this thread
		while (!LExecuterList->empty())
		{
			//TODO: load balancing... here

			AsyncExecutable* dispacher = LExecuterList->front();
			LExecuterList->pop_front();
			dispacher->Flush();
			dispacher->ReleaseRefForThis();
		}
		
	}

	Finalize();
}