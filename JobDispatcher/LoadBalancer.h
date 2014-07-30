#pragma once

class LoadBalancer
{
public:
	LoadBalancer(int threadCount);
	~LoadBalancer();

	void SetRecentTickElapsed(int64_t elapsed);
	int GetFastestWorkerThreadId() const;

private:

	int			mWorkerThreadCount;
	int64_t*	mRecentTickElapsed;
};