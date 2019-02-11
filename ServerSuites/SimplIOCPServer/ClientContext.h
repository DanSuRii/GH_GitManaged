#pragma once
#include "ICompletionKey.h"
#include "IOMsgDispatcher.h"

namespace NS_DPNET
{

	class ClientCtx : public ICompletionKey
	{
		void Handle(WriteIO& ioWrite, DWORD dwIOSize);
		void Handle(ReadIO& ioRead, DWORD dwIOSize);

#if false //it doesnt work
		template<class _TArg>
		auto GetInitalizeList()
		{
			return { typeid(_TArg), GetCallerFunction<_TArg>() };
		}

#endif // false //it doesnt work

	public:
		ClientCtx();
	private:

		class CltMsgDispatcher : public IOMsgDispatcher<ClientCtx>
		{
			template<class _TArg>
			auto GetCallerFunction()
			{
				return [](ClientCtx* pThis, void* rParam, DWORD dwIOSize) { auto param = (_TArg*)rParam; pThis->Handle(*param, dwIOSize);  };
			}

			template< class _TArg, class _TDispatcher  >
			void Regist(_TDispatcher& dispatcher)
			{
				dispatcher.Regist(typeid(_TArg), GetCallerFunction<_TArg>());
			}
			template< class _TArg>
			void Regist()
			{
				_MsgDispatcherT::Regist(typeid(_TArg), GetCallerFunction<_TArg>());
			}

		public:
			CltMsgDispatcher();
		private:
			std::mutex		mtxInit;
			std::atomic_bool bInit = false;
		};
		static CltMsgDispatcher msgDispatcher;

		inline auto& GetDispatcher()
		{
			return msgDispatcher;
		}

		inline auto GetHandlerFn(std::type_index idx)
		{
			return GetDispatcher().GetFn(idx);
		}

		// Inherited via ICompletionKey
		virtual void HandleIO(IOCtx * pIOCtx, DWORD);
	};
	//ClientCtx::CltMsgDispatcher ClientCtx::msgDispatcher;
}
