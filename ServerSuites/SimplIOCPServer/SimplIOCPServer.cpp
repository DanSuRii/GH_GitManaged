// SimplIOCPServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "network.h"
#include "IOCPNetwork.h"

class Server
{
public:
	Server()
	{
		network = New<NS_DPNET::IOCP>();
	}

	void Run() {
		if (network->GetStatus() != NS_DPNET::Network::ESTAT_AVAILABLE)
		{
			LOG_FN( ", failed to start: ", network->GetStatus() );
			return;
		}
		network->Listen("38001");
	};

private:
	NS_DPNET::PNetwork network;
};

int main()
{
	Server server;
	server.Run();

	return 0;
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
