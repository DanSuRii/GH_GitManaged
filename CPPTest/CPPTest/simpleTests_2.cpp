#include "pch.h"

#include <stack>
#include <typeindex>

#include <unordered_map>

namespace NS_TYPEINDICATOR
{


	class ITypeIndicator
	{

	};
	using PITypeIndicator = std::shared_ptr<ITypeIndicator>;

	template<class _Ty>
	class TypeIndicator : public ITypeIndicator
	{
	public:
		typedef _Ty internalTyp;
		const std::type_index index = typeid(_Ty);
	};


	class AbstractDispatcher
	{
	public:
		template< class _Ty >
		void Dispatch(_Ty* rhs)
		{
			CollectType(std::make_shared< TypeIndicator<_Ty> >());
		}
	protected:
		virtual void CollectType(PITypeIndicator) = 0;
	private:
	};

	class IVisitable
	{
	public:
		virtual void visit(AbstractDispatcher&) = 0;
	};

	class GV : public IVisitable
	{
	};

	template< class Derived >
	class GV_CRTPimpl : public GV
	{
	public:
	private:

		// Inherited via GV
		virtual void visit(AbstractDispatcher & dispatcher) override
		{
			dispatcher.Dispatch(static_cast<Derived*>(this));
		}
	};

	class Markant : public GV_CRTPimpl<Markant>
	{
	public:

		template< class TMsg >
		void Handle(TMsg& rhs);
	};


	class Rewe : public GV_CRTPimpl<Rewe>
	{
	public:
	};


	class Behavior : public IVisitable
	{

	};

	template< class Derived >
	class Behavior_CRTPImpl : public Behavior
	{
	public:
	private:

		// Inherited via Behavior
		virtual void visit(AbstractDispatcher & dispatcher) override
		{
			dispatcher.Dispatch(static_cast<Derived*>(this));
		}
	};

	class Bestellung : public Behavior_CRTPImpl<Bestellung>
	{

	};

	class Rechnung : public Behavior_CRTPImpl<Rechnung>
	{
	};

	class Belastung : public Behavior_CRTPImpl<Belastung>
	{
	};

	class Gutschrift : public Behavior_CRTPImpl<Gutschrift>
	{
	};

	template<>
	void Markant::Handle(Rechnung & rhs)
	{
	}


	class MsgDispatcher
	{
	public:
		template< class Receiver, class Msg >
		void Dispatch(Receiver& lhs, Msg& rhs)
		{

		}
	};

	class ConcreteDispatcher : public AbstractDispatcher
	{
	public:
	private:
		// Inherited via AbstractDispatcher
		virtual void CollectType(PITypeIndicator indicator) override
		{
			args.push(indicator);
		}

		std::stack< PITypeIndicator > args;
	};

}

IMPL_TEST_FNC(TYPEINDICATOR)
{
	using namespace NS_TYPEINDICATOR;
	Rechnung rg;
	Behavior& rBehavior = rg;

	Markant mkt;
	GV& rReceiver = mkt;

	std::cout << typeid(rBehavior).name() << std::endl;
	std::cout << typeid(rReceiver).name() << std::endl;

	ConcreteDispatcher dispatcher;

	rReceiver.visit(dispatcher);
	rBehavior.visit(dispatcher);
}

namespace NS_INVOKER_TESTS
{
	template< class Origin, class ...Args >
	void GetInvoker(Args&& ... args)
	{

	}

	class MsgDispatcher
	{
	public:
		MsgDispatcher()
		{

		}

		template< class ...Args >
		void HandleRequest()
		{
		}

	};

	class ICaller
	{
	public:
	};

	class ACaller : public ICaller
	{

	};

	class BCaller : public ICaller
	{

	};

	class ICallee
	{
	public:
		template< class ... Args >
		void Handle( Args&& ...args )
		{

		}

		
	};

	class ACallee : public ICallee
	{
	public:
		void Handle(BCaller* arg1)
		{

		}

		void Handle(ACaller* arg1, int arg2)
		{
			std::cout << __FUNCSIG__ << std::endl;			
		}

		template< class ... Args >


		ACallee()
		{
			auto toCall = &ACallee::Handle;
		}
	};

}

IMPL_TEST_FNC(INVOKER_TESTS)
{
	using namespace NS_INVOKER_TESTS;
	ACallee aCallee;
	ICallee& callee = aCallee;

	BCaller bCaller;
	ICaller& caller = bCaller;

	int nExtInfo = 3;

	
	
}