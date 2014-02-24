#pragma once

#include <functional>
#include "ObjectPool.h"

template <int N>
struct TupleUnpacker
{
	template <class ObjType, class... FuncArgs, class... TupleArgs, class... Args>
	static void DoExecute(ObjType* obj, void (ObjType::*memfunc)(FuncArgs...), const std::tuple<TupleArgs...>& targ, Args... args)
	{
		TupleUnpacker<N - 1>::DoExecute(obj, memfunc, targ, std::get<N - 1>(targ), args...);
	}
};

template <>
struct TupleUnpacker<0>
{
	template <class ObjType, class... FuncArgs, class... TupleArgs, class... Args>
	static void DoExecute(ObjType* obj, void (ObjType::*memfunc)(FuncArgs...), const std::tuple<TupleArgs...>& targ, Args... args)
	{
		(obj->*memfunc)(args...);
	}
};


template <class ObjType, class... FuncArgs, class... TupleArgs>
void DoExecuteTuple(ObjType* obj, void (ObjType::*memfunc)(FuncArgs...), std::tuple<TupleArgs...> const& targ)
{
	TupleUnpacker<sizeof...(TupleArgs)>::DoExecute(obj, memfunc, targ);
}


struct NodeEntry
{
	NodeEntry() : mNext(nullptr) {}
	NodeEntry* volatile mNext;
};

struct JobEntry
{
	JobEntry() 	{}
	virtual ~JobEntry() {}

	virtual void OnExecute()
	{}

	NodeEntry mNodeEntry;
} ;


template <class ObjType, class... ArgTypes>
struct Job : public JobEntry, ObjectPool<Job<ObjType, ArgTypes...>>
{
	typedef void (ObjType::*MemFunc_)(ArgTypes... args);
	typedef std::tuple<ArgTypes...> Args_;


	Job(ObjType* obj, MemFunc_ memfunc, ArgTypes... args)
		: mObject(obj), mMemFunc(memfunc), mArgs(std::forward<ArgTypes>(args)...)
	{}

	virtual ~Job() {}

	virtual void OnExecute()
	{
		DoExecuteTuple(mObject, mMemFunc, mArgs);
	}

	ObjType*	mObject;
	MemFunc_	mMemFunc;
	Args_		mArgs;
};
