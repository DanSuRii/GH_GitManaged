#include "pch.h"
#include "FIOCP.h"


const int MAX_FILE_IOCP_WORKER = 2;


namespace NS_FIOCP
{
#if 0
	template< class ...Args >
	void Log(Args && ... args)
	{
		const int a[]{ (std::cout << args, 0) ... };
		static_cast<void>(a);
	}

#endif // 0

	class FileCtx
	{
	public:
		HANDLE hFile;

		std::string strFilePath;
	private:
	};

	class ICompletionKey
	{

	};

	class IOCtx : public OVERLAPPED
	{
	};
}




FIOCP::FIOCP()
{
	hIocp = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0  );

	for (int i = 0; i < MAX_FILE_IOCP_WORKER; ++i)
	{
		_workers.emplace_back( std::bind( &FIOCP::WorkerThread, this ) );
	}
}


FIOCP::~FIOCP()
{
	CloseHandle(hIocp);
}

void FIOCP::WorkerThread()
{
	while (true)
	{
		DWORD dwBytesTransferred(0);
		NS_FIOCP::ICompletionKey* pCompletionKey(nullptr);
		NS_FIOCP::IOCtx* pIoctx(nullptr);
		
		bool bSuccess = ::GetQueuedCompletionStatus(hIocp, &dwBytesTransferred, (PULONG_PTR)&pCompletionKey, (LPOVERLAPPED*)&pIoctx, INFINITE);
		if (false == bSuccess)
		{
			LOG_FN(", GetQueuedCompletionStatus() failed: ", GetLastError());
		}

		if (nullptr == pCompletionKey)
		{
			LOG_FN(", occured Exit thread: ", std::this_thread::get_id());
			break;
		}		
	}
}
