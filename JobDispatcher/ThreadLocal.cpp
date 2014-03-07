#include "ThreadLocal.h"
#include "STLAllocator.h"
#include "Timer.h"
#include "JobDispatcher.h"


thread_local Timer* LTimer = nullptr;
thread_local int64_t LTickCount = 0;

thread_local ExecuterListType*	LExecuterList = nullptr;
thread_local AsyncExecutable*	LCurrentExecuterOccupyingThisThread = nullptr;

thread_local LocalMemoryPool* LMemoryPool = nullptr;
