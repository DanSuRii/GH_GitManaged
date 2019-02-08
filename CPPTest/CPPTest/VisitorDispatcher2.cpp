#include "pch.h"
#include <winsock2.h>
#include <unordered_map>
#include <typeindex>

#include <cassert>

//Ignore whether it SharedPtr reference counts

template<class T, class ... Args>
auto New(Args&&... args)
{
	return std::make_shared< T >(args ...);
}


namespace NS_VDIS2
{

	class IOCtx : public OVERLAPPED
	{

	};

	using PIOCtx = std::shared_ptr< IOCtx >;

	class AcceptIO : public IOCtx
	{
	};

	class ReadIO : public IOCtx
	{

	};

	class WriteIO : public IOCtx
	{

	};

	class Visitor
	{
	public:
		template< class T >
		void accept(T* rhs)
		{
			auto newObj = std::make_shared<T>(*rhs);

			return;
		}
	};

	class ICompletionKey
	{
	public:
		virtual void doVisit(Visitor&) = 0;
		virtual void HandleMsg(class IOCtx* rhs) = 0;
	};

	using PICompKey = std::shared_ptr< ICompletionKey >;

	template< class T >
	std::shared_ptr<T> assigner()
	{
		return std::make_shared< T >();
	}

	class IMsgInvoker
	{
	public:
		virtual void Invoke(IOCtx*) = 0;
	};
	using PIMsgInvoker = std::shared_ptr<IMsgInvoker>;

	template< class TCaller, class _Arg >
	class MsgInvoker : public IMsgInvoker
	{
	public:
		using PTCaller = std::shared_ptr<TCaller>;
		using PArg = std::shared_ptr<_Arg>;

		MsgInvoker(TCaller* lhs, void (TCaller::*PMEM_FN)(_Arg*))
			:fn(PMEM_FN)
			, _lhs(lhs)
		{

		}

		// Inherited via IMsgInvoker
		virtual void Invoke(IOCtx* pIoCtx) override
		{			
			fn(_lhs.get(), static_cast< _Arg*>(pIoCtx));
		}
	private:
		PTCaller _lhs;
		PArg _rhs;
		std::function< void(TCaller*, _Arg* ) > fn;
	};


	class IMsgDispatcher
	{
	public:
		using TCont = std::unordered_map< std::type_index, PIMsgInvoker >;
	private:
	};

	class CltCtx : public ICompletionKey
	{
	public:

		// Inherited via ICompletionKey
		virtual void doVisit(Visitor & visitor) override
		{
			visitor.accept(this);
		}

		

		// Inherited via ICompletionKey
		virtual void HandleMsg(IOCtx * rhs) override
		{
		}
	};

	class Listener : public ICompletionKey
	{
	public:
		Listener()
/*
			: handler{
				{ std::type_index(typeid(WriteIO)), New< MsgInvoker<Listener, WriteIO> >(this, &Listener::Handle) } ,
			}
*/
		{
			//MsgInvoker<Listener, WriteIO> inv(this, std::mem_fn( &Listener::Handle ));
			//auto cur = std::make_shared< MsgInvoker<Listener, WriteIO> >(this, &Listener::Handle);
			//auto cur = New< MsgInvoker<Listener, WriteIO> >(this);
			//auto cur = New< MsgInvoker<Listener, WriteIO> >(this, &Listener::Handle);
			//handler[std::type_index(typeid(WriteIO))] = New< MsgInvoker<Listener, WriteIO> >(this, &Listener::Handle);
		}
		virtual void doVisit(Visitor & visitor) override
		{
			visitor.accept(this);
		}

		void Handle(WriteIO& rhs)
		{
			std::cout << __FUNCSIG__ << std::endl;
		}

		void Handle(AcceptIO& rhs  )
		{
			std::cout << __FUNCSIG__ << std::endl;
		}

		// Inherited via ICompletionKey
		virtual void HandleMsg(IOCtx * rhs) override
		{
			std::type_index( typeid(*rhs) );
		}

		std::unordered_map< std::type_index, PIMsgInvoker> handler;
	};




	void DoWork( ICompletionKey* cltCtx, IOCtx* ioCtx)
	{
		assert(nullptr != cltCtx);
		assert( nullptr != ioCtx );

		cltCtx->HandleMsg( ioCtx );
	}
}

IMPL_TEST_FNC(
	DISPATCHERE2
)
{
	NS_VDIS2::PICompKey pKey = New<NS_VDIS2::Listener>() ;
	//std::shared_ptr< NS_VDIS2::ICompletionKey > key = std::make_shared< NS_VDIS2::Listener >();

	NS_VDIS2::PIOCtx pIo = New< NS_VDIS2::WriteIO >();

	NS_VDIS2::Visitor visitor;

	NS_VDIS2::DoWork(pKey.get(), pIo.get());
}