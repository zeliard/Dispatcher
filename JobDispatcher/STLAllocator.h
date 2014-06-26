#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include "ThreadLocal.h"

#define _ASSERT_CRASH(expr) \
	{ \
		if (!(expr)) \
		{ \
			int* dummy = 0; \
			*dummy = 0xDEADBEEF; \
		} \
	}


class FixedSizeMemoryPool
{
public:
	FixedSizeMemoryPool() : mAllocSize(0), mFreeList(nullptr)
	{}

	enum Config
	{
		ALLOC_COUNT = 128
	};

	void* Malloc()
	{
		if (!mFreeList)
		{
			mFreeList = static_cast<uint8_t*>(malloc(mAllocSize*ALLOC_COUNT));

			uint8_t* pNext = mFreeList;
			uint8_t** ppCurr = reinterpret_cast<uint8_t**>(mFreeList);

			for (int i = 0; i < ALLOC_COUNT - 1; ++i)
			{
				pNext += mAllocSize;
				*ppCurr = pNext;
				ppCurr = reinterpret_cast<uint8_t**>(pNext);
			}

			*ppCurr = 0;
		}

		uint8_t* pAvailable = mFreeList;
		mFreeList = *reinterpret_cast<uint8_t**>(pAvailable);

		return pAvailable;
	}

	void Free(void* ptr)
	{
		*reinterpret_cast<uint8_t**>(ptr) = mFreeList;
		mFreeList = static_cast<uint8_t*>(ptr);
	}

	void SetFixedAllocSize(size_t size)
	{
		_ASSERT_CRASH(size >= sizeof(size_t));
		mAllocSize = size;
	}

private:
	size_t		mAllocSize;
	uint8_t*	mFreeList;
};

class LocalMemoryPool
{
public:
	LocalMemoryPool();

	void* Allocate(size_t size)
	{
		if (size >= MAX_CHUNK_SIZE)
		{
			return malloc(size);
		}
		else
		{
			size_t idx = GetPoolIndex(size);
			++mAllocCountTable[idx];
			return mFixedSizeMemoryPool[idx].Malloc();
		}
	}

	void Deallocate(void* ptr, size_t size)
	{
		if (size >= MAX_CHUNK_SIZE)
		{
			return free(ptr);
		}
		else
		{
			size_t idx = GetPoolIndex(size);
			--mAllocCountTable[idx];
			mFixedSizeMemoryPool[idx].Free(ptr);
		}
	}

	void PrintAllocationStatus();

private:

	size_t GetPoolIndex(size_t size) const 
	{ 
		return size / ALLOC_GRANULARITY; 
	}

	enum Config
	{
		ALLOC_GRANULARITY = 8,
		MAX_CHUNK_SIZE = 16384, ///< goes FixedSizeMemoryPool only when required mem-size is less than MAX_CHUNK_SIZE 
		MAX_POOL_SIZE = MAX_CHUNK_SIZE / ALLOC_GRANULARITY
	};


	FixedSizeMemoryPool mFixedSizeMemoryPool[MAX_POOL_SIZE];
	size_t				mAllocCountTable[MAX_POOL_SIZE];

	void* __powerof2check1__[((MAX_CHUNK_SIZE & (MAX_CHUNK_SIZE - 1)) == 0x0) & 0x1];
	void* __powerof2check2__[((ALLOC_GRANULARITY & (ALLOC_GRANULARITY - 1)) == 0x0) & 0x1];
};


template <class T>
class STLAllocator
{
public:
	STLAllocator() = default;

	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	template <class U>
	STLAllocator(const STLAllocator<U>&)
	{}

	template <class U>
	struct rebind
	{
		typedef STLAllocator<U> other;
	};

	void construct(pointer p, const T& t)
	{ 
		new(p) T(t);
	}

	void destroy(pointer p)
	{ 
		p->~T(); 
	}

	T* allocate(size_t n)
	{
		return static_cast<T*>(LMemoryPool->Allocate(n*sizeof(T)));
	}

	void deallocate(T* ptr, size_t n)
	{
		LMemoryPool->Deallocate(ptr, n*sizeof(T));
	}
};

