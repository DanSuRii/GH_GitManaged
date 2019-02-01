#include "pch.h"
#include "AgentDestructor.h"

AgentDestructor::AgentDestructor()
{
}


AgentDestructor::~AgentDestructor()
{
	while (_contFn.size())
	{
		auto fn = _contFn.top();
		fn();
		_contFn.pop();
	}
}

void AgentDestructor::Reg(_Callable _fn)
{
	_contFn.push(_fn);
}
