#pragma once

template< class T, class ... Args >
decltype(auto) New(Args&& ...args)
{
	return std::make_shared<T>(args...);
}

namespace NS_DPUTIL
{
	class RAII
	{
	public:
		using _Callable = std::function< void(void) >;

		RAII(_Callable fn) : _fn(fn) {}
		~RAII(){ if (_fn) _fn();	}

	private:
		_Callable _fn;
	};

	class RAIIStack
	{
	public:
		void Push(RAII::_Callable fn);
		~RAIIStack();
	private:
		std::stack< RAII > _cont;
	};

	inline bool strIsNumber(const std::string& s)
	{
		return !s.empty() && std::find_if(s.begin(), s.end()
			, [](char c) { return !std::isdigit(c); }) == s.end();
	}

	inline bool CheckPortStringToUseable(std::string strInput)
	{
		if (!strIsNumber(strInput))
			return false;
		int PortNumber = std::stoi(strInput);
		if ((PortNumber < 0) || (PortNumber > 65535))
			return false;

		return true;
	}
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
