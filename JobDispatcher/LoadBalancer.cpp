#include <stdint.h>
#include <algorithm>
#include "ThreadLocal.h"
#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(int threadCount) : mWorkerThreadCount(threadCount)
{
	mRecentTickElapsed = new int64_t[mWorkerThreadCount];
}

LoadBalancer::~LoadBalancer()
{
	delete[] mRecentTickElapsed;
}

void LoadBalancer::SetRecentTickElapsed(int64_t elapsed)
{
	mRecentTickElapsed[LWorkerThreadId] = elapsed;
}

int LoadBalancer::GetFastestWorkerThreadId() const
{
	auto it = std::min_element(mRecentTickElapsed, mRecentTickElapsed + mWorkerThreadCount);
	return static_cast<int>(std::distance(mRecentTickElapsed, it));
}
