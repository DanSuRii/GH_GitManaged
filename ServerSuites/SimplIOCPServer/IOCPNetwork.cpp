#include "pch.h"
#include "IOCPNetwork.h"
#include "AgentDestructor.h"
#include "BufferPool.h"

#include <MSWSock.h>
#include <WS2tcpip.h>
#include <cassert>

#include <typeindex>
#include <unordered_map>

#define LOOP_THROUGH( exp ) for( decltype( exp ) i = 0 ; i < ( exp ) ; ++i ) 

namespace NS_DPNET
{
	class Listener : public ICompletionKey
	{
	public:
		Listener(HANDLE hIocp, std::string strPort);
		~Listener();


		inline bool isInit() { return bInit; }
		inline operator bool() { return isInit(); }
		inline bool IsValid(HANDLE hIocp) {
			return (hIocp != NULL && hIocp != INVALID_HANDLE_VALUE);
		}


	private:
		void Handle(AcceptIO& ioAccept);

		/// it works only firsttime to accept waiting		
		/// possible to one to N accance waiting
		void RegistFirstAccept();

		HANDLE _hIocp;

		SocketCtx sock;
		AgentDestructor agt;

		LPFN_ACCEPTEX pfnAcceptEx;

		PAcceptIO pAcceptIO;

		bool bInit = false;



		// Inherited via ICompletionKey
		virtual void HandleIO(IOCtx * pIoCtx, DWORD dwIOSize) override
		{
			CHECK_RETURN( nullptr == pIoCtx, ", Invalid IOCtx" );
			CHECK_RETURN(std::type_index(typeid(AcceptIO)) != std::type_index(typeid(*pIoCtx)), ", Listener only takes AcceptIO");		

			auto acceptIO = static_cast<AcceptIO*>(pIoCtx);
			Handle( *acceptIO );
		}
	};

	Listener::Listener(HANDLE hIocp, std::string strPort)
		: _hIocp(hIocp)
	{
		CHECK_RETURN(false == IsValid(hIocp), "IOCP parameter invalid: ", hIocp);
		CHECK_RETURN(false == NS_DPUTIL::CheckPortStringToUseable(strPort), "Port parameter invalid: ", strPort);
		CHECK_RETURN(false == sock.isValid(), "listen socket ctx is invalid.");

		struct addrinfo hints { 0 };
		struct addrinfo *addrlocal(nullptr);

		LINGER linger;
		linger.l_onoff = 1;
		linger.l_linger = 0;

		//Resolve the interface
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_IP;

		if (0 != ::getaddrinfo(NULL, strPort.c_str(), &hints, &addrlocal))
		{
			LOG_FN(", getaddrinfo() failed with error: ", WSAGetLastError());
			return;
		}

		if (nullptr == addrlocal)
		{
			LOG_FN(", getaddrinfo() failed to resolve/convert the interface: ", WSAGetLastError());
			return;
		}

		NS_DPUTIL::RAIIStack raiiS;
		raiiS.Push([&] {freeaddrinfo(addrlocal); });

		int nRet(0);

		nRet = ::bind(sock.Get(), addrlocal->ai_addr, (int)addrlocal->ai_addrlen);
		if (SOCKET_ERROR == nRet)
		{
			LOG_FN(", bind() failed: ", WSAGetLastError());
			return;
		}

		nRet = ::listen(sock.Get(), 5);
		if (nRet == SOCKET_ERROR)
		{
			LOG_FN(", listen() failed: ", WSAGetLastError());
			return;
		}

		HANDLE hRes = ::CreateIoCompletionPort((HANDLE)sock.Get(), _hIocp, (DWORD_PTR)this, 0);
		if (NULL == hRes)
		{
			LOG_FN(", CreateIoCompletionPort() failed to join IOCP : ", GetLastError());
			return;
		}

		GUID acceptex_guid = WSAID_ACCEPTEX;
		DWORD dwBytes(0);

		nRet = ::WSAIoctl(
			sock.Get(), SIO_GET_EXTENSION_FUNCTION_POINTER, &acceptex_guid, sizeof(acceptex_guid)
			, &pfnAcceptEx, sizeof(pfnAcceptEx), &dwBytes, NULL, NULL);

		if (nRet == SOCKET_ERROR) {
			LOG_FN(", WSAIoctl() failed to load AcceptEx : ", WSAGetLastError());
			return;
		}

		RegistFirstAccept();

		bInit = true;
	}

	Listener::~Listener()
	{
	}

	/// it works only firsttime to accept waiting		
	/// possible to one to N accance waiting

