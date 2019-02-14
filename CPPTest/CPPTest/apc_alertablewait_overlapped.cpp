#include "pch.h"

#include <winsock2.h>

#include <thread>
#include <array>
#include <algorithm>

#include <atomic>

namespace  NS_APC_ALERTABLEWAIT_OVERLAPPED
{
	template< class ... Args >
	void Log(Args&& ...args)
	{
		int a[]{ (std::cout << args, 0) ... };
		static_cast<void>(a);

		std::cout << std::endl;
	}

#define LOG_FN( ... ) Log( __FUNCSIG__, __VA_ARGS__ )


	class RAII
	{
	public:
		using _Callable = std::function<void(void)>;

		RAII(_Callable rhs)
			: _fn(rhs)
		{
			std::tuple<int, double, int> arg;
		}

		~RAII()
		{
			if (_fn) _fn();
		}
	private:
		_Callable _fn;
	};
	
	using ObjectID = std::int64_t;

	class Object
	{
	public:
		Object()
			: objID(++ObjIDGenerator)
		{

		}

		auto GetObjID() { return objID; }
	private:
		ObjectID objID;
		static std::atomic_int64_t ObjIDGenerator;
	};
	decltype(Object::ObjIDGenerator) Object::ObjIDGenerator = 0;

	void WorkerThread(HANDLE iocp)
	{
		while (true)
		{
			DWORD dwTransferred(0);
			ULONG_PTR lpCompletionKey(0);
			LPOVERLAPPED overlapped(nullptr);

			BOOL bSuccess = ::GetQueuedCompletionStatus(iocp, &dwTransferred, &lpCompletionKey, &overlapped, INFINITE);

			if (FALSE == bSuccess)
			{
				LOG_FN("An error occured from call GetQueuedCompletionStatus() : ", GetLastError());
			}

			if (NULL == lpCompletionKey)
			{
				LOG_FN( ", ", std::this_thread::get_id(),  ">> Detected exit");

				break;
			}

			Object* pCur = (Object*)lpCompletionKey;
			std::cout << pCur->GetObjID() << std::endl;
		}
	}

	

	void Do()
	{
		const int THREAD_CNT = 8;

		HANDLE hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

		//std::array<std::thread, THREAD_CNT> arr;

		std::thread arrThread[THREAD_CNT];
		std::generate(std::begin(arrThread), std::end(arrThread), [&] { return std::thread(WorkerThread, hIOCP); });


		RAII grd([&] {
			BOOL arrRes[sizeof(arrThread)];
			std::generate(std::begin(arrRes), std::end(arrRes), [&] {return ::PostQueuedCompletionStatus(hIOCP, 0, NULL, NULL); });

			for (auto& cur : arrThread)
			{
				cur.join();
			}
		});
		Object obj;
		//OVERLAPPED 

		HANDLE hIOCPRes = ::CreateIoCompletionPort(NULL, hIOCP, (ULONG_PTR)&obj, 0);		

		if (INVALID_HANDLE_VALUE == hIOCPRes || NULL == hIOCPRes)
		{
			LOG_FN( "IOCompletionPort Join Failed : ", hIOCPRes  );
			return;
		}

		OVERLAPPED overlapped;

		::PostQueuedCompletionStatus(hIOCP, -1, (ULONG_PTR)&obj, &overlapped);

	}
}

IMPL_TEST_FNC(APC_ALERTABLEWAIT_OVERLAPPED)
{
	NS_APC_ALERTABLEWAIT_OVERLAPPED::Do();
}