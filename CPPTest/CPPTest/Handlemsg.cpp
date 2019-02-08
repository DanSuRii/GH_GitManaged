#include "pch.h"
#include <WinSock2.h>

#include <cassert>

#include <unordered_map>
#include <typeindex>

class ICompletioKey;

class IOCtx : public OVERLAPPED
{
public:
	virtual void InvokeHandleMsg(ICompletioKey*) = 0;
};

template<class Derived>
class CRTPIoctx :public IOCtx
{
public:
	// Inherited via IOCtx
	virtual void InvokeHandleMsg(ICompletioKey * pComp) override;
};

class AcceptIO : public CRTPIoctx<AcceptIO>
{
public:
private:
};


class ReadIO : public CRTPIoctx<ReadIO>
{
public:
private:
};


class WriteIO : public CRTPIoctx<WriteIO>
{
public:
private:
};



class ICompletioKey
{
public:
	template<class T>
	void HandleMsg(T& msg)
	{
		auto iter = mapToCall.find(typeid(msg));
		if (iter == mapToCall.end())
		{
			return;
		}
	};

protected:
	std::unordered_map< std::type_index, std::function<void(void)> > mapToCall;
};

class ClientCtx : public ICompletioKey
{
public:

	ClientCtx()
		:_map{
			{ typeid(ReadIO), [&](ClientCtx* pThis, IOCtx* ptrMsg) { pThis->Handle(dynamic_cast<ReadIO*>(ptrMsg)); } },
		}
	{
		//_map[typeid(ReadIO)] = [&](IOCtx* ptrMsg) { this->Handle(dynamic_cast<ReadIO*>(ptrMsg)); };
	}
	

	void HandleMsg(IOCtx* msg)
	{
		auto iter = _map.find( std::type_index(typeid(msg)) );
		if (iter != _map.end())
		{
			iter->second(this, msg);
		}
	}
	
private:
	void Handle(ReadIO* ptrMsg)
	{

	}


	std::unordered_map < std::type_index, std::function< void(ClientCtx*  , IOCtx*) > > _map;
};

class Listener : public ICompletioKey
{
public:
};
/*

*/

using PIComplKey = std::shared_ptr< ICompletioKey >;
using PIOCtx = std::shared_ptr< IOCtx >;

template< class _Ret, class ... Args >
auto New(Args&& ...args)
{
	return std::make_shared< _Ret >(std::forward<Args>(args)...);
}


class MsgDispatcher
{
public:
	void Handle(Listener*, AcceptIO*)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}
	void Handle(ClientCtx*, ReadIO*)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}
	void Handle(ClientCtx*, WriteIO*)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}
	void Handle(void*, void*)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}
	//void Handle(Listener*, AcceptIO*) {}

};

namespace DP_TEST
{
	class MsgHandler
	{
		template< class _CPKey, class _IOCtx >
		void HandleMsg(_CPKey*, _IOCtx*);
	};

	template<>
	void MsgHandler::HandleMsg< Listener, AcceptIO >(Listener*, AcceptIO* pIOAccept)
	{

	}
}
template<class Derived>
void CRTPIoctx<Derived>::InvokeHandleMsg(ICompletioKey * pComp)
{
	if (nullptr == pComp)
	{
		_CrtDbgBreak();
		return;
	}

	pComp->HandleMsg(*(static_cast<Derived*>(this)));
}


IMPL_TEST_FNC(HANDLEMSG)
{
	PIComplKey pComplKey = New< Listener >();
	PIOCtx pIoctx = New< WriteIO >();


	MsgDispatcher dispatcher;
	dispatcher.Handle(pComplKey.get(), pIoctx.get());
}

// Inherited via IOCtx
