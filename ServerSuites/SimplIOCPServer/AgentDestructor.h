#pragma once
#include "utilities.h"

class AgentDestructor /*: public IAgentDestructor*/
{
public:
	using _Callable = std::function< void(void) >;

	AgentDestructor();
	virtual ~AgentDestructor();

	virtual void Reg(_Callable _fn);
	void Destruct();

private:
	std::stack< _Callable > _contFn;
};

