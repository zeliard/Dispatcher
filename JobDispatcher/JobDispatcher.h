#pragma once

#include <atomic>
#include <thread>
#include <memory>

#include "STLAllocator.h"
#include "Job.h"
#include "Timer.h"
#include "ThreadLocal.h"

class JobQueue
{
public:
	JobQueue() : mHead(&mStub), mTail(&mStub)
	{
		mOffset = offsetof(struct JobEntry, mNodeEntry);
		_ASSERT_CRASH(mHead.is_lock_free());
	}
	~JobQueue() {}

	/// mutiple produce
	void Push(JobEntry* newData)
	{
		NodeEntry* prevNode = (NodeEntry*)std::atomic_exchange_explicit(&mHead,
			&newData->mNodeEntry, std::memory_order_acq_rel);

		prevNode->mNext = &(newData->mNodeEntry);
	}

	/// single consume
	JobEntry* Pop()
	{
		NodeEntry* tail = mTail;
		NodeEntry* next = tail->mNext;

		if (tail == &mStub)
		{
			/// in case of empty
			if (nullptr == next)
				return nullptr;

			/// first pop
			mTail = next;
			tail = next;
			next = next->mNext;
		}

		/// in most cases...
		if (next)
		{
			mTail = next;

			return reinterpret_cast<JobEntry*>(reinterpret_cast<int64_t>(tail)-mOffset);
		}

		NodeEntry* head = mHead;
		if (tail != head)
			return nullptr;

		/// last pop
		mStub.mNext = nullptr;
		
		NodeEntry* prevNode = (NodeEntry*)std::atomic_exchange_explicit(&mHead, 
			&mStub, std::memory_order_acq_rel);
		
		prevNode->mNext = &mStub;

		next = tail->mNext;
		if (next)
		{
			mTail = next;

			return reinterpret_cast<JobEntry*>(reinterpret_cast<int64_t>(tail)-mOffset);
		}

		return nullptr;
	}


private:

	std::atomic<NodeEntry*> mHead;

	NodeEntry*			mTail;
	NodeEntry			mStub;

	int64_t				mOffset;

};



class AsyncExecutable : public std::enable_shared_from_this<AsyncExecutable>
{
public:

	AsyncExecutable() : mRemainTaskCount(0) {}
	virtual ~AsyncExecutable()
	{}


	template <class T>
	std::shared_ptr<T> GetSharedFromThis()
	{
		static_assert(true == std::is_convertible<T*, AsyncExecutable*>::value, "T should be derived from AsyncExecutable");

		return std::static_pointer_cast<T>(shared_from_this());
	}

	/// Push a task into Job Queue, and then Execute tasks if possible
	void DoTask(JobEntry* task)
	{
		if ( mRemainTaskCount.fetch_add(1) != 0 )
		{
			/// register the task in this dispatcher
			mJobQueue.Push(task);
		}
		else
		{
			/// register the task in this dispatcher
			mJobQueue.Push(task);

			/// Does any dispathcer exist occupying this worker-thread at this moment?
			if (LCurrentExecuterOccupyingThisThread != nullptr)
			{
				/// just register this dispatcher in this worker-thread
				LExecuterList->push_back(this);
			}
			else
			{
				/// acquire
				LCurrentExecuterOccupyingThisThread = this;

				/// invokes all tasks of this dispatcher
				Flush();

				/// invokes all tasks of other dispatchers registered in this thread
				while (!LExecuterList->empty())
				{
					AsyncExecutable* dispacher = LExecuterList->front();
					LExecuterList->pop_front();
					dispacher->Flush();
					
				}

				/// release 
				LCurrentExecuterOccupyingThisThread = nullptr;
				
			}
		}
	}

private:
	/// Execute all tasks registered in JobQueue of this dispatcher
	void Flush()
	{
		while ( true )
		{
			if (JobEntry* job = mJobQueue.Pop())
			{
				job->mTask();
				delete job;

				if ( mRemainTaskCount.fetch_sub(1) == 1 )
					break;
			}
		}
	}

private:
	/// member variables
	JobQueue	mJobQueue;

	std::atomic<int64_t> mRemainTaskCount;

	friend class Timer;
};


template <class T>
struct is_shared_ptr
{
	const static bool value = false;
};

template <class T>
struct is_shared_ptr<std::shared_ptr<T>>
{
	const static bool value = true;
};



template <class T, class F, class... Args>
void DoAsync(T instance, F memfunc, Args&&... args)
{ 
	static_assert(true == is_shared_ptr<T>::value, "T should be shared_ptr");
	static_assert(true == std::is_convertible<T, std::shared_ptr<AsyncExecutable>>::value, "T should be shared_ptr AsyncExecutable");
	
	JobEntry* job = new JobEntry(std::bind(memfunc, instance.get(), std::forward<Args>(args)...));

	std::static_pointer_cast<AsyncExecutable>(instance)->DoTask(job);
} 

template <class T, class F, class... Args>
void DoAsyncAfter(uint32_t after, T instance, F memfunc, Args&&... args)
{
	static_assert(true == is_shared_ptr<T>::value, "T should be shared_ptr");
	static_assert(true == std::is_convertible<T, std::shared_ptr<AsyncExecutable>>::value, "T should be shared_ptr AsyncExecutable");

	JobEntry* job = new JobEntry(std::bind(memfunc, instance.get(), std::forward<Args>(args)...));
	LTimer->PushTimerJob(std::static_pointer_cast<AsyncExecutable>(instance), after, job);
}