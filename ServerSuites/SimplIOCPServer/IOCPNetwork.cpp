#include "pch.h"
#include "IOCPNetwork.h"
#include "AgentDestructor.h"

#include <MSWSock.h>

#define LOOP_THROUGH( exp ) for( decltype( exp ) i = 0 ; i < exp ; ++i ) 

namespace NS_DPNET
{
	class SocketCtx
	{
	public:
		SocketCtx()
		{
			_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if( INVALID_SOCKET == _sock )
			{
				LOG_FN( ", socket() failed: ", WSAGetLastError() );
				return ;
			}

		}
		~SocketCtx()
		{
			ClearSocket();
		}

		bool isValid() { return _sock != INVALID_SOCKET; }
		bool ClearSocket()
		{
			if (isValid())
			{
				::closesocket(_sock);
				_sock = INVALID_SOCKET;
			}
		}

		SOCKET Get() { return _sock; }
	private:
		SOCKET _sock;
	};

	class ICompletionKey
	{
	public:
	};

	class ClientCtx : public ICompletionKey
	{

	};

	class RecvBuffer 
	{
	};

	class IOCtx : public OVERLAPPED
	{

	};

	class AcceptIO : public IOCtx
	{
	public:
	private:
		SocketCtx ctx;
	};

	class Listener : public ICompletionKey
	{
	public:
		Listener( HANDLE hIocp );
		~Listener();


		inline bool isInit() { return bInit; }
		inline operator bool() { return isInit(); }
	private:
		HANDLE _hIocp;
		
		SocketCtx sock;
		AgentDestructor agt;

		LPFN_ACCEPTEX pfnAcceptEx;

		bool bInit = false;
	};

	Listener::Listener( HANDLE hIocp )
		: _hIocp(hIocp)
	{
		if (false == sock.isValid()) {
			LOG_FN(", listen socket ctx is invalid.");
			return;
		}

		int nRet = ::listen(sock.Get(), 5);
		if( nRet == SOCKET_ERROR )
		{ 
			LOG_FN( ", listen() failed: ", WSAGetLastError() );
			return;
		}

		HANDLE hRes = ::CreateIoCompletionPort( (HANDLE)sock.Get(), _hIocp, (DWORD_PTR)this, 0 );
		if (NULL == hRes)
		{
			LOG_FN(", CreateIoCompletionPort() failed to join IOCP : ", GetLastError());
			return;
		}

		GUID acceptex_guid = WSAID_ACCEPTEX;
		DWORD dwBytes(0);

		nRet = ::WSAIoctl(
			sock.Get(), SIO_GET_EXTENSION_FUNCTION_POINTER, &acceptex_guid, sizeof(acceptex_guid)
			, &pfnAcceptEx, sizeof(pfnAcceptEx), &dwBytes, NULL, NULL );

		if (nRet == SOCKET_ERROR) {
			LOG_FN(", WSAIoctl() failed to load AcceptEx : ", WSAGetLastError());
			return;
		}

		bInit = true;
	}

	Listener::~Listener()
	{
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
	}& gSysInfo( SystemInfo::GetInstance() );


	IOCP::IOCP()
	{
		hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0 );
		if( NULL == hIOCP )
		{
			LOG_FN( ", CreateIoCompletionPort() failure: ", ::GetLastError() );
			return;
		}
		RegDtor([&] { ClearIOCPHandle(); });

		DWORD cpuCnt = gSysInfo.GetCpuCnt();
		for(decltype(cpuCnt) i = 0; i < cpuCnt; ++i)
		{
			_threads.emplace_back( &IOCP::WorkerThread, this );
		}
		
		RegDtor([&] { ClearAndWaitsThreads(); });

		


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
		}
	}

	IOCP::~IOCP()
	{
		while (0 < _contFNDestructor.size())
		{
			auto cur = _contFNDestructor.top();
			cur();
			_contFNDestructor.pop();
		}
	}

	void IOCP::WorkerThread()
	{
		while(true)
		{
			DWORD dwIOSize(0);
			ClientCtx* pCtxt(nullptr);
			OVERLAPPED* pOverlapped(nullptr);
			BOOL bSuccess = ::GetQueuedCompletionStatus(hIOCP, &dwIOSize, (PULONG_PTR)&pCtxt, &pOverlapped, INFINITE);
			if (FALSE == bSuccess) {
				LOG_FN(",GetQueuedCompletionStatus() failed: ", WSAGetLastError() );
			}
		
		}
	}

	void IOCP::RegDtor(AgentDestructor::_Callable fn)
	{
		dtorAgt.Reg(fn);
	}

}