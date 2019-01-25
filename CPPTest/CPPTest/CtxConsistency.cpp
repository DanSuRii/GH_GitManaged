#include "pch.h"
#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib" )

#include <memory>
#include <iostream>

#include <vector>
#include <thread>

template< class T, class ...Args >
auto New( Args&& ...args )
{
	return std::make_shared<T>(args...);
}

template<class ...Args>
void Log(Args&& ...args)
{
	const int a[]{ ( std::cout << args, 0 ) ... };
	static_cast<void>(a);

	std::cout << std::endl;

	return;
}

#define LOG_FN( ... )  Log( __FUNCSIG__ , __VA_ARGS__ )


class WSAInitializer 
{
	WSAInitializer()
	{
		WSAStartup(MAKEWORD(2, 2), &_wsa);
	}
public:
	virtual ~WSAInitializer()
	{
		WSACleanup();
	}

	static WSAInitializer& GetInstance() 
	{
		static WSAInitializer _inst;
		return _inst;
	}
private:
	WSADATA _wsa;
}& g_wsainit = WSAInitializer::GetInstance();

class IOCtx : OVERLAPPED
{
public:
private:
};
using PIOCtx = std::shared_ptr<IOCtx>;


class ClientCtx
{
public:
private:
	
};

class IOCP
{
public:
	

	IOCP( size_t threadCnt )
		:hIocp( ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 ) )
		, _threadCnt( threadCnt )
	{
		for (size_t i = 0; i < _threadCnt; ++i)
			_threads.emplace_back( &IOCP::WorkerThread, this );
	}

	virtual ~IOCP()
	{
		for( size_t i = 0 ; i < _threads.size(); ++i)		
			PostQueuedCompletionStatus(hIocp, 0, 0, NULL);

		for (auto& cur : _threads)
			cur.join();
	}
private:
	void WorkerThread()
	{
		while (true)
		{
			DWORD dwTransffered(0);
			ULONG_PTR pCompletionKey;
			OVERLAPPED *pOverlapped;
			bool bSuccess = ::GetQueuedCompletionStatus(hIocp, &dwTransffered, &pCompletionKey, &pOverlapped, INFINITE);

			if( !bSuccess ){
				LOG_FN( "GetQueuedCompletionStatus() failed: ", GetLastError() );
			}

			if (NULL == pCompletionKey) {
				//it means, break the thread request
				break;
			}
		}

		LOG_FN("Exited Thread(", GetCurrentThreadId(), ")");
	}

	HANDLE hIocp;

	size_t _threadCnt = 0;
	std::vector< std::thread >	_threads;
};

IMPL_TEST_FNC(CTX_CONSISTENCY)
{
	IOCP iocp(8);
}