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

		bool Listen( std::string strPort );
	private:
		void WorkerThread();

		bool bInit = false;
		HANDLE hIOCP;
		std::stack< std::function< void(void) > > _contFNDestructor;

		void RegDtor(AgentDestructor::_Callable fn);

		AgentDestructor dtorAgt;

		std::vector< std::thread > _threads;
		std::list< std::shared_ptr< class Listener > > _listeners;
	};
}