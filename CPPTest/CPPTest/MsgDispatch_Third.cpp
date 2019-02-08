#include "pch.h"
#include <WinSock2.h>

#include <typeindex>
#include <unordered_map>

namespace NS_MSGDISPATCHER_THIRD
{


	class ICompletionKey
	{
	public:
		using _TFnHandler = std::function< void(ICompletionKey*, void*) >;

		template<class _TyIO>
		void HandleIO(_TyIO* rhs)
		{
			auto fn = GetHandler(typeid(*rhs));
			fn(this, rhs);
		}

		virtual _TFnHandler GetHandler(std::type_index idx) = 0;
	};

	class IOCtx : OVERLAPPED
	{
	public:
		virtual void InvokeHandle(ICompletionKey& lhs) = 0;
	};

	template< class Derived >
	class IOCtx_CRTPimpl : public IOCtx
	{
	public:

		// Inherited via IOCtx
		virtual void InvokeHandle(ICompletionKey & lhs) override
		{
			lhs.HandleIO(static_cast<Derived*>(this));
		}
	};

	class AcceptIO : public IOCtx_CRTPimpl<AcceptIO>
	{

	};

	class ReadIO : public IOCtx_CRTPimpl<ReadIO>
	{

	};

	class WriteIO : public IOCtx_CRTPimpl<WriteIO>
	{

	};



	class ClientCtx : public ICompletionKey
	{
	public:
		ClientCtx();
	private:


		// Inherited via ICompletionKey
		virtual _TFnHandler GetHandler(std::type_index idx) override
		{
			_TFnHandler NothingHandler([](ICompletionKey*, void*) {
				std::cout << __FUNCSIG__ << " Nothing handler was been Occured" << std::endl;
			});

			auto iter = _mapHandler.find(idx);
			if (iter == _mapHandler.end())
			{
				//second try...
#if false //JUST IN CASE USES CHECK
				for (iter = _mapHandler.begin(); iter != _mapHandler.end(); ++iter)
				{
					if (iter->first == idx)
					{
						return iter->second;
					}
				}

#endif // false //JUST IN CASE USES CHECK

				return NothingHandler;
			}

			

			return iter->second;
		}

		/*
			template< class _TyArg >
			void Handle(_TyArg& );
		*/

		void Handle(ReadIO& readIO);

		std::unordered_map< std::type_index, _TFnHandler > _mapHandler;
	};


	class Listener : public ICompletionKey
	{
	public:

		// Inherited via ICompletionKey
		virtual _TFnHandler GetHandler(std::type_index idx) override
		{
			return _TFnHandler();
		}

		std::unordered_map< std::type_index, _TFnHandler > _mapHandler;
	};

	template< class _T_Receiver, class _T_to_Handle >
	typename ICompletionKey::_TFnHandler GetDispatcher()
	{
		return [](ICompletionKey* pThis, void*rhs) {
			auto pRhs = (_T_to_Handle*)rhs;
			auto _actualThis = dynamic_cast<_T_Receiver*>(pThis);

			_actualThis->Handle(*pRhs);
		};
	}

	ClientCtx::ClientCtx()
	{
		_mapHandler[typeid(ReadIO)] = [](ICompletionKey* pThis, void*rhs) {
			auto pRhs = (ReadIO*)rhs;
			auto _actualThis = dynamic_cast<ClientCtx*>(pThis);

			_actualThis->Handle(*pRhs);
		};
	}
	void ClientCtx::Handle(ReadIO & readIO)
	{
		std::cout << __FUNCSIG__ << std::endl;
	}

}


IMPL_TEST_FNC(
	TRY_MSGDISPATCH_THIRD
)
{
	using namespace NS_MSGDISPATCHER_THIRD;
	ClientCtx cltCtx;
	ICompletionKey& rCompKey = cltCtx;

	WriteIO ioWrite;
	IOCtx& ctxIO = ioWrite;
	ctxIO.InvokeHandle(rCompKey);

	ReadIO ioRead;
	IOCtx& ctxIORead = ioRead;

	ctxIORead.InvokeHandle(rCompKey);
}

