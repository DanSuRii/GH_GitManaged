#include "pch.h"
#include <WinSock2.h>

#include <list>
#include <memory>

namespace NS_VISIT_ITERATIOR
{
	template< class ... Args >
	void Log(Args&& ... args)
	{
		int a[]{ ( std::cout << args ,0) ... };
		std::cout << "\n";

		return;
	}

#define LOG_FN(...) Log( __FUNCSIG__, __VA_ARGS__ )

	class AbstractVisitor
	{
	public:
		template<class T>
		void Dispatch(T* cur)
		{
		}

		virtual void DoCall(AbstractVisitor&)
		{
			LOG_FN( ", was been called" );
		}
	};
	using PVisitor = std::shared_ptr<AbstractVisitor>;

	class VisitorComposite : private AbstractVisitor
	{
	public:

		void DoCall()
		{
			if (visitors.size() < 2)
			{
				LOG_FN( ", Not enough visitors" );
				return;
			}
			auto rBegin = visitors.rbegin();
			auto rNext = ++rBegin;

			if (rNext == visitors.rend())
			{
				//Failed to call
				return ;
			}

			(*rBegin)->DoCall(*(*rNext));
		}

		//void Add(AbstractVisitor* toAdd)
		//{
		//	visitors.push_back(toAdd);
		//}

		template< typename T, class ... Args >
		void EmplaceBack( Args&& ... args )
		{
			visitors.push_back( std::make_shared<T>( std::forward<Args>(args) ... ) );
		}

		void Add(PVisitor&& cur)
		{
			visitors.push_back( cur );
		}

	private:
		std::list<PVisitor> visitors;
	};

	//Leaf
	template< typename _CurT >
	class VisitorImpl : public AbstractVisitor
	{
	public:
		VisitorImpl(_CurT* pCur)
			: _pCur(pCur)
		{
			
		}

		template< class _RightT >
		void DoCall(VisitorImpl<_RightT>& rhs)
		{			
			//_pCur->(rhs.get());
			LOG_FN( ", was been called" );
		}

		virtual void DoCall(AbstractVisitor& rhs)
		{
			//rhs.DoCall < VisitorImpl<_CurT> >(*this);
		}


		inline _CurT* Get() { return _pCur; }

	private:
		_CurT* _pCur;
	};

	class IOCtx : public OVERLAPPED
	{
	public:
		virtual void accept(VisitorComposite& visitor) = 0;
	};
	
	class ICompletionKey
	{
	public:
		virtual void accept(VisitorComposite& visitor, IOCtx*) = 0;
	};

	class ClientCtx : public ICompletionKey
	{	
	public:
		// Inherited via ICompletionKey
		virtual void accept(VisitorComposite & visitor, IOCtx* pNext ) override
		{
			//VisitorImpl<ClientCtx> curVisitor(this);
			//visitor.join(curVisitor);
			visitor.EmplaceBack< VisitorImpl<ClientCtx> >( this ) ;
			//visitor.Add(std::make_shared< VisitorImpl<ClientCtx>>(this));
			
		}
	};

	class Listener : public ICompletionKey
	{

	};

	class AcceptIO : public IOCtx
	{
	public:
		virtual void accept(VisitorComposite& visitor)
		{
			visitor.EmplaceBack< VisitorImpl<AcceptIO> >(this);
		}
	};

	class ReadIO : public IOCtx
	{

	};

	class WriteIO : public IOCtx
	{

	};


	void Do()
	{
		VisitorComposite comp;
		ClientCtx cltCtx;
		AcceptIO acceptIO;

		cltCtx.accept(comp, &acceptIO);
		acceptIO.accept(comp);

		comp.DoCall();
	}
}

IMPL_TEST_FNC(VISIT_ITERATIOR)
{
	NS_VISIT_ITERATIOR::Do();
}