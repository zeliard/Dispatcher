G.O.D: Grand Object-bound Dispatcher
==========
G.O.D is a high performance non-blocking task dispatcher which guarantees class member functions' execution sequence 

## Features
* Designed for multi-platform by using C++11 standard
** Visual Studio Solutions for now, but you can easily use in *NIX environment (Just #include relevant files)
* Wait-free algorithms
* Simple code-base for easy to understand (easy to adapt to other projects)


## HOW TO USE

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

For more information, just see self-explaning [DispatcherTest.cpp](JobDispatcher/DispatcherTest.cpp)  

## WARNING!

1. you should use x64 mode due to std::atomic variable alignment
2. you should guarantee the lifetime (i.e. refcount > 0) of an object inherited from AsyncExecutable
3. be careful! when using blockable LOCKs in an async job... (which can cause deadlock.. )


