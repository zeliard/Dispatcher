G.O.D: Grand Object-bound Dispatcher
==========

G.O.D is a high performance non-blocking task dispatcher which guarantees class member functions' execution sequence 

## FEATURES
* Designed for multi-platform by using C++11 standard
 * Visual Studio Solutions for now, but you can easily use in *NIX environment (Just #include relevant files)
* Lock-free algorithms (non-blocking)
* Load-balancing among worker-threads (load-scatterer)
* Using custom STL allocator
* Deferred task execution using Timer
* Simple code-base for easy to understand (easy to adapt to other projects)


## HOW TO USE

```C++

// First, you can attach G.O.D (AsyncExecutable) to an object like this:
class TestObject : public AsyncExecutable
{
public:

	void TestFunc(double a, int b)
	{
	  // do something... 
	  // (e.g.) someobj->DoAsync(...);
	}
	
	// ... ...
};


// somewhere ... 
TestObject* testobj = new TestObject;

// And then, make your own worker thread which implements Runnable::Run() like this:
class TestThread : public Runnable
{
public:
	virtual bool Run()
	{
	   // Now, you can call a member function like this:
	   testobject->DoAsync(&TestObject::TestFunc, 100.123, 456);

	   // or, deferred execution 1000ms later like this:
	   testobject->DoAsyncAfter(1000, &TestObject::TestFunc, 100.123, 456);
	}
};

```

For more information, just see self-explaning files:
[DispatcherTest.cpp](JobDispatcher/DispatcherTest.cpp)
[TestObject.h](JobDispatcher/TestObject.h)
[TestThread.cpp](JobDispatcher/TestThread.cpp)


## WARNING!

1. You should use x64 mode due to std::atomic variable alignment
2. You should guarantee the lifetime (i.e. AsyncExecutable::mRefCount > 0) of an object inherited from AsyncExecutable
3. Be careful to use DoAsync(...) in a blockable LOCK (critical section). It can cause deadlock...


