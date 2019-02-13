#include "pch.h"

#include <WinSock2.h>

#include <atomic>

namespace NS_GLOBAL_MESSAGE_DISPATCHER
{
	using ObjectID = unsigned long long;
	
	//is it possible to IOCP use for msg queue that 

	class G_MsgDispatcher
	{
	public:

		static G_MsgDispatcher& GetInstance()
		{
			static G_MsgDispatcher _inst;
			return _inst;
		}

		inline HANDLE GetIOCPHandle() { return hIOCP; }

	private:

		void Workerthread();

		HANDLE hIOCP;
	};

	
	class IOCtx : public OVERLAPPED
	{

	};

	class MsgQueue
	{

	};

	class Object
	{
	public:

		Object()
		{
			idObj = ++ObjIDGenerator;
			//Register to Global Dispatcher for find this object

			//::CreateIoCompletionPort( idObj, GetGIOCPHandle(), , 0)
			//Register global loop for receive message
		}

		~Object()
		{
			//peek message before arrival, 
		}

		void SendMsg(ObjectID idDest)
		{
			//::PostQueuedCompletionStatus(GetGIOCPHandle(), , , );
		}

	private:		
		inline HANDLE GetGIOCPHandle() { return msgDispatcher.GetIOCPHandle(); }

		ObjectID idObj;
		static std::atomic_int64_t ObjIDGenerator;
		G_MsgDispatcher& msgDispatcher = G_MsgDispatcher::GetInstance();
	};

	decltype(Object::ObjIDGenerator) Object::ObjIDGenerator = 0;




	class ICompletionKey : public Object
	{

	};

	class Listener : public ICompletionKey
	{

	};
	
	class ClientCtx : public ICompletionKey
	{

	};

	class IOCP
	{

	};

	class Server : public Object
	{

	};

	class CltInstance : public Object
	{

	};

	class Client : public Object
	{

	};

	void Do()
	{
		
	}
	void G_MsgDispatcher::Workerthread()
	{
		while (true)
		{
			DWORD dwIOSize(0);
			Object* pObj(nullptr);
			IOCtx* pIOCtx(nullptr);

			BOOL bSuccess = ::GetQueuedCompletionStatus(hIOCP, &dwIOSize, (PULONG_PTR)&pObj, (LPOVERLAPPED *)&pIOCtx, INFINITE);

			if (false == bSuccess)
			{

			}

			if (nullptr == pObj)
			{
				break;
			}
		}
	}
}


IMPL_TEST_FNC(GLOBAL_MESSAGE_DISPATCHER)
{
	NS_GLOBAL_MESSAGE_DISPATCHER::Do();
}