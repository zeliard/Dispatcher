#include "STLAllocator.h"

#include <stdio.h>

LocalMemoryPool::LocalMemoryPool()
{
	for (int i = 0; i < MAX_POOL_SIZE; ++i)
	{
		mAllocCountTable[i] = 0;
		mFixedSizeMemoryPool[i].SetFixedAllocSize((i + 1)*ALLOC_GRANULARITY);
	}

}


void LocalMemoryPool::PrintAllocationStatus()
{
	for (int i = 0; i < MAX_POOL_SIZE; ++i)
	{
		if (mAllocCountTable[i]>0)
			printf("SIZE: %d, Allocation Count: %d\n", (i + 1)*ALLOC_GRANULARITY, mAllocCountTable[i]);
	}
}

