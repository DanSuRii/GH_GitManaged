#pragma once

#include "network.h"

namespace NS_DPNET
{ 
	class IOCP : public Network
	{
	public:
		IOCP();
		virtual ~IOCP();
	private:

		HANDLE hIOCP;
	};
}