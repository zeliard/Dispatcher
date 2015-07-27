G.O.D: Grand Object-bound Dispatcher
==========

G.O.D is a high performance non-blocking task dispatcher which guarantees class member functions' execution sequence 

## FEATURES
* Designed for multi-platform by using C++11 standard
 * Visual Studio Solutions for now, but you can easily use in *NIX environment (Just #include relevant files)
* Lock-free algorithms (non-blocking)
* Using custom STL allocator
* Deferred task execution using Timer
* Simple code-base for easy to understand (easy to adapt to other projects)

## REAL WORLD EXAMPLES
* [TERA's Lock-Free Executor and Timer](http://download.enmasse.com/documents/201205-gdm-tera.pdf)
  - Conceptually the same, but G.O.D is a improved version of TERA's.
* [Mac OS X Grand Central Dispatch (GCD)](http://en.wikipedia.org/wiki/Grand_Central_Dispatch)
  - G.O.D is a kind of minimal/portable version of GCD
 
## BRANCHES
* base_version: basic version for educational purposes
* bind_version: std::bind is adopted instead of a custom Job class interface
* LB_version: load-balancing among worker-threads (new arrival!)

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
auto testobj = std::make_shared<TestObject>(); ///< new TestObject;

// And then, make your own worker thread which implements Runnable::Run() like this:
class TestWorkerThread : public Runnable
{
public:
	virtual bool Run()
	{
	   // Now, you can call a member function like this:
	   testobj->DoAsync(&TestObject::TestFunc, 100.123, 456);

	   // or, deferred execution 1000ms later like this:
	   testobj->DoAsyncAfter(1000, &TestObject::TestFunc, 100.123, 456);
	   
	   // ... ...
	}
};

// Lastly, run worker-threads at the main() function
JobDispatcher<TestWorkerThread> workerService(WORKER_COUNT);
workerService.RunWorkerThreads();

```

For more information, just see self-explaning [DispatcherTest.cpp](JobDispatcher/DispatcherTest.cpp)  

## WARNING!
1. You should use x64 mode due to std::atomic variable alignment
2. You should guarantee the lifetime (i.e. AsyncExecutable::mRefCount > 0) of an object inherited from AsyncExecutable
3. Be careful to use DoAsync(...) in a blockable LOCK (critical section). It can cause deadlock...


