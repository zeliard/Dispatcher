#pragma once

class Runnable
{
public:
	Runnable()  {}
	virtual ~Runnable() {}

	void ThreadRun(int tid);
	virtual bool Run() = 0;

	int GetThreadId() { return mThreadId; }

private:
	void Initialize();
	void Finalize();

	int mThreadId;

};


