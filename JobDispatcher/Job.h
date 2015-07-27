#pragma once

#include <functional>
#include "ObjectPool.h"

struct NodeEntry
{
	NodeEntry() : mNext(nullptr) {}
	NodeEntry* volatile mNext;
};

typedef std::function<void()> Job;

struct JobEntry : public ObjectPool<JobEntry>
{
	JobEntry(Job&& job) : mTask(std::move(job)) 	{}
	virtual ~JobEntry() {}
	
	NodeEntry mNodeEntry;

	Job mTask;
} ;




