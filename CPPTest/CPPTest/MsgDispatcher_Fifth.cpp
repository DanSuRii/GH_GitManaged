#include "pch.h"

namespace NS_DISPATCHER_FIFTH
{
	template< class ... Args >
	void Log( Args&& ...args )
	{
		int a[]{ (std::cout << args << std::endl, 0) ... };
		static_cast<void>(a);
	}

	class AbstractVisitor
	{
	public:
		virtual void Dispatch() = 0;
	};

	class IMsg
	{
	public:

		virtual void visit(AbstractVisitor&) = 0;
	};

	template< int >
	class MsgImpl : public IMsg
	{
	public:
		virtual void visit(AbstractVisitor&)
		{

		}
	};

	template<class T>
	class ConcreteVisitor : public AbstractVisitor
	{
		// Inherited via AbstractVisitor
		virtual void Dispatch() override
		{
		}
	};

	class Recipient
	{
	public:
		template< class ...Args >
		void Handler(Args&& ...args)
		{
			Log("Unspecified Handler with Args");
		}
		void HandleMsg(MsgImpl<2>& msg)
		{

		}

		void HandleMsg(MsgImpl<4>& msg)
		{

		}
	}; 




	class MessageDispatcher
	{
	public:
		void HandleMsg(Recipient& recipient, IMsg& msg)
		{
			//recipient.HandleMsg(msg);



		}
	};

	/*
		template<>
		void Recipient::Handler< int, double, int >( int, double, int )
		{

		}
	*/

	void Do()
	{
		MsgImpl<5> iMsg;
		IMsg& msg = iMsg;
	}
}

IMPL_TEST_FNC(DISPATCHER_FIFTH)
{
	NS_DISPATCHER_FIFTH::Do();
}