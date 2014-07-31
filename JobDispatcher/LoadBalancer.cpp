#include <stdint.h>
#include <algorithm>
#include "ThreadLocal.h"
#include "STLAllocator.h"
#include "LoadBalancer.h"
#include "JobDispatcher.h"

LoadBalancingTask::LoadBalancingTask(int threadCount) : mRefCount(threadCount), mWorkerThreadCount(threadCount)
{
	mRefCount.fetch_add(threadCount);
	mCurrentHandOverThreadId = 0;
	mHandOverList = new DispatcherList[mWorkerThreadCount]; //TODO: need to be optimized
}

LoadBalancingTask::~LoadBalancingTask()
{
	delete[] mHandOverList;
}


void LoadBalancingTask::HandOverToOtherThreads(AsyncExecutable* dispatcher)
{
	if (LWorkerThreadId == mCurrentHandOverThreadId)
		++mCurrentHandOverThreadId;

	if (mCurrentHandOverThreadId == mWorkerThreadCount)
		mCurrentHandOverThreadId = 0;

	mHandOverList[mCurrentHandOverThreadId++].push_back(dispatcher);

}

bool LoadBalancingTask::OnThreadLocalExecute()
{
	const DispatcherList& listForThisThread = mHandOverList[LWorkerThreadId];

	for (auto& it : listForThisThread)
	{
		LExecuterList->push_back(it);
	}
	
	if (1 == mRefCount.fetch_sub(1))
		return true;

	return false;
}




LoadBalancer::LoadBalancer(int threadCount) : mWorkerThreadCount(threadCount), mLoadBalancingTaskCount(0)
{
	mRecentTickElapsed = new int64_t[mWorkerThreadCount];
	
	for (int i = 0; i < LB_MAX_TASK_SIZE; ++i)
		std::atomic_init(&mLoadBalancingTasks[i], nullptr);

}

LoadBalancer::~LoadBalancer()
{
	delete[] mRecentTickElapsed;
}

void LoadBalancer::SetRecentTickElapsed(int64_t elapsed)
{
	mRecentTickElapsed[LWorkerThreadId] = elapsed;
}


void LoadBalancer::DoLoadBalancing()
{
	if (mRecentTickElapsed[LWorkerThreadId] < LB_HANDOVER_THRESHOLD)
		return;

	if (LExecuterList->size() < mWorkerThreadCount)
		return;

	LoadBalancingTask* lbTask = new LoadBalancingTask(mWorkerThreadCount);

	while (!LExecuterList->empty())
	{
		AsyncExecutable* dispacher = LExecuterList->front();
		LExecuterList->pop_front();
		lbTask->HandOverToOtherThreads(dispacher);
	}

	PushLoadBalancingTask(lbTask);
}

void LoadBalancer::DoLoadSharing()
{
	while (true)
	{
		if (LCurrentLoadBalancingTaskIndex >= mLoadBalancingTaskCount.load())
			break;
		
		LoadBalancingTask* lbTask = mLoadBalancingTasks[LCurrentLoadBalancingTaskIndex & LB_MAX_TAST_MASK].load();

		if (lbTask->OnThreadLocalExecute())
		{
			/// final call only in one thread
			PopLoadBalancingTask(LCurrentLoadBalancingTaskIndex);
		}

		++LCurrentLoadBalancingTaskIndex;
	}
}

void LoadBalancer::PushLoadBalancingTask(LoadBalancingTask* lbTask)
{
	int64_t index = mLoadBalancingTaskCount.fetch_add(1);

	LoadBalancingTask* prevTask = std::atomic_exchange(&mLoadBalancingTasks[index & LB_MAX_TAST_MASK], lbTask);
	_ASSERT_CRASH(prevTask == nullptr);
}

void LoadBalancer::PopLoadBalancingTask(int64_t index)
{
	LoadBalancingTask* prevTask = std::atomic_exchange(&mLoadBalancingTasks[index & LB_MAX_TAST_MASK], (LoadBalancingTask*)nullptr);
	_ASSERT_CRASH(prevTask != nullptr);
	delete prevTask;
}