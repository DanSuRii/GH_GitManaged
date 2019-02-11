// TestConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <vector>
#include <thread>
#include <random>
#include <queue>

#include <list>
#include <mutex>
#include <atomic>
#include <string>
#include <sstream>

#include "../SimplIOCPServer/Logger.h"

int main()
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
		threads.emplace_back([&]
		{
			int nTryCnt = 70000;

			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::mt19937_64 generator(seed);


			while (--nTryCnt > 0 )
			{
				LOG_FN( pStrs[generator() % _countof(pStrs)], pStrs[generator() % _countof(pStrs)], pStrs[generator() % _countof(pStrs)]);
				std::this_thread::yield();
			}

		});
	}

	for (auto& cur : threads)
	{
		cur.join();
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
