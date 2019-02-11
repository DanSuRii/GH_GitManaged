#include "pch.h"
#include "SocketCtx.h"

namespace NS_DPNET
{


	SocketCtx::SocketCtx()
	{
		_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == _sock)
		{
			LOG_FN(", socket() failed: ", WSAGetLastError());
			return;
		}
	}

	SocketCtx::SocketCtx(SocketCtx && rhs)
	{
		this->_sock = rhs._sock;
		rhs._sock = INVALID_SOCKET;
	}

	SocketCtx::~SocketCtx()
	{
		ClearSocket();
	}

	bool SocketCtx::ClearSocket()
	{
		if (isValid())
		{
			::closesocket(_sock);
			_sock = INVALID_SOCKET;
		}

		return true;
	}

}
