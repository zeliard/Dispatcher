#include "stdafx.h"
#include "JobDispatcher.h"


thread_local std::deque<AsyncExecutable*>* LExecuterList = nullptr;
thread_local AsyncExecutable*	LCurrentExecuterOccupyingThisThread = nullptr;

