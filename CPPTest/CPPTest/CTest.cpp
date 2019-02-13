#include "pch.h"
#include "CTest.h"


//#define DECL_TEST( x ) std::make_shared< TestImpl<#x##_hash> >( #x )	

namespace NS_TEST
{



#if false
	std::shared_ptr<Test> TestRepo::_cont[] = {
DECL_TEST(HWORLD),
DECL_TEST(WORLDTEST),
DECL_TEST(SHAREDTOSHARED),
DECL_TEST(SSTREAMLOG),
DECL_TEST(LIST_SPLICE_INSERT),
DECL_TEST(VARIDARLIC_STRSTREAM),
DECL_TEST(GET_STRUCTPTR_FROM_MEMBER_PTR),
DECL_TEST(TEST_GENERATES),
DECL_TEST(CTX_CONSISTENCY),
DECL_TEST(DISPATCHERE),
DECL_TEST(DISPATCHERE2),
DECL_TEST(HANDLEMSG),
DECL_TEST(TYPEINDICATOR),
DECL_TEST(TRY_MSGDISPATCH_THIRD),
DECL_TEST(INVOKER_TESTS),
DECL_TEST(DISPATCHER_FOURTH),
DECL_TEST(DISPATCHER_FIFTH),
DECL_TEST(GLOBAL_MESSAGE_DISPATCHER),
DECL_TEST(APC_ALERTABLEWAIT_OVERLAPPED),
//DECL_TEST(SSTREAMLOG),
	};

#else

#define REG_TEST( x ) std::make_shared< TestImpl< x > >( #x ),	

	std::shared_ptr<Test> TestRepo::_cont[] = {
#include "tests.decl"
	};

#undef REG_TEST

#endif // false

	//void TestImpl<"HWORLD"_hash >::Do() { std::cout << this->_strName << std::endl; }
	//void TestImpl<"WORLDTEST"_hash>::Do() { std::cout << this->_strName << std::endl; }

	void TestImpl<HWORLD>::Do() { std::cout << this->_strName << std::endl; }
	void TestImpl<WORLDTEST>::Do() { std::cout << this->_strName << std::endl; }


	/*
	std::shared_ptr<Test> TestRepo::_cont[] = {
		std::make_shared< TestImpl<"HWORLD"_hash> >( "HWORLD" ),
		std::make_shared< TestImpl<"WORLD"_hash> > ( "WORLD" ),
	};
	*/


	void TestRepo::DoAll() {
		for (auto cur : _cont)
		{
			cur->Do();

			
		}
	}

}
