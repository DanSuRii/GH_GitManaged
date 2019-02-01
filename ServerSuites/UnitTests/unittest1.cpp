#include "stdafx.h"
#include "CppUnitTest.h"

#include <thread>
#include <list>
#include <atomic>

#include <mutex>
#include <vector>
#include "../SimplIOCPServer/types.h"
#include "../SimplIOCPServer/constants.h"
#include "../SimplIOCPServer/BufferPool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			const int MAX_THREAD_CNT = 8;
			const int TRIAL_CNT = 70000;
			std::atomic_int trialCnt(0);

			std::list<std::thread> listThread;
			auto& bufferPool = BufferPool::GetInstance();
			std::atomic_int idxLast(-1);

			for (int i = 0; i < MAX_THREAD_CNT; ++i)
			{
				listThread.emplace_back([&]
				{
					while (++trialCnt < TRIAL_CNT)
					{
						BuffAccessor accessor = bufferPool.GetNewAccessor();

						char* curBuf = bufferPool.GetBuffer(accessor);

						//useage history saves...
					}
				});
			}		

			for (auto& cur : listThread)
			{
				cur.join();
			}
			Assert::IsTrue( bufferPool.AllAvailable() );
			//Assert::AreEqual( bufferPool.GetAvailableCnt() , bufferPool.GetContainerLen() );
		}

	};
}