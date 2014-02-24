Dispatcher
==========

High Performance Non-blocking Task(Job) Dispatcher (C++ 11 standard, multi-platform)


## HOW TO USE
self-explaning DispatcherTest.cpp 

```C++
// IMPORTANT: the beginning of a workerthread, you should put this:
LExecuterList = new std::deque<AsyncExecutable*>;

// And then you can attach dispatcher(AsyncExecutable)to an object like this:
class TestObject : public AsyncExecutable
{
{
public:

	void TestFunc(double a, int b)
	{
	  // ...
	}
}

// Now, you can call a member function like this:
TestObject* testobj = new TestObject;
testobject->DoAsync(&TestObject::TestFunc, 100.123, 456);

```

## WARNING!

1. you should use x64 mode due to std::atomic variable alignment
2. you should guarantee the lifetime (i.e. refcount > 0) of an object inherited from AsyncExecutable
3. be careful! when using blockable LOCKs in an async job... (which can cause deadlock.. )


