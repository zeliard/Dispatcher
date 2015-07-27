#pragma once

class Runnable
{
public:
	Runnable() = default;
	virtual ~Runnable() = default;

	void ThreadRun();

	virtual bool Run() = 0;

private:
	void Initialize();
	void Finalize();
};

