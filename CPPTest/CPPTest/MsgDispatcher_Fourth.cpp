#include "pch.h"

#include <WinSock2.h>


#include <tuple>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>



namespace NS_DISPATCHER_FOURTH
{
	template< class T, class ...Args >
	decltype(auto) New(Args&& ...args)
	{
		return std::make_shared< T >( std::forward<Args>( args ) ...);
	}

	class AbstrctVisitor
	{
	public:
		template<class T>
		struct CurTyp
		{
			typedef T myT;

		};

		template<class T>
		auto Dispatch(T& rhs)
		{
			return CurTyp<T>();
		}
	};

	class ConcreteVisitor : public AbstrctVisitor
	{

	};

	class IMsg
	{
	public:
		virtual void visitMsg(AbstrctVisitor&) = 0;
	};
	using PIMsg = std::shared_ptr<IMsg>;

	template< class Derived >
	class MsgVisitImpl
	{

	};
	
	template< class _Consumer, class ...Args >
	class MsgImpl : public IMsg
	{
	public:
		MsgImpl() {};
		MsgImpl(Args&& ...args) {
			argList = std::make_tuple(args ...);
		}
		~MsgImpl() {};		

	private:
		_Consumer*				pConsumer;
		std::tuple< Args ... >	argList;

		// Inherited via IMsg
		virtual void visitMsg(AbstrctVisitor & visitor) override
		{
			visitor.Dispatch(*this);
		}
	};

	template< class _Consumer, class ...Args >
	PIMsg GenerateMsg(_Consumer& consumer, Args&& ...args)
	{
		class ConVisitor : public AbstrctVisitor
		{
		public:
		} visitor;
		consumer.visitConsumer(visitor);
		//return New< MsgImpl<_Consumer, Args ...> >( std::forward( args ) ...);
		//return std::make_shared< MsgImpl< _Consumer, Args ...> >( std::forward<Args>( args ) ... );
		return New< MsgImpl<_Consumer, Args ...> >(std::forward<Args>(args) ...);
	}


	template< class _MyT, class Args >
	class MsgDispatcher
	{

	};

	class MsgQueue
	{
	public:
		void QueueMsg(PIMsg rhs)
		{
			std::lock_guard< decltype(mtxMsgQ) > grd(mtxMsgQ);
			msgs.push(rhs);
		}

		PIMsg GetMsg()
		{
			PIMsg toRet;
			std::lock_guard< decltype(mtxMsgQ) > grd(mtxMsgQ);
			{
				toRet = msgs.front();
				msgs.pop();
			}

			return toRet;
		}

	private:
		void MsgPump()
		{
			msgs.pop();
		}

		std::mutex mtxMsgQ;
		std::queue<PIMsg> msgs;
	};

	class MsgConsumer
	{
	public:
		virtual void visitConsumer(AbstrctVisitor&) = 0;

		void MsgArrival(PIMsg msg) { msgQ.QueueMsg(msg); };

	private:
		MsgQueue msgQ;
	};

	template<class _Derived>
	class MsgConsumerVisitImpl : public MsgConsumer
	{
		// Inherited via MsgConsumer
		virtual void visitConsumer(AbstrctVisitor & visitor ) override
		{
			visitor.Dispatch(static_cast<_Derived&>(*this));
		}
	};

	class MsgConsumer_A : public MsgConsumerVisitImpl<MsgConsumer_A>
	{
		
	};

	class MsgConsumer_B : public MsgConsumerVisitImpl< MsgConsumer_B >
	{

	};

	class IOCtx : public OVERLAPPED
	{

	};

	class WriteIO : public IOCtx
	{

	};
	class ReadIO : public IOCtx
	{

	};

	class AcceptIO : public IOCtx
	{

	};

	enum EHandel
	{
		EH_GV,

		EH_CNT,
	};

	struct CustomMsg
	{

	};
	
	struct CustomMsgA : public CustomMsg
	{

	};

	struct CustomMsgB : public CustomMsg
	{

	};

	void Do() 
	{
		//GetQueuedCompletionStatus
		MsgConsumer_A consumer;
		MsgConsumer& Dest = consumer;		
		
		PIMsg msg = GenerateMsg(Dest, int(3), double(3.2), std::make_pair("Rewe", EH_GV));
		Dest.MsgArrival(msg);
		
	} 
}


IMPL_TEST_FNC(DISPATCHER_FOURTH)
{
	NS_DISPATCHER_FOURTH::Do();
}