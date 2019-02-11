#include "stdafx.h"
#include "CppUnitTest.h"

#include <algorithm>

#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>

#include <list>
#include <vector>
#include <queue>

#include "../SimplIOCPServer/types.h"
#include "../SimplIOCPServer/constants.h"
#include "../SimplIOCPServer/BufferPool.h"

#include "../SimplIOCPServer/Logger.h"

#include <random>

#include <fstream>

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
			//const int TRIAL_CNT = 700000000;
			const int TRIAL_CNT = 700000;
			std::atomic_int trialCnt(0);

			std::list<std::thread> listThread;
			auto& bufferPool = BufferPool::GetInstance();

			const char const * pStrs[]{
				"	8Q77AIFCqjdD1Ly5e6aq	",
				"	ccZD3iqWiKMaGGfFjzc3	",
				"	mMux9toqKtDSmlhLqx8k	",
				"	lKOXNm4VzYs3O48KFVoP	",
				"	fOLix3P6TaUjqanyIVtQ	",
				"	DZ4ho30NLaLaYpANvXBb	",
				"	btxHhkoYdfPVn2eONXsl	",
				"	5NppFIBdYsospPnVqTRr	",
				"	1QCQi7LhkphZMocOWjSH	",
				"	ia3YAB3KURYceLPMUj2v	",
				"	LSNWiyGzdFRi4i2JTz23	",
				"	d7BjAvqNHAk5qJDys3UZ	",
				"	LtblMFMfSIk0wdiR3VpR	",
				"	mndSbaqFQS3bWtBm2yjr	",
				"	tLdRsfBJ79Owb6XRzE6i	",
				"	lhe5oxDwkDmPKFFRyWv7	",
				"	22qbcUHDoaAQHpBxMj1f	",
				"	3VfzKi4h1dT1y4KDnbhY	",
				"	MtRe6SHencJZNu1azuKm	",
				"	2GBdYamRVMs6NYNf6zug	",
				"	FNIqs8Wn2AiHiTNCT4Fx	",
				"	uZ1BasFBjA4h45Er4Nv9	",
				"	L1WtnLbXWon0aIb402cK	",
				"	mrlFsCprKhKXZJiPNYlO	",
				"	5PRoq1seCUNdGxzGY9WM	",
				"	7eeoH3PaVblD2AXvTL58	",
				"	EMVZF9GnBxACzVflxgpI	",
				"	B8PEBFFeqv8kGRwEcJqL	",
				"	5QNsjOqttJdaFJYjTBeM	",
				"	2TtaxDPK9OCXPRYdN04E	",
			};


			for (int i = 0; i < MAX_THREAD_CNT; ++i)
			{
				listThread.emplace_back([&]
				{
					unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::mt19937_64 generator(seed);
					while (++trialCnt < TRIAL_CNT)
					{
						{
							BuffAccessor accessor = bufferPool.GetNewAccessor();

							char* curBuf = bufferPool.GetBuffer(accessor);

							const char * toWrite = pStrs[generator() % _countof(pStrs)];

							std::strcpy( curBuf, toWrite );
						}

						std::this_thread::yield();
						
						//useage history saves...
						//Sends aways msgs for somewhere					
					}
				});
			}		

			for (auto& cur : listThread)
			{
				cur.join();
			}

			auto availableCnt = bufferPool.GetAvailableCnt();
			Assert::IsTrue( bufferPool.AllAvailable() );
			//Assert::AreEqual( bufferPool.GetAvailableCnt() , bufferPool.GetContainerLen() );
		}

	};

	TEST_CLASS(LOG_THROUPUT_TEST)
	{
		TEST_METHOD(Method1)
		{
			const int MAX_THREAD_COUNT = 8;
			const char const * pStrs[]{
	"	8Q77AIFCqjdD1Ly5e6aq	",
	"	ccZD3iqWiKMaGGfFjzc3	",
	"	mMux9toqKtDSmlhLqx8k	",
	"	lKOXNm4VzYs3O48KFVoP	",
	"	fOLix3P6TaUjqanyIVtQ	",
	"	DZ4ho30NLaLaYpANvXBb	",
	"	btxHhkoYdfPVn2eONXsl	",
	"	5NppFIBdYsospPnVqTRr	",
	"	1QCQi7LhkphZMocOWjSH	",
	"	ia3YAB3KURYceLPMUj2v	",
	"	LSNWiyGzdFRi4i2JTz23	",
	"	d7BjAvqNHAk5qJDys3UZ	",
	"	LtblMFMfSIk0wdiR3VpR	",
	"	mndSbaqFQS3bWtBm2yjr	",
	"	tLdRsfBJ79Owb6XRzE6i	",
	"	lhe5oxDwkDmPKFFRyWv7	",
	"	22qbcUHDoaAQHpBxMj1f	",
	"	3VfzKi4h1dT1y4KDnbhY	",
	"	MtRe6SHencJZNu1azuKm	",
	"	2GBdYamRVMs6NYNf6zug	",
	"	FNIqs8Wn2AiHiTNCT4Fx	",
	"	uZ1BasFBjA4h45Er4Nv9	",
	"	L1WtnLbXWon0aIb402cK	",
	"	mrlFsCprKhKXZJiPNYlO	",
	"	5PRoq1seCUNdGxzGY9WM	",
	"	7eeoH3PaVblD2AXvTL58	",
	"	EMVZF9GnBxACzVflxgpI	",
	"	B8PEBFFeqv8kGRwEcJqL	",
	"	5QNsjOqttJdaFJYjTBeM	",
	"	2TtaxDPK9OCXPRYdN04E	",
			};


			std::vector< std::thread > threads;
			for (int i = 0; i < MAX_THREAD_COUNT; ++i) {
				threads.emplace_back( [&] 
				{
					int nTryCnt = 70000;

					unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::mt19937_64 generator(seed);


					while(--nTryCnt)
					{
						LOG_FN(pStrs[generator() % _countof(pStrs)], pStrs[generator() % _countof(pStrs)], pStrs[generator() % _countof(pStrs)]);
						std::this_thread::yield();
					}

				});
			}

			for (auto& cur : threads)
			{
				cur.join();
			}

		}
	};
}