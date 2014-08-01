#pragma once
#include <atomic>
#include <vector>

class AsyncExecutable;

typedef std::vector<AsyncExecutable*> DispatcherList; ///< don't need to use custom STL allocator due to occasionalness

class LoadBalancingTask
{
public:
	LoadBalancingTask(int threadCount);
	~LoadBalancingTask();

	void HandOverToOtherThreads(AsyncExecutable* dispatcher);
	bool OnThreadLocalExecute(); ///< returns whether this task has done or not

private:
	
	std::atomic<int> mRefCount;
	int mWorkerThreadCount;

	DispatcherList* mHandOverList;
	int mCurrentHandOverThreadId;

};

class LoadBalancer
{
public:
	LoadBalancer(int threadCount);
	~LoadBalancer();

	void DoLoadBalancing();
	void DoLoadSharing();

private:

	void PushLoadBalancingTask(LoadBalancingTask* lbTask);
	void PopLoadBalancingTask(int64_t index);

private:

	enum Config
	{
		LB_MAX_TASK_SIZE = 1024,
		LB_MAX_TAST_MASK = LB_MAX_TASK_SIZE-1,
		LB_HANDOVER_THRESHOLD = 100 ///< worker thread loop elapsed time (ms)
	};

	std::atomic<LoadBalancingTask*> mLoadBalancingTasks[LB_MAX_TASK_SIZE];
	std::atomic<int64_t> mLoadBalancingTaskCount;

	int	 mWorkerThreadCount;
	int64_t* mRecentTickElapsed;

	void* __powerof2check__[((LB_MAX_TASK_SIZE & LB_MAX_TAST_MASK) == 0x0) & 0x1];
};