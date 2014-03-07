#include "STLAllocator.h"

#include <memory>

LocalMemoryPool::LocalMemoryPool()
{
	memset(mAllocCountTable, 0, sizeof(mAllocCountTable));

	for (int i = 0; i < MAX_POOL_SIZE; ++i)
	{
		mFixedSizeMemoryPool[i].SetFixedAllocSize((i + 1)*ALLOC_GRANULARITY);
	}

}


void LocalMemoryPool::PrintAllocationStatus()
{
	for (int i = 0; i < MAX_POOL_SIZE; ++i)
	{
		if (mAllocCountTable[i]>0)
			printf_s("SIZE: %d, Allocation Count: %d\n", 
			(i + 1)*ALLOC_GRANULARITY, mAllocCountTable[i]);
	}
}

