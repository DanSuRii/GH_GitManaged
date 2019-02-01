#pragma once
#include "utilities.h"

class AgentDestructor /*: public IAgentDestructor*/
{
public:
	using _Callable = std::function< void(void) >;

	AgentDestructor();
	virtual ~AgentDestructor();

	virtual void Reg(_Callable _fn);

private:
	std::stack< _Callable > _contFn;
};

