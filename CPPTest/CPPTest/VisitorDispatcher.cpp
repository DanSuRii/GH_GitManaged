#include "pch.h"

#include <WinSock2.h>

class AbstractDispatcher
{
public:
	template< class ClassCtx >
	void dispatch(ClassCtx& lhs)
	{
	}
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
	virtual void accept(AbstractDispatcher& dispatcher)
	{
		dispatcher.dispatch( *(static_cast< Derived* >(this)) );
	}
};

class ClientCtx : public IAcceptCompletion<ClientCtx>
{
public:
	void Handle(IORead* ioCtx);
	void Handle(IOWrite* ioCtx);
};

class Listener : public IAcceptCompletion<Listener>
{
public:
	void Handle(IOAccept* ioCtx);
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
auto New(Args&& ...args)
{
	return std::make_shared< T >(args ...);
}

using PICompletionKey = std::shared_ptr<ICompletionKey>;
using PIOCtx = std::shared_ptr<IOCtx>;

IMPL_TEST_FNC(DISPATCHERE)
{
	PICompletionKey listener = New< Listener >();

	PIOCtx ioaccept = New<IOAccept>();

	AbstractDispatcher dispatcher;

	listener->accept(dispatcher);
}