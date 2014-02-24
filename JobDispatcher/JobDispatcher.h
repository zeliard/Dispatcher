#pragma once

#include <deque>
#include <atomic>
#include <inttypes.h>

#include "Job.h"

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


class AsyncExecutable;

#ifdef WIN32
#define thread_local __declspec(thread)
#endif

thread_local extern std::deque<AsyncExecutable*>* LExecuterList;
thread_local extern AsyncExecutable*	LCurrentExecuterOccupyingThisThread;


class AsyncExecutable
{
public:

	AsyncExecutable() : mRemainTaskCount(0), mRefCount(0) {}
	virtual ~AsyncExecutable()
	{
		_ASSERT_CRASH(mRefCount == 0);
	}


	template <class T, class... Args>
	void DoAsync(void (T::*memfunc)(Args...), Args... args) 
	{ 
		Job<T, Args...>* job = new Job<T, Args...>(static_cast<T*>(this), memfunc, args...); 
		DoTask(job); 
	} 

	void AddRefForThis()
	{
		mRefCount.fetch_add(1);
	}

	void ReleaseRefForThis()
	{
		mRefCount.fetch_sub(1);
	}

private:
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

			AddRefForThis(); ///< refcount +1 for this object

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
					dispacher->ReleaseRefForThis();
				}

				/// release 
				LCurrentExecuterOccupyingThisThread = nullptr;
				ReleaseRefForThis(); ///< refcount -1 for this object
			}
		}
	}

	/// Execute all tasks registered in JobQueue of this dispatcher
	void Flush()
	{
		while ( true )
		{
			if (JobEntry* job = mJobQueue.Pop())
			{
				job->OnExecute();
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
	
	/// should not release this object when it is in the dispatcher
	std::atomic<int32_t> mRefCount;

};


