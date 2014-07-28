#pragma once

#include <thread>
#include <vector>
#include <memory>
#include "Runnable.h"

class WorkerThreadManager
{
public:
	WorkerThreadManager(int workerCount) : mWorkerThreadCount(workerCount) {}

	template <class T>
	void RunWorkerThreads()
	{
		static_assert(true == std::is_convertible<T, Runnable>::value, "only allowed when Runnable");

		for (int i = 0; i < mWorkerThreadCount; ++i)
		{
			std::shared_ptr<T> instance = std::make_shared<T>();
			mWorkerThreadList.push_back(std::thread(&T::ThreadRun, instance, i));
		}

		for (auto& thread : mWorkerThreadList)
		{
			if (thread.joinable())
				thread.join();
		}
	}

private:

	int mWorkerThreadCount;
	std::vector<std::thread> mWorkerThreadList;
};

extern WorkerThreadManager* GWorkerThreadManager;