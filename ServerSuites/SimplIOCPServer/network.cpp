#include "pch.h"
#include "network.h"

#pragma comment(lib, "ws2_32.lib")

namespace NS_DPNET
{
	//private class WSAInitializer
	//once per system
	class WSAInit
	{
		WSAInit()
		{
			int nRet = ::WSAStartup(MAKEWORD(2, 2), &_wsaData);
			if (nRet != 0)
			{
				LOG_FN(", WSAStartup() failed: ", nRet);
				return;
			}

			_bInit = true;
		}
	public:
		static WSAInit& GetInstance()
		{
			static WSAInit _instance;
			return _instance;
		}

		bool isInit() { return _bInit; }
	private:
		bool _bInit = false;

		WSADATA _wsaData;

	}&gInit = WSAInit::GetInstance();

	Network::Network()
	{
	}


	Network::~Network()
	{
	}
}
