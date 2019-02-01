#include "pch.h"

namespace NS_DPUTIL
{
	void RAIIStack::Push(RAII::_Callable fn)
	{
		_cont.emplace(fn);
	}
	RAIIStack::~RAIIStack()
	{
		//there are no information for std::stack::destructor pop elements LIFO
		//, then just simply pop it all
		while( _cont.size() > 0){ 
			_cont.pop();
		}
	}
}
