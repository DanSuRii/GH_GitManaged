#include "pch.h"

#include <WinSock2.h>

#include <unordered_map>
#include <typeindex>
#include <mutex>


class AbstractDispatcher
{
public:
	template< class ClassCtx >
	void dispatch(ClassCtx& lhs)
	{
	}
};

class MsgDispatcher
{
public:
	class IMsgInvoker
	{
	public:
		virtual void Invoke() = 0;
	};

	template< class CltCtx, class IoCtx >
	class MsgInvoker : public IMsgInvoker
	{
	public:
		using PCltCtx = std::shared_ptr< CltCtx >;
		using PIoCtx = std::shared_ptr< IoCtx >;

		std::type_index idxClt = typeid(CltCtx);
		std::type_index idxIo = typeid(IoCtx);		

		virtual void Invoke() override
		{
			lhs->Handler(rhs);
		}

	private:
		PCltCtx lhs;
		PCltCtx rhs;

		// Inherited via IMsgInvoker		
	};

	MsgDispatcher();

	//std::unordered_map< std::tuple< std::type_index, std::type_index >, std::function< void(void) > > mapToFind;
	//std::unordered_map< std::pair< std::type_index, std::type_index >, std::function< void(void) > > mapToFind;
};


class IOCtx : public OVERLAPPED
{
public:
	virtual void accept(AbstractDispatcher&) = 0;
};

template< class Derived >
class IAcceptIOCtx : public IOCtx
{
	virtual void accept(AbstractDispatcher& dispatcher)
	{
		dispatcher.dispatch(*(static_cast<Derived*>(this)));
	}
};

class IOAccept : public IAcceptIOCtx<IOAccept>
{

};

class IORead : public IAcceptIOCtx<IORead>
{

};

class IOWrite : public IAcceptIOCtx<IOWrite>
{

};


class ICompletionKey
{
public:
	virtual void accept(AbstractDispatcher&) = 0;
};

template< class Derived >
class IAcceptCompletion : public ICompletionKey
{
public:
	using TMsgMap = std::unordered_map< std::type_index, std::function<void(Derived&, IOCtx*)> >;

	IAcceptCompletion()
	{
		/*
		if (msgHandler.size() <= 0)
		{
			std::lock_guard< std::mutex > grd( mtxForInit );
			static_cast<Derived*>(this)->InitMsgHandler();
		}
		*/
	}

	virtual void accept(AbstractDispatcher& dispatcher) override
	{
		dispatcher.dispatch( *(static_cast< Derived* >(this)) );
	}

protected:
/*
	static std::mutex mtxForInit;
	static TMsgMap msgHandler;
*/
};

/*
template< class T >
std::mutex IAcceptCompletion<T>::mtxForInit;

template< class T >
typename IAcceptCompletion<T>::TMsgMap IAcceptCompletion<T>::msgHandler;
*/


class ClientCtx : public IAcceptCompletion<ClientCtx>
{
public:
	void Handle(IORead* ioCtx)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}
	void Handle(IOWrite* ioCtx)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}

protected:
	void InitMsgHandler()
	{
		//msgHandler[std::type_index(typeid(IORead))] = std::bind(static_cast<void(ClientCtx::*)(IORead*)>(&ClientCtx::Handle), std::placeholders::_1, std::placeholders::_2);
	}
};

class Listener : public IAcceptCompletion<Listener>
{
public:
	void Handle(IOAccept* ioCtx)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}
	void InitMsgHandler()
	{
		std::cout << __FUNCSIG__ << std::endl;
	}


private:
};

class Dispatcher
{
public:
	template< class CltCtx, class IOCtx >
	void Handle( CltCtx* left, IOCtx* right )
	{
		left->Handle(right);
	}
};



template< class T, class ... Args >
std::shared_ptr<T> New(Args&& ...args)
{
	return std::make_shared< T >( std::forward<Args>( args ) ...);
}

using PICompletionKey = std::shared_ptr<ICompletionKey>;
using PIOCtx = std::shared_ptr<IOCtx>;

MsgDispatcher::MsgDispatcher()
{
	using namespace std::placeholders;

	auto cur = static_cast<void(ClientCtx::*)(IOWrite*)>(&ClientCtx::Handle);
	//mapToFind[{typeid(ClientCtx), typeid(IOAccept)}] = std::bind(static_cast<void(ClientCtx::*)(IOAccept*)>(&ClientCtx::Handle), _1, _2);
}
class RAII
{
public:
	RAII(std::function<void(void)>fn)
		:_fn (fn)
	{

	}

	~RAII()
	{
		if (_fn)
			_fn();
	}
private:
	std::function<void(void)> _fn;
};


IMPL_TEST_FNC(DISPATCHERE)
{
	/*
	PICompletionKey listener = New< Listener >();

	PIOCtx ioaccept = New<IOAccept>();

	listener->accept(dispatcher);
	*/
	/*
	AbstractDispatcher dispatcher;
	Listener* listener = new Listener();
	RAII grd([&] { delete listener; });
	ICompletionKey* complKey = dynamic_cast<ICompletionKey*>(listener);
	*/

	//if(complKey) complKey->accept(dispatcher);
}