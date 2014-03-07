#pragma once
#include <inttypes.h>
#include <deque>

#ifdef WIN32
#define thread_local __declspec(thread)
#endif

class Timer;
class AsyncExecutable;
class LocalMemoryPool;

template <class T>
class STLAllocator;

typedef std::deque<AsyncExecutable*, STLAllocator<AsyncExecutable*>> ExecuterListType;

thread_local extern Timer* LTimer;
thread_local extern int64_t LTickCount;

thread_local extern ExecuterListType*	LExecuterList;
thread_local extern AsyncExecutable*	LCurrentExecuterOccupyingThisThread;

thread_local extern LocalMemoryPool* LMemoryPool;