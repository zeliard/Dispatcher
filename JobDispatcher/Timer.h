#pragma once

#include <chrono>
#include "Job.h"

typedef std::chrono::high_resolution_clock Clock;

class AsyncExecutable;

class Timer
{
public:
	Timer();

	
	void PushTimerJob(AsyncExecutable* obj, uint32_t after, JobEntry* task);
	
	void DoTimerJob();

	AsyncExecutable* mObjTest;
	JobEntry* mTaskTest;

} ;


thread_local extern Timer* LTimer;