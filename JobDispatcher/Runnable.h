#pragma once

class LoadBalancer;

class Runnable
{
public:
	Runnable()  {}
	virtual ~Runnable() {}

	void ThreadRun(int tid, LoadBalancer* lb);
	virtual bool Run() = 0;

	int GetThreadId() { return mThreadId; }

private:
	void Initialize(int tid);
	void Finalize();

	int mThreadId;

};


