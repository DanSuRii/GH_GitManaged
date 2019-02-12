#include "pch.h"
#include "AgentDestructor.h"

AgentDestructor::AgentDestructor()
{
}


AgentDestructor::~AgentDestructor()
{
	Destruct(); //user must define in the Destructor to call this::Destruct().
}

void AgentDestructor::Reg(_Callable _fn)
{
	_contFn.push(_fn);
}

void AgentDestructor::Destruct()
{
	while (_contFn.size())
	{
		auto fn = _contFn.top();
		fn();
		_contFn.pop();
	}
}

