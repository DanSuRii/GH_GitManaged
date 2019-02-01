#pragma once

template< class T, class ... Args >
decltype(auto) New( Args&& ...args )
{
	return std::make_shared<T>(args...);
}

/*
class IAgentDestructor
{
public:
	using _Callable = std::function< void(void) >;

	virtual void Reg(_Callable _fn) = 0;
};
using PIAgentDestructor = std::shared_ptr< IAgentDestructor >;
*/
