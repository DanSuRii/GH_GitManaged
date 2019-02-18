#include "pch.h"
#include "ClientContext.h"

namespace NS_DPNET
{
	ClientCtx::CltMsgDispatcher ClientCtx::msgDispatcher;
	ClientCtx::CltMsgDispatcher::CltMsgDispatcher()
	{
		if (false == bInit)
		{
			std::lock_guard< decltype(mtxInit) > grd(mtxInit);
			if (false == bInit)
			{
				Regist<WriteIO>();
				Regist<ReadIO>();
			}
		}
	}


	void ClientCtx::Handle(WriteIO & ioWrite, DWORD dwIOSize)
	{
		LOG_FN(", was been occurred");				

		if (false == ioWrite.HandleIO(dwIOSize))
		{
			ioWrite.GetBufKey();
		}
	}

	void ClientCtx::Handle(ReadIO & ioRead, DWORD dwIOSize)
	{
		LOG_FN(", was been occurred");

		// MessageDispatcher.Send( this->SvrID this->GetID() GetMsg( ioRead.GetBufKey() ) );
				
	}

	ClientCtx::ClientCtx(HANDLE hIOCPtoJoin, SocketCtx&& sockCtx)
		: _sockCtx( std::move( sockCtx ) )
	{
		HANDLE hIOCP = ::CreateIoCompletionPort((HANDLE)_sockCtx.Get(), hIOCPtoJoin, (ULONG_PTR)this, 0 );
		if (NULL == hIOCP)
		{
			LOG_FN(", CreateIoCompletionPort() failed: %d\n", GetLastError());
			return;
		}

		bInit = true;
	}
	
	// Inherited via ICompletionKey

	void ClientCtx::HandleIO(IOCtx * pIOCtx, DWORD dwIOSize)
	{
		CHECK_RETURN(nullptr == pIOCtx, "nullptr failed");
		auto fn = GetHandlerFn(typeid(*pIOCtx));
		if (fn) {
			fn(this, pIOCtx, dwIOSize);
		}
		else {
			LOG_FN( ", Handler does not exists" );
		}

	}

}
