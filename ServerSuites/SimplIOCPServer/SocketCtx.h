#pragma once

namespace NS_DPNET
{


	class SocketCtx
	{
	public:
		SocketCtx();
		//We deleted copy constructor, although there is possible to Get() method after closesocket()
		SocketCtx(SocketCtx& rhs) = delete;
		SocketCtx(SocketCtx&& rhs);

		~SocketCtx();

		bool isValid() { return _sock != INVALID_SOCKET; }
		bool ClearSocket();

		SOCKET Get() { return _sock; }
	private:
		SOCKET _sock;
	};

}
