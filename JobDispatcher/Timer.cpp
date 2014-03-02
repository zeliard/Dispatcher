#include "Timer.h"
#include "JobDispatcher.h"

thread_local Timer* LTimer = nullptr;


Timer::Timer()
{

}

void Timer::PushTimerJob(AsyncExecutable* obj, uint32_t after, JobEntry* task)
{
	obj->AddRefForThis(); ///< for timer
	mObjTest = obj;
	mTaskTest = task;
}


void Timer::DoTimerJob()
{
	//TODO: at timer event
	mObjTest->DoTask(mTaskTest);
	mObjTest->ReleaseRefForThis();

	
}

