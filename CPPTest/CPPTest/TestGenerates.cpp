#include "pch.h"
#include <thread>
#include <vector>

#include <algorithm>

template< class ...Args >
void Log( Args... args )
{
	const int a[]{ (std::cout << args , 0) ... };
	static_cast<void>(a);

	std::cout << std::endl;
}

class IOCP
{
public:
	void WorkerThread()
	{
		Log(_threadCnt);
	}

	IOCP(size_t threadcnt)
		: _threadCnt(threadcnt)
	{
		std::generate_n(_threads.end(), threadcnt, [&] { return std::thread{ &IOCP::WorkerThread, this }; });
	}

	virtual ~IOCP()
	{
		for (auto& cur : _threads)
			cur.join();
	}
private:
	size_t _threadCnt;
	std::vector< std::thread > _threads;
};


IMPL_TEST_FNC(TEST_GENERATES)
{
	const int thread_cnt(8);

	if(false)
		IOCP iocp(thread_cnt);

	Log(__FUNCSIG__ ", Failed to execute this Generate have problem, "
		" it will doenst generate or emplace back");
}
