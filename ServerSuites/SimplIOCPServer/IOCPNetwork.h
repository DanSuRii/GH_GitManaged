#pragma once

#include "network.h"
#include "SocketCtx.h"

namespace NS_DPNET
{ 
	class IOCP : public Network
	{
		IOCP(const IOCP&) = delete;
	public:
		IOCP();
		virtual ~IOCP();

		virtual bool Listen( std::string strPort );
	private:
		void ClearIOCPHandle();
		void ClearAndWaitsThreads();

		void WorkerThread();

		bool bInit = false;
		HANDLE hIOCP;
		//std::stack< std::function< void(void) > > _contFNDestructor;

		void RegDtor(AgentDestructor::_Callable fn);

		AgentDestructor dtorAgt;

		std::vector< std::thread > _threads;
		std::list< std::shared_ptr< class Listener > > _listeners;
	};
	using PIOCP = std::shared_ptr<IOCP>;
}