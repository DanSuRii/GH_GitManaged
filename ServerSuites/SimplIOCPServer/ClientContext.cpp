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
	}
	void ClientCtx::Handle(ReadIO & ioWrite, DWORD dwIOSize)
	{
		LOG_FN(", was been occurred");
	}


	ClientCtx::ClientCtx()
	{
	}
	
	// Inherited via ICompletionKey

	void ClientCtx::HandleIO(IOCtx * pIOCtx, DWORD dwIOSize)
	{
		CHECK_RETURN(nullptr == pIOCtx, "nullptr failed");
		auto fn = GetHandlerFn(typeid(*pIOCtx));
		fn(this, pIOCtx, dwIOSize);
	}

}
