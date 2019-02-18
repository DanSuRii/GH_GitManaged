// SimplIOCPServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "network.h"
#include "IOCPNetwork.h"

namespace NS_DPSERVER
{
	class Service
	{

	};

	class Services
	{
	public:
		std::list<Service> services;
	};

	class Server
	{
	public:
		Server()
		{
			network = New<NS_DPNET::IOCP>();
		}

		~Server()
		{
			_myThread.join();
		}

		void Run() {
			if (network->GetStatus() != NS_DPNET::Network::ESTAT_AVAILABLE)
			{
				LOG_FN(", failed to start: ", network->GetStatus());
				return;
			}
			network->Listen("38001");
		};

	private:

		void WorkerThread()
		{
			while (false == bQuitFlag)
			{
				//TODO: Peek the MsgBox -> Process
				std::this_thread::yield();
			}
		}

		NS_DPNET::PNetwork network;

		void PostQuit() { bQuitFlag = true; }

		std::atomic_bool bQuitFlag = false;

		std::thread _myThread;
		
		class PostBox : public NS_DPNET::ICBPostbox
		{
			// Inherited via ICBPostbox
			virtual void Post(NS_DPNET::ClientID cltID, char * pBuf, size_t len) override
			{
				if (len < sizeof(Msg::MsgHeader))
				{
					LOG_FN( ", Impossible to handle msg size is smaller than MsgHeader" );
					return;
				}

				Msg::MsgHeader* pCurMsg = reinterpret_cast<Msg::MsgHeader*>(pBuf);
				pCurMsg->_type;
				//Relate msg or solo msg

				//if pCurMsg->len() <>  typeInfo[ pcurMsg->Type() ]._len 
				//return Invalid Packet: size does not matched
			}

		public:
			/*std::queue< IMsgs >*/
		};

		//std::queue<  >
	};


	class CltInstance
	{
	public:
		using ClientID = int;
	private:
	};
}

namespace NS_DPCLIENT
{
	class Client
	{
	};
}

int main()
{
	auto accessor = BufferPool::GetInstance().GetNewAccessor();
	auto accessorMoved(std::move(accessor));

	auto network = New<NS_DPNET::IOCP>();

	//Read Configuration
	//Generate Server Sets

	NS_DPSERVER::Server server;
	server.Run();

	//wait for user command to quit or somthing another...


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
