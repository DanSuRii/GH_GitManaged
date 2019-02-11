#pragma once

#include "SocketCtx.h"
#include "BufferPool.h"


namespace NS_DPNET
{
	class IOCtx : public OVERLAPPED
	{
	public:
		EIOTyp GetTyp();

		//virtual void RequestHandleSelf(ICompletionKey& Receiver) = 0;
	};

/*
	template< class Derived  >
	class IOCtx_CRTP : public IOCtx
	{
	public:
		// Inherited via IOCtx
		virtual void RequestHandleSelf(ICompletionKey & Receiver) override
		{
			Receiver.HandleIO(static_cast<Derived*>(this));
		}
	};
*/

	class AcceptIO : public IOCtx
	{
	public:
		AcceptIO();

		SOCKET GetSock() { return ctx.Get(); }
		inline BuffAccessor GetBufKey() { return bufKey; }
	private:
		SocketCtx ctx;
		BuffAccessor bufKey = BufferPool::GetInstance().GetNewAccessor();
	};
	using PAcceptIO = std::shared_ptr< AcceptIO >;

	class ReadIO : public IOCtx
	{
	public:
	};

	class WriteIO : public IOCtx
	{
	public:
	};

}