	void Listener::Handle(AcceptIO & ioAccept)
	{
		ioAccept.GetBufKey();

		SOCKET thisSock = sock.Get();
		int nRet = ::setsockopt( ioAccept.GetSock(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&thisSock, sizeof(thisSock) );
		if (SOCKET_ERROR == nRet)
		{
			LOG_FN();
		}

		//Call back to registered server someone's turn up.

		//ioAccept.GetBufKey();

	}

	void Listener::RegistFirstAccept()
	{
		assert(nullptr == pAcceptIO);
		pAcceptIO = New< decltype(pAcceptIO)::element_type >();

		const DWORD dwAddrLen = sizeof(SOCKADDR_STORAGE) + 16;
		DWORD dwRecvNumBytes(0);

		BufInfo bufInfo = BufferPool::GetInstance().GetBufInfo(pAcceptIO->GetBufKey());

		pfnAcceptEx(sock.Get(), pAcceptIO->GetSock()
			, bufInfo._pBuffer
			, bufInfo._len - (2 * dwAddrLen)
			, dwAddrLen, dwAddrLen
			, &dwRecvNumBytes
			, pAcceptIO.get());
	}

	class SystemInfo
	{
		SystemInfo()
		{
			::GetSystemInfo(&_sysinfo);
		}

	public:
		DWORD GetCpuCnt() { return _sysinfo.dwNumberOfProcessors; }
		static SystemInfo & GetInstance() {
			static SystemInfo _instance;
			return _instance;
		}
	private:
		SYSTEM_INFO	_sysinfo;
	}&gSysInfo(SystemInfo::GetInstance());


	IOCP::IOCP()
	{
		hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		if (NULL == hIOCP)
		{
			LOG_FN(", CreateIoCompletionPort() failure: ", ::GetLastError());
			return;
		}
		RegDtor([&] { ClearIOCPHandle(); });

		DWORD cpuCnt = gSysInfo.GetCpuCnt();
		for (decltype(cpuCnt) i = 0; i < cpuCnt; ++i)
		{
			_threads.emplace_back(&IOCP::WorkerThread, this);
		}

		RegDtor([&] { ClearAndWaitsThreads(); });

		eState = ESTAT_AVAILABLE;
		RegDtor([&] { eState = ESTAT_DESTROY; });
		bInit = true;
	}

	void IOCP::ClearIOCPHandle()
	{
		if (NULL != hIOCP) {
			CloseHandle(hIOCP);
			hIOCP = NULL;
		}
	}

	void IOCP::ClearAndWaitsThreads()
	{
		for (decltype(_threads.size()) i = 0; i < _threads.size(); ++i)
			::PostQueuedCompletionStatus(hIOCP, 0, NULL, nullptr);

		for (auto& cur : _threads)
		{
			cur.join();
			std::stringstream strToOut;
			strToOut << "Thread " << cur.get_id() << " was been Joined() \n";
			OutputDebugStringA( strToOut.str().c_str() );
		}
	}

	IOCP::~IOCP()
	{
		/*
		while (0 < _contFNDestructor.size())
		{
			auto cur = _contFNDestructor.top();
			cur();
			_contFNDestructor.pop();
		}
		*/
		dtorAgt.Destruct();
	}

	bool IOCP::Listen(std::string strPort)
	{
		//if portnumber already uses, do not create listener
		auto pListener = New<Listener>(hIOCP, strPort);

		if (nullptr == pListener || false == (*pListener))
		{
			return false;
		}

		_listeners.push_back(pListener);

		return true;
	}

	void IOCP::WorkerThread()
	{
		while (true)
		{
			DWORD dwIOSize(0);
			ICompletionKey* pCtxt(nullptr);
			IOCtx* pOverlapped(nullptr);
			BOOL bSuccess = ::GetQueuedCompletionStatus(hIOCP, &dwIOSize, (PULONG_PTR)&pCtxt, (LPOVERLAPPED*)&pOverlapped, INFINITE);
			if (FALSE == bSuccess) {
				LOG_FN(",GetQueuedCompletionStatus() failed: ", WSAGetLastError());
			}

			dwIOSize, pCtxt, pOverlapped;

			if (nullptr == pCtxt)
			{
				LOG_FN( ", ThID: ", std::this_thread::get_id() , ", occured break. Exit thread" );
				break;
			}

			//HandleClientExit
			{
				if (std::type_index(typeid(AcceptIO)) != std::type_index(typeid(*pOverlapped)))
				{
					if (!bSuccess || (bSuccess && (0 == dwIOSize)))
					{
						//TODO: remove this context, if not ... it will be leak :)
						continue;
					}
				}
			}

			pCtxt->HandleIO(pOverlapped, dwIOSize);

		}
	}

	void IOCP::RegDtor(AgentDestructor::_Callable fn)
	{
		dtorAgt.Reg(fn);
	}

} //NS_DPNET