#pragma once
#include <inttypes.h>
#include <deque>

#ifdef WIN32
#define thread_local __declspec(thread)
#else
#define thread_local __thread
#endif

class Timer;
class AsyncExecutable;
class LocalMemoryPool;

template <class T>
class STLAllocator;

typedef std::deque<AsyncExecutable*, STLAllocator<AsyncExecutable*>> ExecuterListType;

extern thread_local Timer* LTimer;
extern thread_local int64_t LTickCount;

extern thread_local ExecuterListType*	LExecuterList;
extern thread_local AsyncExecutable*	LCurrentExecuterOccupyingThisThread;

extern thread_local LocalMemoryPool* LMemoryPool;