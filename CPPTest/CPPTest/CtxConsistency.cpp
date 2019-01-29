#include "pch.h"
#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib" )

#include <memory>
#include <iostream>

#include <vector>
#include <thread>
#include <queue>
#include <list>

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

#if false // it has no idea whats live in 
class IBucket
{
public:
	IBucket(int nID)
		: _nID(nID)
	{

	}


private:
	int _nID = -1;
};
using PIBucket = std::shared_ptr<IBucket>;

//POOLER for IOPool or BufferPool
//It doesnt handle Client or somthing another, resources are never protect against asynchronous access.
class POOLER
{
	const size_t DEFAULT_POOL_LEN = 256;
	POOLER()
	{

	}
public:

	//create on access, dosnt need to populate at creation
	PIBucket GetBucket(int idx)
	{
		if (idx < 0 || idx >= pool.size())
			return nullptr;

		if (pool[idx] == nullptr)
		{
			//Create from Request? it need to ..creation information
		}
	}

	static POOLER& GetInstance() 
	{
		static POOLER _inst;
		return _inst;
	}
private:
	void Recycle(int idx)
	{

	}
	//never removed in here, just for a reuse waiting
	//and... this also doesnt need to Synchronise 
	std::vector< PIBucket > pool{ DEFAULT_POOL_LEN };

	//This just need to Synchronise. it doesnt need to search or loop.
	std::list< unsigned int > available;
} & g_pool = POOLER::GetInstance();

enum EBucketTimes
{
	EBT_CREATIONAL,
	EBT_LASTRECYCLED,
	EBT_LASTMODIFIED,

	EBT_CNT,
};

template< class T >
class Bucket : public IBucket
{
public:
	Bucket() {};
	operator T&() { Access(); }
	T& Access() { return _data; }
private:
	T _data;

};

void ClientCode()
{
}

#endif // false

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

//file io, network io, and...


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
using PIOCP = std::shared_ptr<IOCP>;

//make it pooling, BUFFER or 



template<class Obj>
class POOL
{
public:
	const int DEFAULT_CONTAINER_CNT;
	using PObj = std::shared_ptr<Obj>;
	
	class Bucket
	{
	public:
		Bucket( int pos )
			: _pos(pos)
		{
		}
	private:
		int _pos;
		PObj _data; //possible to char _data[sizeof(Obj)]; new(_data) Obj();
	};
	using PBucket = std::shared_ptr< Bucket >;

	static POOL& GetInstance() {
		static POOL _instance;
		return _instance;
	}
private:
	POOL()
	{
		for (int i = 0; i < _cont.size(); ++i)
		{
			_cont[i] = std::make_shared<Bucket>(i);
			_available.push_back(i);
		}
	};

	void _grow_up()
	{
		size_t oldSize = _cont.size();
		_cont.resize(_cont.size() * 2);

		for (size_t sz = oldSize; sz < _cont.size(); ++sz)
		{
			_available.push_back(sz);
		}
	}

	std::vector<PBucket> _cont{ DEFAULT_CONTAINER_CNT };

	std::list<int> _available;
};

POOL<IOCtx>::PObj NewIOctx()
{

}



void ClientCode()
{
}



IMPL_TEST_FNC(CTX_CONSISTENCY)
{
	//POOLER pooler;
	IOCP iocp(8);
	std::vector<int> vecInt{ 1,2,3 };
	vecInt.push_back(3);
}