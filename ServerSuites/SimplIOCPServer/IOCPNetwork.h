#pragma once

#include "network.h"

namespace NS_DPNET
{ 

	class IOCP : public Network
	{
	public:
		IOCP();
		void ClearIOCPHandle();
		void ClearAndWaitsThreads();
		virtual ~IOCP();
	private:
		void WorkerThread();

		bool bInit = false;
		HANDLE hIOCP;
		std::stack< std::function< void(void) > > _contFNDestructor;

		void RegDtor(AgentDestructor::_Callable fn);

		AgentDestructor dtorAgt;

		std::vector< std::thread > _threads;
	};
}