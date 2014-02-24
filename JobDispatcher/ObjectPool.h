#pragma once
#include <atomic>
#include <malloc.h>
#include <inttypes.h>

#define ALIGNMENT 8

#ifdef WIN32
#define EXPLICIT_MEMORY_ALIGNMENT __declspec(align(ALIGNMENT))
#define MEM_ALLOC(x)	_aligned_malloc(x, ALIGNMENT)
#define MEM_FREE(x)		_aligned_free(x)
#else
#define EXPLICIT_MEMORY_ALIGNMENT __attribute__((aligned(ALIGNMENT)))
#define MEM_ALLOC(x)	memalign(ALIGNMENT, x)
#define MEM_FREE(x)		free(x)
#endif 

#define _ASSERT_CRASH(expr) \
	{ \
		if (!(expr)) \
		{ \
			int* dummy = 0; \
			*dummy = 0xDEADBEEF; \
		} \
	}



template <class T>
class ObjectPool
{
public:

	enum
	{
		POOL_MAX_SIZE = 4096, ///< must be power of 2
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1
	};

	/// memory pre- allocation is just optional :)
	static void PrepareAllocation()
	{
		for (int i = 0; i < POOL_MAX_SIZE; ++i)
			mPool[i] = MEM_ALLOC(sizeof(T));

		mTailPos.fetch_add(POOL_MAX_SIZE);
	}

	static void* operator new(size_t objSize)
	{
		uint64_t popPos = mHeadPos.fetch_add(1);

		void* popVal = std::atomic_exchange(&mPool[popPos & POOL_SIZE_MASK], (void*)nullptr);
		if (popVal != nullptr)
			return popVal;
	
		_ASSERT_CRASH(objSize == sizeof(T));

		return MEM_ALLOC(objSize);
	}

	static void	operator delete(void* obj)
	{
		uint64_t insPos = mTailPos.fetch_add(1);

		void* prevVal = std::atomic_exchange(&mPool[insPos & POOL_SIZE_MASK], obj);

		if (prevVal != nullptr)
			MEM_FREE(prevVal);
	}


private:

	static std::atomic<void*>	 EXPLICIT_MEMORY_ALIGNMENT	mPool[POOL_MAX_SIZE];
	static std::atomic<uint64_t> EXPLICIT_MEMORY_ALIGNMENT	mHeadPos;
	static std::atomic<uint64_t> EXPLICIT_MEMORY_ALIGNMENT	mTailPos;

	void* __powerof2check__[((POOL_MAX_SIZE & POOL_SIZE_MASK) == 0x0) & 0x1];
};

template <class T>
std::atomic<void*> ObjectPool<T>::mPool[POOL_MAX_SIZE];

template <class T>
std::atomic<uint64_t> ObjectPool<T>::mHeadPos;

template <class T>
std::atomic<uint64_t> ObjectPool<T>::mTailPos;
